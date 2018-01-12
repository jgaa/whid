#ifndef WORKMODEL_H
#define WORKMODEL_H

#include <QAbstractTableModel>
#include <QDateTime>
#include <QSqlTableModel>
#include <memory>

class Node;
class NodeModel;

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

public slots:
    void addWork(Work::ptr_t work);

    // QAbstractItemModel interface
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    std::shared_ptr<Node> getNode(const QModelIndex& id) const;

    std::vector<int> columnMapping_; // Own to data-model
    NodeModel& nodeModel_;
};

#endif // WORKMODEL_H
