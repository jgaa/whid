
#include <QDate>
#include <QDateTime>
#include <QSqlError>
#include <QDebug>
#include "reportmodel.h"
#include "utility.h"

ReportModel::ReportModel()
{
    setFilter("node=-1");
}

void ReportModel::setFilter(const QString &filter)
{
    const static std::array<QString, 4> modes = {
      "%Y-%m-%d", "%Y #%W", "%Y-%m", "%Y"
    };

    const auto query = QString{"select "
            "strftime('%1', start, 'unixepoch') as start_date, "
            "sum(used) as Hours, "
            "start as first_item "
            "from work "
            "where %2 "
            "group by start_date "
            "order by start"}.arg(modes.at(static_cast<size_t>(mode_)), filter);

    setQuery(query);
    if (lastError().isValid())
         qDebug() << "Query failed: " << query << ", error: " << lastError();
}

QVariant ReportModel::data(const QModelIndex &ix, int role) const
{
    /* Fields:
     *   - When: text
     *   - Hours: minutes
     *   - Baseline: Virtual, minutes we were supposed to be working
     *   - Diff: virtual, Hours - Baseline
     */

    if (role != Qt::DisplayRole) {
        return {};
    }

    if (ix.isValid()) {
        if (ix.row() != row_cache_.row) {
            const_cast<ReportModel&>(*this).fetchCache(ix.row());
        }

        switch(ix.column()) {
        case 0: return row_cache_.label;
        case 1: return toHourMin(row_cache_.work);
        case 2: return toHourMin(row_cache_.baseline);
        case 3: return row_cache_.diff() / 60 / 60;
        }
    }

    QSqlQueryModel::data(ix, role);
}

QVariant ReportModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch(section) {
        case 0: return "When";
        case 1: return "Hours";
        case 2: return "Baseline";
        case 3: return "Diff";
        }
    }

    return {};
}

int ReportModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

void ReportModel::fetchCache(const int row)
{
    row_cache_.row = row;
    row_cache_.label = QSqlQueryModel::data(createIndex(row, 0), Qt::DisplayRole).toString();
    row_cache_.work = QSqlQueryModel::data(createIndex(row, 1), Qt::DisplayRole).toInt();
    row_cache_.baseline = computeBaseline(row);
}

int ReportModel::computeBaseline(int row)
{
    const auto time = QSqlQueryModel::data(createIndex(row, 2), Qt::DisplayRole).toLongLong();
    QDateTime dt = QDateTime::fromTime_t(time);
    auto date = dt.date();
    int baseline = 0;
    auto enddate = QDateTime::currentDateTime().date();

    // Adjust the start-date according to the current mode
    switch(mode_) {
    case Mode::DAYS:
        break;
    case Mode::WEEKS:
        for(; date.day() != Qt::Monday; date = date.addDays(-1))
            ;
        break;
    case Mode::MONTHS:
        date = {date.year(), date.month(), 1};
        break;
    case Mode::YEARS:
        date = {date.year(), 1 /* January */, 1};
        break;
    }

    for(auto d = date;d <= enddate;) {
        // For now, just assume all days except saturday and sundays are work days
        const auto day = d.dayOfWeek();
        if (day < Qt::Saturday) {

            // TODO: Make  hours per day configurable
            baseline += 8 * 60 * 60;
        }

        d = d.addDays(1);

        switch(mode_) {
        case Mode::DAYS:
            return baseline;
        case Mode::WEEKS:
            if (date.weekNumber() != d.weekNumber()) {
                return baseline;
            }
            break;
        case Mode::MONTHS:
            if (date.month() != d.month()) {
                return baseline;
            }
            break;
        case Mode::YEARS:
            if (date.year() != d.year()) {
                return baseline;
            }
            break;
        }
    }

    return baseline;
}
