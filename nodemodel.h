#ifndef NODEMODEL_H
#define NODEMODEL_H

#include <memory>
#include <QStandardItemModel>
#include <QDateTime>

class Node : public std::enable_shared_from_this<Node> {
public:
    enum class Type {
        ROOT,
        FOLDER,
        CUSTOMER,
        PROJECT,
        TASK
    };

    Node() {}

    Node(std::shared_ptr<Node>&& parent)
        : parent_(move(parent))
    {
    }

    virtual ~Node() = default;

    virtual Type getType() const = 0;

    int getTypeId() const {
        return static_cast<int>(getType());
    }

    size_t getNumChildren() const {
        return children_.size();
    }

    Node * getChild(size_t index) const {
        return children_.at(index).get();
    }

    Node * getParent() const {
        auto parent = parent_.lock();
        if (parent) {
            return parent.get();
        }
        return nullptr;
    }

    int getRow(const Node *node) const {
        for(size_t i = 0; i < children_.size(); ++i) {
            if (children_[i].get() == node) {
                return static_cast<int>(i);
            }
        }

        return -1;
    }

    int getRow() const {
        auto parent = parent_.lock();
        if (parent) {
            return parent->getRow(this);
        }

        return -1;
    }

    int id = {};
    QString name;
    QString descr;
    bool active = true;
    int charge = -1;

    bool isFetched = false;

    void addChild(std::shared_ptr<Node>&& node) {
        children_.push_back(move(node));
    }

    void clearChildren() {
        children_.clear();
    }

private:
    std::vector<std::shared_ptr<Node>> children_;
    std::weak_ptr<Node> parent_;
};

class Root : public Node {
public:
    Root() {}

    Type getType() const override {
        return Type::ROOT;
    }
};

class Folder : public Node {
public:
    Folder(std::shared_ptr<Node>&& parent)
        : Node(move(parent))
    {
    }

    Type getType() const override {
        return Type::FOLDER;
    }
};

class Customer : public Node {
public:
    Customer(std::shared_ptr<Node>&& parent)
        : Node(move(parent))
    {
    }

    Type getType() const override {
        return Type::CUSTOMER;
    }
};

class Project : public Node {
public:
    Project(std::shared_ptr<Node>&& parent)
        : Node(move(parent))
    {
    }

    Type getType() const override {
        return Type::PROJECT;
    }
};

class Task : public Node {
public:
    Task(std::shared_ptr<Node>&& parent)
        : Node(move(parent))
    {
    }

    Type getType() const override {
        return Type::TASK;
    }
};

class Work {
public:
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

    int id = {};
    QString name;
    QString descr;
    QDateTime start;
    QDateTime end;
    int paused;
    int time; // Time consumed, including roundup and adjustments
    Status status = Status::OPEN;
    QString note;
    int charge;

    std::weak_ptr<Node> node;
};

class NodeModel : public QStandardItemModel
{
public:
    NodeModel();

    std::shared_ptr<Root> root_;

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    bool hasChildren(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    //bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    //bool insertRows(int row, int count, const QModelIndex &parent) override;
    //bool insertColumns(int column, int count, const QModelIndex &parent) override;
    //bool removeRows(int row, int count, const QModelIndex &parent) override;
    //bool removeColumns(int column, int count, const QModelIndex &parent) override;

private:
    void loadData();
    void fetchChildren(Node& node);
};

#endif // NODEMODEL_H
