#include "summarymodel.h"
#include "nodemodel.h"
#include "utility.h"
#include "assert.h"
#include <functional>

#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

using namespace std;


SummaryModel::SummaryModel(NodeModel &nm)
    : node_model_{nm}
{
    loadData();
    schedulaAtMidnight();
}

void SummaryModel::modeChanged(const SummaryModel::Mode mode)
{
    mode_ = mode;
    dataChanged();
}

void SummaryModel::whenChanged(const SummaryModel::When when)
{
    when_ = when;
    dataChanged();
}

void SummaryModel::dataChanged()
{
    beginResetModel();
    loadData();
    endResetModel();
}

void SummaryModel::setSelectedWeek(const QDate &date)
{
    if (when_ == When::WEEK_NUMBER) {
        weekSelection = date;
        dataChanged();
    }
}

int SummaryModel::rowCount(const QModelIndex&) const
{
    return static_cast<int>(rows_.size());
}

int SummaryModel::columnCount(const QModelIndex &) const
{
    return static_cast<int>(headers_.size());
}

QVariant SummaryModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid()) {
        if (role == Qt::DisplayRole) {
            const auto value = rows_.at(static_cast<size_t>(index.row())).at(static_cast<size_t>(index.column()));
            if (value.type() == QVariant::Int) {
                return toHourMin(value.toInt());
            }
            return value;
        }

        if (role == Qt::FontRole) {
            if (index.row() == (static_cast<int>(rows_.size()) -1) || index.column() == 8) {
                QFont font;
                font.setBold(true);
                return font;
            }
        }
    }

    return {};
}

QVariant SummaryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return headers_.at(static_cast<size_t>(section));
    }

    return {};
}

void SummaryModel::loadData()
{
    headers_.clear();
    rows_.clear();

    switch(mode_) {
        case Mode::WEEK:
            loadWeek();
            break;
    }
}

void SummaryModel::loadWeek()
{
    enum Fields {
        F_ORIGIN,
        F_NODE,
        F_MINUTES,
        F_DAY
    };
    headers_ = {"Task", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun", "Sum"};

    array<int, 8> summary{};

    QDate firstday, lastday;
    getDateSpan(firstday, lastday);

    // Query all relevant data from that period
    QSqlQuery query;
    query.prepare("SELECT ( "
                  "    WITH RECURSIVE "
                  "        parent_of(name, id, parent) AS ("
                  "            VALUES(node.name, node.id, node.parent)"
                  "            UNION ALL"
                  "            SELECT node.name, node.id, node.parent FROM node JOIN parent_of ON node.id = parent_of.parent"
                  "            ORDER BY node.name"
                  "        )"
                  "    select GROUP_CONCAT(parent_of.name,'/') as task  FROM parent_of"
                  "    ) as origin,"
                  "    work.node as nodeid, SUM(work.used) as seconds, date(work.start, 'unixepoch') as day"
                  "    FROM work "
                  "    LEFT JOIN node on node.id = work.node"
                  "    where day >= date('" + firstday.toString("yyyy-MM-dd") + "') and day <= date('" + lastday.toString("yyyy-MM-dd") + "')"
                  "    group by day, nodeid"
                  "    order by node, day"
);

    // Iterate over the data and add rows
    if (!query.exec()) {
        qWarning() << "Failed to query work table: " << query.lastError();
        return;
    }

    map<int, vector<QVariant>> nodes;

    while(query.next()) {

        // Calculate column
        const auto date = query.value(F_DAY).toDate();
        const auto node_id = query.value(F_NODE).toInt();
        const auto day = static_cast<size_t>(date.toJulianDay()
                                             - weekSelection.toJulianDay());
        assert(day < summary.size());

        // Make sure the row contains one entry for each day.
        auto& row = nodes[node_id];
        if (row.size() < headers_.size()) {
            // new node
            row.resize(headers_.size());
            row[0] = query.value(F_ORIGIN).toString();
        }

        const auto minutes = query.value(F_MINUTES).toInt();
        row[day + 1] = minutes;
        row[headers_.size() -1] = row[headers_.size() -1].toInt() + minutes;
        summary[day] += minutes;
        summary[headers_.size() -2] += minutes;
    }

    for(auto it: nodes) {
        rows_.push_back(move(it.second));
    }

    std::sort(rows_.begin(), rows_.end(), [](const auto &left, const auto& right) {
       return left[0] < right[1];
    });

    // Insert summary at the end
    rows_.push_back({});
    rows_.back().push_back({}); // node name col
    for(const auto v : summary) {
        if (v) {
            rows_.back().push_back(v);
        } else {
            rows_.back().push_back({});
        }
    }
}

void SummaryModel::getDateSpan(QDate &firstday, QDate &lastday)
{
    if (mode_ == Mode::WEEK) {
        switch(when_) {
            case When::CURRENT:
                weekSelection = QDate::currentDate();
                emit selectionTextChanged("");
                break;
            case When::PREVIOUS:
                weekSelection = QDate::currentDate().addDays(-7);
                emit selectionTextChanged("");
                break;
            case When::WEEK_NUMBER:
                emit selectionTextChanged("Week #"
                    + QString::number(weekSelection.weekNumber())
                    + " " + QString::number(weekSelection.year()));
                break;
        }

        const auto todays_weekday = weekSelection.dayOfWeek();
        weekSelection = weekSelection.addDays(-(todays_weekday-1));

        firstday = weekSelection;
        lastday = firstday.addDays(6);
    }
}

void SummaryModel::atMidnight()
{
    dataChanged();
    schedulaAtMidnight();
}

void SummaryModel::schedulaAtMidnight()
{
    ::scheduleAtMidnight([this](){
        atMidnight();
    });
}
