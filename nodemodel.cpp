
#include <assert.h>
#include <stack>

#include <QDebug>
#include <QMessageBox>
#include "nodemodel.h"
#include "database.h"

using namespace std;

NodeModel::NodeModel()
    : root_{make_shared<Root>()}
{
    loadData();
}

QModelIndex NodeModel::addNode(const QModelIndex &parentIndex, std::shared_ptr<Node> node)
{
    // Insert to database
    try {
        flushNode(*node);
    } catch(const std::exception& ex) {
        qWarning() << "Failed to add node with name " << node->name
                   << ": " << ex.what();

        // TODO: Add error dialog
        return {};
    }

    // Update in-memory image of model
    int children = rowCount(parentIndex);
    beginInsertRows(parentIndex, children, children);
    auto parent = static_cast<Node *>(parentIndex.internalPointer());
    if (parent == nullptr) {
        parent = getRootNode();
    }
    parent->addChild(move(node));
    endInsertRows();

    return index(children, 0, parentIndex);
}

bool NodeModel::deleteNodes(const QModelIndexList& indexes)
{
    // Find the number of affected work-nodes
    auto filter = createFilter(indexes, "node");

    QSqlQuery query("SELECT COUNT(*) FROM work WHERE " + filter);
    if (!query.next()) {
        qWarning() << "Failed to query work-table: " << query.lastError().text();
        return false;
    }

    const auto work_rows = query.value(0).toInt();
    if (work_rows) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setText("If you delete the selection, you will also delete " + QString::number(work_rows) + " work item(s)");
        msgBox.setInformativeText("Press OK to delete the selected item(s)?");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        if(msgBox.exec() == QMessageBox::Cancel) {
            return false;
        }
    }

    beginResetModel();

    auto db = QSqlDatabase::database();
    db.transaction();
    query.exec("DELETE FROM work WHERE " + filter);
    filter = createFilter(indexes, "id");
    query.exec("DELETE FROM node WHERE " + filter);
    if (!db.commit()) {
        qWarning() << "Failed to query work-table: " << query.lastError().text();
        return false;
    }
    loadData();
    endResetModel();

    return true;
}

Node::ptr_t NodeModel::getNodeFromId(const int id)
{
    std::weak_ptr<Node> cached;

    auto node = cached.lock();
    if (node) {
        if (node->id == id) {
            return node;
        }
    }

    node = getNodeFromId(*root_, id);
    cached = node;
    return node;
}

Node::ptr_t NodeModel::getNodeFromId(Node &node, const int id)
{
    for(size_t i = 0; i < node.getNumChildren(); ++i) {
        auto n = node.getChild(i);
        if (n->id == id) {
            return n->shared_from_this();
        }

        if (auto child = getNodeFromId(*n, id)) {
            return child;
        }
    }

    return {};
}

QModelIndex NodeModel::index(int row, int column,
                             const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return {};
    }

    Node *parent_item = parent.isValid()
            ? static_cast<Node *>(parent.internalPointer())
            : root_.get();

    try {
        auto child = parent_item->getChild(static_cast<size_t>(row));
        assert(child != nullptr);
        return createIndex(row, column, child);
    } catch(const std::out_of_range&) {
        qWarning() << "Invalid child index " << row
                   << " for node " << parent_item->id;
    }

    return {};
}

QModelIndex NodeModel::parent(const QModelIndex &child) const
{
    if (!child.isValid()) {
        return {};
    }

    auto node = static_cast<Node *>(child.internalPointer());
    auto parent = node->getParent();
    if (parent == root_.get()) {
        return {};
    }

    return createIndex(parent->getRow(), 0, parent);
}

int NodeModel::rowCount(const QModelIndex &parent) const
{
    Node *parent_item = parent.isValid()
            ? static_cast<Node *>(parent.internalPointer())
            : root_.get();

    return static_cast<int>(parent_item->getNumChildren());
}

