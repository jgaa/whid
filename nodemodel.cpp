
#include <assert.h>
#include <QDebug>
#include "nodemodel.h"
#include "database.h"

using namespace std;

NodeModel::NodeModel()
    : root_{make_shared<Root>()}
{
    loadData();
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

    const_cast<NodeModel *>(this)->fetchChildren(*parent_item);
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

    if (role != Qt::DisplayRole) {
        return {};
    }

    const auto node = static_cast<const Node *>(index.internalPointer());

    return node->name;
}

void NodeModel::loadData()
{
    root_->clearChildren();
    fetchChildren(*root_);
}

void NodeModel::fetchChildren(Node &parent)
{
    if (parent.isFetched) {
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT id, name, type, descr, active, charge FROM node where parent=? ORDER BY name");
    query.addBindValue(parent.id);
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

