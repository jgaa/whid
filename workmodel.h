#ifndef WORKMODEL_H
#define WORKMODEL_H

#include <QAbstractTableModel>
#include <QDateTime>
#include <memory>

class Node;

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

    void setStatus(int newStatus);

    int id = {};
    QString name;
    QString descr;
    QDateTime start;
    QDateTime end;
    int paused = {}; // Seconds pause
    int time = {}; // Seconds consumed, including roundup and adjustments
    Status status = Status::OPEN;
    QString note;
    int charge = {};

    std::weak_ptr<Node> node;
};

class WorkModel : public QAbstractTableModel
{
public:
    WorkModel();
};

#endif // WORKMODEL_H