int NodeModel::columnCount(const QModelIndex&) const
{
    return 1;
}

bool NodeModel::hasChildren(const QModelIndex &parent) const
{
    return rowCount(parent) > 0;
}

QVariant NodeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return {};
    }

    const auto node = static_cast<const Node *>(index.internalPointer());

    switch(role) {
        case Qt::DisplayRole:
        case Qt::EditRole:
            return node->name;
        case Qt::DecorationRole:
            return node->getIcon({16,16});

    }

    return {};
}

bool NodeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    const auto valueStr = value.toString();
    if (valueStr.isEmpty()) {
        return false;
    }

    auto node = static_cast<Node *>(index.internalPointer());
    assert(node);
    assert(node->getType() != Node::Type::ROOT);
    node->name = valueStr;
    try {
        flushNode(*node);
    } catch(const std::exception& ex) {
        qWarning() << "Failed to reame node #" << node->id << " to "
                   << valueStr << ". Error: " << ex.what();
        return false;
    }

    // FIXME: If we emit, the application crash
    //emit dataChanged(index, index);
    return true;
}

Qt::ItemFlags NodeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

void NodeModel::getIdWithChildren(const QModelIndex &ix, std::set<int> &ids)
{
    if (ix.isValid()) {
        auto node = static_cast<Node *>(ix.internalPointer());
        getIdWithChildren(*node, ids);
    }
}

QString NodeModel::createFilter(const QModelIndexList &ixlist,
                                const QString& fieldName)
{
    // Get the id's of all nodes and children in the selection
    set<int> ids;
    for(auto n: ixlist) {
        getIdWithChildren(n, ids);
    }

    QString filter;
    for(const auto id : ids) {
        if (!filter.isEmpty()) {
            filter += " OR ";
        }
        filter += " " + fieldName + "=" + QString::number(id);
    }

    return filter;
}

void NodeModel::getIdWithChildren(const Node& node, std::set<int>& ids)
{
    ids.insert(node.id);
    for(size_t i = 0; i < node.getNumChildren(); ++i) {
        getIdWithChildren(*node.getChild(i), ids);
    }
}

void NodeModel::loadData()
{
    enum Fields {
        F_ID,
        F_NAME,
        F_TYPE,
        F_DESCR,
        F_ACTIVE,
        F_CHARGE,
        F_PARENT
    };

    root_->clearChildren();
    stack<Node::ptr_t> parents;
    parents.push(root_);
    QSqlQuery query;
    query.prepare("WITH RECURSIVE "
                  "childrens(id, name, type, descr, active, charge, parent, level) AS ("
                  "VALUES(0, 'root', 0, NULL, 1, NULL, NULL, 1) "
                  "UNION ALL "
                  "SELECT node.id, node.name, node.type, node.descr, node.active, node.charge, node.parent, childrens.level+1 "
                  "    FROM node JOIN childrens ON node.parent = childrens.id "
                  "    ORDER BY 8 DESC, 2 ASC "
                  ") "
                  "SELECT id, name, type, descr, active, charge, parent from childrens");

    if (!query.exec()) {
        qWarning() << "Failed to fetch from database: " << query.lastError();
        return;
    }

    while(query.next()) {
        const int nt = query.value(F_TYPE).toInt();
        const int parent_id = query.value(F_PARENT).toInt();

        while(parent_id != parents.top()->id) {
            parents.pop();

            // We should never move above root
            assert(!parents.empty());
        }

        Node::ptr_t node;

        switch(nt) {
            case static_cast<int>(Node::Type::FOLDER):
                node = make_shared<Folder>(parents.top());
            break;

            case static_cast<int>(Node::Type::CUSTOMER):
                node = make_shared<Customer>(parents.top());
            break;

            case static_cast<int>(Node::Type::PROJECT):
                node = make_shared<Project>(parents.top());
            break;

            case static_cast<int>(Node::Type::TASK):
                node = make_shared<Task>(parents.top());
            break;

            default:
                qWarning() << "Ignoring unknown node type " << nt << "from database";
                continue;
        }

        node->id = query.value(F_ID).toInt();
        node->name = query.value(F_NAME).toString();
        node->descr = query.value(F_DESCR).toString();
        node->active = query.value(F_ACTIVE).toBool();
        node->charge = query.value(F_CHARGE).toInt();

        parents.top()->addChild(node);
        parents.push(move(node));
    }
}

