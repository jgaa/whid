#include "workmodel.h"
#include "nodemodel.h"
#include "utility.h"

#include <QSqlRecord>
#include <QSqlField>
#include <QSqlError>
#include <QDebug>
#include <array>
#include <QSqlQuery>

using namespace std;

WorkModel::WorkModel(NodeModel& nm)
    : nodeModel_{nm}
{
    setFilter("id=-1"); // Show nothing
    setTable("work");
    setSort(fieldIndex("start"), Qt::AscendingOrder);
}

void WorkModel::addWork(Work::ptr_t work)
{
    auto node = work->node.lock();
    if (!node) {
        qWarning() << "No node";
        return;
    }

    QSqlQuery query;

    QString sql = "INSERT INTO work (status, start, end, used, paused, name, node) "
                  "VALUES (:status, :start, :end, :used, :paused, :name, :node)";

    query.prepare(sql);
    query.bindValue(":status", work->getStatusId());
    query.bindValue(":start", work->start.toTime_t());
    query.bindValue(":end", work->end.toTime_t());
    query.bindValue(":used", (work->start.msecsTo(work->end) / 1000) - work->paused);
    query.bindValue(":paused", work->paused);
    query.bindValue(":name", work->name);
    query.bindValue(":node", node->id);

    if (!query.exec()) {
        qWarning() << "Failed to add work. error:  " << query.lastError();
    }

    // Just reselect everything
    select();

//    //insertRecord is broken after upgrade
//    QSqlRecord r;

//    r.append(QSqlField{"status", QVariant::Int});
//    r.append(QSqlField{"start", QVariant::LongLong});
//    r.append(QSqlField{"end", QVariant::LongLong});
//    r.append(QSqlField{"used", QVariant::Int});
//    r.append(QSqlField{"paused", QVariant::Int});
//    r.append(QSqlField{"name", QVariant::String});
//    r.append(QSqlField{"node", QVariant::Int});

//    r.setValue("status", work->getStatusId());
//    r.setValue("start", work->start.toTime_t());
//    r.setValue("end", work->end.toTime_t());
//    r.setValue("used", (work->start.msecsTo(work->end) / 1000) - work->paused);
//    r.setValue("paused", work->paused);
//    r.setValue("name", work->name);
//    r.setValue("node", node->id);

//    if (!insertRecord(-1, r)) {
//        qWarning() << "Failed to insert data: " << this->lastError();
//        return;
//    }
//    if (!submitAll()) {
//        qWarning() << "Failed to commit data: " << lastError();
//    }
}

QVariant WorkModel::data(const QModelIndex &ix, int role) const
{
    static const int statusCol = fieldIndex("status");
    static const int startCol = fieldIndex("start");
    static const int endCol = fieldIndex("end");
    static const int usedCol = fieldIndex("used");
    static const int pausedCol = fieldIndex("paused");

    if (ix.isValid()) {
        if (ix.column() == statusCol) {
            if (role == Qt::DisplayRole) {
                if (auto node = getNode(ix)) {
                    QString path;
                    node->getPath(path);
                    return path;
                }
            }
            if (role == Qt::DecorationRole) {
                auto nix = index(ix.row(), statusCol, {});
                int status = QSqlTableModel::data(nix, Qt::DisplayRole).toInt();
                return Work::getStatusIcon(status);
            }
            if (role == Qt::ToolTipRole) {
                auto nix = index(ix.row(), statusCol, {});
                int status = QSqlTableModel::data(nix, Qt::DisplayRole).toInt();
                return Work::getStatusText(status);
            }
        }
        if (ix.column() == startCol && role == Qt::DisplayRole) {
            const auto when = QDateTime::fromTime_t(
                        QSqlTableModel::data(ix, Qt::DisplayRole).toUInt());
            return when.toString("yyyy-MM-dd hh:mm");
        }

        if (ix.column() == endCol && role == Qt::DisplayRole) {
            const auto when = QDateTime::fromTime_t(
                        QSqlTableModel::data(ix, Qt::DisplayRole).toUInt());
            return when.toString("hh:mm");
        }

        if ((ix.column() == usedCol || ix.column() == pausedCol)
            && role == Qt::DisplayRole) {
            return toHourMin(QSqlTableModel::data(ix, Qt::DisplayRole).toInt());
        }
    }

    return QSqlTableModel::data(ix, role);
}

Qt::ItemFlags WorkModel::flags(const QModelIndex &ix) const
{
    static const int nameCol = fieldIndex("name");

    if (ix.isValid()) {
        if (ix.column() != nameCol) {
            return QSqlTableModel::flags(ix) & ~Qt::EditRole;
        }
    }

    return QSqlTableModel::flags(ix);
}

QVariant WorkModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    static const QIcon editIcon(":/res/icons/edit.svg");
    static const int nameCol = fieldIndex("name");
    static const int statusCol = fieldIndex("status");

    if (role == Qt::DecorationRole && orientation == Qt::Horizontal) {
        if (section == nameCol) {
            return editIcon.pixmap(12,12);
        }
    }

    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if (section == statusCol) {
            return "Status and Task";
        } else {
            auto name = QSqlTableModel::headerData(section, orientation, role).toString();
            name[0] = name[0].toUpper();
            return name;
        }
    }

    return QSqlTableModel::headerData(section, orientation, role);
}


QVariant Work::getStatusIcon() const
{
    return getStatusIcon(getStatusId());
}

QVariant Work::getStatusIcon(int status)
{
    static const array<QIcon, 7> icons = {{
        QIcon(":/res/icons/open.svg"),
        QIcon(":/res/icons/done.svg"),
        QIcon(":/res/icons/interrupted.svg"),
        QIcon(":/res/icons/heldback.svg"),
        QIcon(":/res/icons/free.svg"),
        QIcon(":/res/icons/approved.svg"),
        QIcon(":/res/icons/invoiced.svg"),
    }};

    return icons.at(static_cast<size_t>(status));
}

QString Work::getStatusText(int status)
{
    static const array<QString, 7> text = {{
        "Open",
        "Completed",
        "Interrupted - pending manual correction",
        "Held back",
        "Completed, Free (of charge)",
        "Completed and Approved",
        "Completed and Invoiced"
    }};

    return text.at(static_cast<size_t>(status));
}

void Work::setStatus(int newStatus)
{
    static const array<Status, 7> statuses = {{
        Work::Status::OPEN,
        Work::Status::DONE,
        Work::Status::INTERRUPTED, // App closed / crashed without updating the end time
        Work::Status::HELD_BACK, // Manual hold-back
        Work::Status::FREE,
        Work::Status::APPROVED,
        Work::Status::INVOICED
    }};

    status = statuses.at(static_cast<size_t>(newStatus));
}

std::shared_ptr<Node> WorkModel::getNode(const QModelIndex &ix) const
{
    static const int nodeCol = fieldIndex("node");

    auto nix = index(ix.row(), nodeCol, {});
    const auto node_id = QSqlTableModel::data(nix, Qt::DisplayRole).toInt();
    return nodeModel_.getNodeFromId(node_id);
}
