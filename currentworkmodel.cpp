#include "currentworkmodel.h"

#include <QDebug>
#include <QIcon>
#include <array>

using namespace std;

void CurrentWorkModel::initialize()
{
    timer_ = make_unique<QTimer>();
    connect(timer_.get(), &QTimer::timeout, this, &CurrentWorkModel::updateTime);
    timer_->start(10000);
}

QVariant CurrentWorkModel::data(const QModelIndex &index, int role) const
{
    static const array<QIcon, 2> icons = {{
        QIcon{":/res/icons/pause.svg"},
        QIcon{":/res/icons/active.svg"}
    }};

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch(index.column()) {
            case HN_FROM:
                return  work_.at(static_cast<size_t>(index.row()))->work->start.toString("HH:mm");
            case HN_TO:
                return  work_.at(static_cast<size_t>(index.row()))->work->end.toString("HH:mm");
            case HN_PAUSE:
                return  toHourMin(work_.at(static_cast<size_t>(index.row()))->getPaused());
            case HN_USED:
                return  toHourMin(work_.at(static_cast<size_t>(index.row()))->getWorkedDuration());
            case HN_NAME:
                return  work_.at(static_cast<size_t>(index.row()))->work->name;
        }
    } else if (role == Qt::DecorationRole && index.column() == 0) {
        const auto state = work_.at(static_cast<size_t>(index.row()))->current_state;
        return icons.at(state).pixmap({16,16});
    }

    return {};
}

QVariant CurrentWorkModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    static const QIcon editIcon(":/res/icons/edit.svg");
    static const array<QString, 5> names = {{"From", "To", "Pause", "Used", "What"}};

    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return names.at(static_cast<size_t>(section));
    } else if (role == Qt::DecorationRole && orientation == Qt::Horizontal) {
        if (section == CurrentWorkModel::HN_PAUSE || section == CurrentWorkModel::HN_NAME) {
            return editIcon.pixmap(12,12);
        }
    }

    return {};
}

Qt::ItemFlags CurrentWorkModel::flags(const QModelIndex &index) const
{
    auto flags = QAbstractTableModel::flags(index);

    if (index.column() == HN_PAUSE || index.column() == HN_NAME) {
        flags |= Qt::ItemIsEditable;
    }

    return flags;
}

QModelIndex CurrentWorkModel::addWork(Work::ptr_t work)
{
    auto cw = make_shared<CurrentWork>();
    cw->work = work;

    // Pause the active one
    suspendActive();

    // Insert at top
    beginInsertRows({}, 0, 0);
    work_.insert(work_.begin(), cw);
    endInsertRows();
    return createIndex(0, 0, cw.get());
}

void CurrentWorkModel::suspendActive()
{
    if (!work_.empty()) {
        if (work_[0]->current_state == CurrentWork::ACTIVE) {
            work_[0]->startPause();

            const auto ix = createIndex(0,0);
            emit dataChanged(ix, ix);
        }
    }
}

void CurrentWorkModel::updateTime()
{
    qDebug() << "Timer executing";

    for(int row = 0; row < static_cast<int>(work_.size()); ++row) {
        auto ix = index(row, HN_PAUSE);
        emit dataChanged(ix, ix);
        ix = index(row, HN_USED);
        emit dataChanged(ix, ix);
    }
}

QString CurrentWorkModel::toHourMin(const int duration) const
{
    qDebug() << "Duration is " << duration;

    auto minutes = duration / 60;
    auto hours = minutes / 60;
    minutes -= (hours * 60);

    QString val;
    return val.sprintf("%02d:%02d", hours, minutes);
}

int CurrentWorkModel::parseDuration(const QString &value) const
{
    int minutes = {}, hours = {};
    bool have_seen_column = false;

    for(const auto& ch : value) {
        if (ch >= '0' && ch <= '9') {
            minutes *= 10;
            minutes += (ch.toLatin1() - '0');
        } else if (ch == ':') {
            if (have_seen_column) {
                throw std::runtime_error("Invalid input");
            }
            hours = minutes * 60 * 60;
            minutes = 0;
        } else {
            throw std::runtime_error("Invalid input");
        }
    }

    return (minutes * 60) + hours;
}

bool CurrentWorkModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole) {
        bool result = false;
        if (auto cw = this->getCurrentWork(index)) {
            if (index.column() == HN_NAME) {
                cw->work->name = value.toString();
                result = true;
            } else if (index.column() == HN_PAUSE) {
                cw->setPaused(parseDuration(value.toString()));
                result = true;
            }
        }

        if (result) {
            emit dataChanged(index, index);
        }

        return result;
    }

    return true;
}

CurrentWork *CurrentWorkModel::getCurrentWork(const QModelIndex &index) const
{
    if (index.isValid()) try {
        return work_[static_cast<size_t>(index.row())].get();
    } catch(std::exception&) {
        ;
    }

    return {};
}

void CurrentWorkModel::suspend(const QModelIndex &ix)
{
    if (auto cw = getCurrentWork(ix)) {
        if (cw->current_state == CurrentWork::ACTIVE) {
            cw->startPause();
            const auto chix = index(ix.row(), HN_FROM, {});
            emit dataChanged(chix, chix, {});
        }
    }
}

void CurrentWorkModel::resume(const QModelIndex &ix)
{
    if (auto cw = getCurrentWork(ix)) {
        if (cw->current_state == CurrentWork::PAUSED) {
            cw->endPause();

            if (cw != work_[0].get()) {
                suspendActive();
                beginMoveRows({}, ix.row(), ix.row(), {}, 0);
                auto rowData = move(work_[ix.row()]);
                work_.erase(work_.begin() + ix.row());
                work_.insert(work_.begin(), move(rowData));
                endMoveRows();
            }
            const auto chix = index(0, HN_FROM, {});
            emit dataChanged(chix, chix);
        }
    }
}

void CurrentWorkModel::done(const QModelIndex &ix)
{

}
