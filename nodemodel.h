#ifndef NODEMODEL_H
#define NODEMODEL_H

#include <memory>
#include <QStandardItemModel>
#include <QDateTime>
#include <QMenu>

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

    virtual QVariant getIcon(QSize size) const = 0;

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

    // Returns true if type equals this node or any parent
    bool hasType(const Type type) const {
        if (getType() == type) {
            return true;
        }

        if (auto parent = parent_.lock()) {
            return parent->hasType(type);
        }

        return false;
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

    void addCustomer();

protected:
    QVariant getNodeIcon(QString name, QSize size) const;

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

    QVariant getIcon(QSize) const override {
        return {};
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

    QVariant getIcon(QSize size) const override {
        return getNodeIcon("folder.svg", size);
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

    QVariant getIcon(QSize size) const override {
        return getNodeIcon("customer.svg", size);
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

    QVariant getIcon(QSize size) const override {
        return getNodeIcon("project.svg", size);
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

    QVariant getIcon(QSize size) const override {
        return getNodeIcon("task.svg", size);
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

    QModelIndex addNode(const QModelIndex& parentIndex, std::shared_ptr<Node> node);

    Node *getRootNode() const {
        return root_.get();
    }

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    bool hasChildren(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    //bool insertRows(int row, int count, const QModelIndex &parent) override;
    //bool insertColumns(int column, int count, const QModelIndex &parent) override;
    //bool removeRows(int row, int count, const QModelIndex &parent) override;
    //bool removeColumns(int column, int count, const QModelIndex &parent) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
    void loadData();
    void fetchChildren(Node& node);
    void flushNode(Node& node);

    std::shared_ptr<Root> root_;
};

#endif // NODEMODEL_H
