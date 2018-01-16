#ifndef WORKMODEL_H
#define WORKMODEL_H

#include <QAbstractTableModel>
#include <QDateTime>
#include <QSqlTableModel>
#include <memory>

class Node;
class NodeModel;
class WorkDialog;

class Work {
public:
    using ptr_t = std::shared_ptr<Work>;

    enum class Status {
        OPEN,
        DONE,
        INTERRUPTED, // App closed / crashed without updating the end time
        HELD_BACK, // Manual hold-back
        FREE,
        APPROVED,
        INVOICED
    };

    int getStatusId() const {
        return static_cast<int>(status);
    }

    int getUsed() const {
         return static_cast<int>((start.msecsTo(end) / 1000) - paused);
    }

    QVariant getStatusIcon() const;
    static QVariant getStatusIcon(int status);
    static QString getStatusText(int status);

    void setStatus(int newStatus);

    int id = {};
    QString name;
    QDateTime start;
    QDateTime end;
    int paused = {}; // Seconds pause
    int time = {}; // Seconds consumed, including roundup and adjustments
    Status status = Status::OPEN;
    QString note;
    int charge = {};

    std::weak_ptr<Node> node;
};

class WorkModel : public QSqlTableModel
{
    Q_OBJECT

public:

    WorkModel(NodeModel& nm);
    Work::ptr_t getWork(const QModelIndex& ix) const;

public slots:
    void addWork(Work::ptr_t work);
    void updateWork(const QModelIndex& ix, const Work::ptr_t& work);
    void recalculateWorkToday();

signals:
    void workedToday(int seconds);

public:
    void setStatus(QModelIndexList indexes, Work::Status status);

    // QAbstractItemModel interface
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    std::shared_ptr<Node> getNode(const QModelIndex& id) const;
    std::vector<int> columnMapping_; // Own to data-model
    NodeModel& nodeModel_;
    mutable bool allIsWritable = false;
};

#endif // WORKMODEL_H
