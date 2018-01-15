#ifndef CURRENTWORKMODEL_H
#define CURRENTWORKMODEL_H

#include <assert.h>

#include <QAbstractTableModel>
#include <QTimer>

#include "workmodel.h"


class CurrentWork {
public:
    enum CurrentState {
        PAUSED,
        ACTIVE
    };

    CurrentState current_state = CurrentState::ACTIVE;
    QDateTime paused;
    std::shared_ptr<Work> work;

    int getPaused() const {
        const int this_segment = static_cast<int>(
                    paused.msecsTo(QDateTime::currentDateTime()) / 1000);
        return this_segment + work->paused;
    }

    void setPaused(int seconds) {
        work->paused = seconds;
        if (current_state == PAUSED) {
            paused = QDateTime::currentDateTime();
        } else {
            paused = {};
        }
    }

    void startPause() {
        paused = QDateTime::currentDateTime();
        current_state = PAUSED;
    }

    void endPause() {
        assert(current_state == PAUSED);
        work->paused = getPaused();
        paused = {};
        current_state = ACTIVE;
    }

    int getWorkedDuration() const {
        int duration = work->start.msecsTo(QDateTime::currentDateTime()) / 1000;
        duration -= getPaused();
        return duration;
    }

    void setStartTime(const QDateTime& when) {
        work->start = when;
    }
};


class CurrentWorkModel : public QAbstractTableModel
{
public:
    Q_OBJECT

public:
    enum Headers {
        HN_FROM,
        HN_TO,
        HN_PAUSE,
        HN_USED,
        HN_NAME
    };

    CurrentWorkModel();
    void initialize();


    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex&) const override {
        return static_cast<int>(work_.size());
    }

    int columnCount(const QModelIndex&) const override {
        return 5;
    }

    bool removeRows(int row, int count, const QModelIndex &parent) override;

    QVariant data(const QModelIndex&index, int role) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    CurrentWork *getCurrentWork(const QModelIndex &index) const;

public slots:
    // Suspend the active item and add the new one at the top of the list
    QModelIndex addWork(Work::ptr_t work);
    void updateWork(const QModelIndex& ix, const Work::ptr_t& work);
    void suspend(const QModelIndex &index);
    void resume(const QModelIndex &index);
    void done(const QModelIndex &ix);

    void updateTime();

signals:
    void workDone(Work::ptr_t);

private:
    void suspendActive();

    std::vector<std::shared_ptr<CurrentWork>> work_;    
    std::unique_ptr<QTimer> timer_;
};

#endif // CURRENTWORKMODEL_H
