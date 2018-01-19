#ifndef NODEMODEL_H
#define NODEMODEL_H

#include <memory>
#include <set>
#include <array>
#include <QStandardItemModel>
#include <QDateTime>
#include <QMenu>

class Node : public std::enable_shared_from_this<Node> {
public:
    using ptr_t = std::shared_ptr<Node>;
    using wptr_t = std::weak_ptr<Node>;

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

    QString getTypeName() const {
        static const std::array<QString, 5> names = {{
            "Root", "Folder", "Customer", "Project", "Task"
        }};
        return names.at(static_cast<size_t>(getTypeId()));
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

    virtual QPixmap getIcon(QSize size) const = 0;

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

    QString getPath() const {
        QString path;
        getPath(path);
        return path;
    }

    void getPath(QString& path) const {
        if (auto parent = parent_.lock()) {
            parent->getPath(path);
            if (parent->getType() != Node::Type::ROOT) {
                path += "/";
            }
        }

        path += name;
    }

    QVariant getStatusIcon(QSize size);

    int id = {};
    QString name;
    QString descr;
    bool active = true;
    int charge = {};

    void addChild(std::shared_ptr<Node> node) {
        children_.push_back(move(node));
    }

    void clearChildren() {
        children_.clear();
    }

protected:
    QPixmap getNodeIcon(QString name, QSize size) const;

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

    QPixmap getIcon(QSize) const override {
        return {};
    }
};

class Folder : public Node {
public:
    Folder(std::shared_ptr<Node> parent)
        : Node(move(parent))
    {
    }

    Type getType() const override {
        return Type::FOLDER;
    }

    QPixmap getIcon(QSize size) const override {
        return getNodeIcon("folder.svg", size);
    }
};

class Customer : public Node {
public:
    Customer(std::shared_ptr<Node> parent)
        : Node(move(parent))
    {
    }

    Type getType() const override {
        return Type::CUSTOMER;
    }

    QPixmap getIcon(QSize size) const override {
        return getNodeIcon("customer.svg", size);
    }
};

class Project : public Node {
public:
    Project(std::shared_ptr<Node> parent)
        : Node(move(parent))
    {
    }

    Type getType() const override {
        return Type::PROJECT;
    }

    QPixmap getIcon(QSize size) const override {
        return getNodeIcon("project.svg", size);
    }
};

class Task : public Node {
public:
    Task(std::shared_ptr<Node> parent)
        : Node(move(parent))
    {
    }

    Type getType() const override {
        return Type::TASK;
    }

    QPixmap getIcon(QSize size) const override {
        return getNodeIcon("task.svg", size);
    }
};

class NodeModel : public QStandardItemModel
{
    Q_OBJECT

public:
    NodeModel();

    QModelIndex addNode(const QModelIndex& parentIndex, std::shared_ptr<Node> node);
    bool deleteNodes(const QModelIndexList& indexes);

    Node *getRootNode() const {
        return root_.get();
    }

    Node::ptr_t getNodeFromId(const int id);

public slots:
    // Notification that someone changed a node directly
    // We need to save it
    void onDataChanged(const QModelIndex &ix, const QModelIndex &, const QVector<int> &);

    // QAbstractItemModel interface
public:
    Node::ptr_t getNodeFromId(Node& node, const int id);
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    bool hasChildren(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    // Get the id fields for ix and it's childen
    void getIdWithChildren(const QModelIndex& ix, std::set<int>& ids);
    QString createFilter(const QModelIndexList& ixlist, const QString& fieldName);


private:
    void loadData();
    void flushNode(Node& node);
    void getIdWithChildren(const Node& node, std::set<int>& ids);

    std::shared_ptr<Root> root_;
};

#endif // NODEMODEL_H