/*
void NodeModel::fetchChildren(Node &parent)
{
    if (parent.isFetched) {
        return;
    }

    QSqlQuery query;

    if (parent.id > 0) {
        query.prepare("SELECT id, name, type, descr, active, charge FROM node where parent=? ORDER BY name");
        query.addBindValue(parent.id);
    } else {
        query.prepare("SELECT id, name, type, descr, active, charge FROM node where parent IS NULL ORDER BY name");
    }
    if (!query.exec()) {
        qWarning() << "Failed to fetch from database: " << query.lastError();
    }

    while(query.next()) {
        const int nt = query.value(2).toInt();
        shared_ptr<Node> node;

        switch(nt) {
            case static_cast<int>(Node::Type::FOLDER):
                node = make_shared<Folder>(parent.shared_from_this());
            break;

            case static_cast<int>(Node::Type::CUSTOMER):
                node = make_shared<Customer>(parent.shared_from_this());
            break;

            case static_cast<int>(Node::Type::PROJECT):
                node = make_shared<Project>(parent.shared_from_this());
            break;

            case static_cast<int>(Node::Type::TASK):
                node = make_shared<Task>(parent.shared_from_this());
            break;

            default:
                qWarning() << "Ignoring unknown node type " << nt << "from database";
                continue;
        }

        node->id = query.value(0).toInt();
        node->name = query.value(1).toString();
        node->descr = query.value(3).toString();
        node->active = query.value(4).toBool();
        node->charge = query.value(5).toInt();

        parent.addChild(move(node));
    }

    parent.isFetched = true;
}
*/

void Node::addCustomer()
{
    auto node = std::make_shared<Customer>(shared_from_this());
    node->name = "New Customer";

    // Always add at end
    addChild(move(node));
}

QVariant Node::getNodeIcon(QString name, QSize size) const
{
    QString path(":/res/icons/");
    path += name;

    return QIcon(path).pixmap(size);
}

void NodeModel::flushNode(Node &node)
{
    QSqlQuery query;
    QString sql, fields;

    const bool do_update = node.id != 0;

    if (do_update) {
        sql = "UPDATE node SET "
              "name=:name, type=:type, descr=:descr, active=:active, charge=:charge, parent=:parent "
              "WHERE id = :id";
    } else {
        sql = "INSERT INTO node (name, type, descr, active, charge, parent) "
              "VALUES (:name, :type, :descr, :active, :charge, :parent)";
    }

    int parent_id = 0;
    auto parent = node.getParent();
    if (parent && (parent->getType() != Node::Type::ROOT)) {
        parent_id = parent->id;
    }

    query.prepare(sql);
    query.bindValue(":name", node.name);
    query.bindValue(":type", node.getTypeId());
    query.bindValue(":descr", node.descr);
    query.bindValue(":active", node.active);
    query.bindValue(":charge", node.charge > 0 ? QVariant{node.charge} : QVariant::Int);

    if (parent_id) {
        query.bindValue(":parent", parent_id);
    } else {
        query.bindValue(":parent", QVariant::Int);
    }

    if (do_update) {
        query.bindValue(":id", node.id);
    }

    if (!query.exec()) {
        qWarning() << "flushNode failed. error:  " << query.lastError();
        throw runtime_error("Failed to insert/update node");
    } else {
        if (!do_update) {
            node.id = query.lastInsertId().toInt();
        }
        qDebug() << "Flushed node #" << node.id << " " << node.name << " with parent " << parent_id;
    }
}

