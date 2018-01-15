#include "currentworkmodel.h"
#include "utility.h"
#include "nodemodel.h"

#include <QDebug>
#include <QIcon>
#include <QMessageBox>
#include <array>

using namespace std;


CurrentWorkModel::CurrentWorkModel()
{

}

void CurrentWorkModel::initialize()
{
    timer_ = make_unique<QTimer>();
    connect(timer_.get(), &QTimer::timeout, this, &CurrentWorkModel::updateTime);
    timer_->start(10000);
}

bool CurrentWorkModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count -1);

    while(count-- > 0) {
        work_.erase(work_.begin() + row);
    }

    endRemoveRows();
    emit dataChanged({}, {});
    return true;
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
            if (role == Qt::DisplayRole) {
                return  work_.at(static_cast<size_t>(index.row()))->work->start.toString("HH:mm");
            } else {
                return  work_.at(static_cast<size_t>(index.row()))->work->start;
            }
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
    } else if (role == Qt::ToolTipRole) {
        if (auto node = work_.at(static_cast<size_t>(index.row()))->work->node.lock()) {
            QString path;
            node->getPath(path);
            return path;
        }
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
        if (section == CurrentWorkModel::HN_PAUSE
                || section == CurrentWorkModel::HN_NAME
                || section == CurrentWorkModel::HN_FROM) {
            return editIcon.pixmap(12,12);
        }
    }

    return {};
}

Qt::ItemFlags CurrentWorkModel::flags(const QModelIndex &index) const
{
    auto flags = QAbstractTableModel::flags(index);

    if (index.column() == HN_PAUSE || index.column() == HN_NAME || index.column() == HN_FROM) {
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
    for(int row = 0; row < static_cast<int>(work_.size()); ++row) {
        auto ix = index(row, HN_PAUSE);
        emit dataChanged(ix, ix);
        ix = index(row, HN_USED);
        emit dataChanged(ix, ix);
    }
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
            } else if (index.column() == HN_FROM) {
                cw->setStartTime(value.toDateTime());
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
        return work_.at(static_cast<size_t>(index.row())).get();
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
                auto rowData = move(work_[static_cast<size_t>(ix.row())]);
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
    if (auto cw =  getCurrentWork(ix)) {
        bool do_save = true;

        if (cw->getWorkedDuration() < 60) {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Question);
            msgBox.setText("The work-item contains less than 1 minute work-time.");
            msgBox.setInformativeText("Do you really want to save your changes?");
            msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Save);
            switch(msgBox.exec()) {
                case QMessageBox::Discard:
                    do_save = false;
                    break;
                case QMessageBox::Cancel:
                    return;
            }
        }

        if (cw->current_state == CurrentWork::PAUSED) {
            cw->endPause();
        }
        auto work = move(cw->work);

        beginRemoveRows({}, ix.row(), ix.row());
        work_.erase(work_.begin() + ix.row());
        endRemoveRows();

        if (do_save) {
            work->end = QDateTime::currentDateTime();
            work->status = Work::Status::DONE;
            emit workDone(move(work));
        }

        if (!work_.empty()) {
            work_[0]->endPause();
            const auto chix = index(0, HN_FROM, {});
            emit dataChanged(chix, chix);
        } else {
            emit dataChanged({}, {});
        }
    }
}
