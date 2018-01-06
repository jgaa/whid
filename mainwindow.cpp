#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void MainWindow::initialize()
{
    db_ = std::make_unique<Database>();
    nodeModel_ = std::make_unique<NodeModel>();
    ui->nodeTree->setModel(nodeModel_.get());
    ui->nodeTree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->nodeTree, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(customContextMenu(const QPoint &)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::customContextMenu(const QPoint &point)
{
    QModelIndex index = ui->nodeTree->indexAt(point);

    Node *node = nullptr;

    if (!index.isValid()) {
        index = QModelIndex{};
    } else {
        node = static_cast<Node *>(index.internalPointer());
    }

    if (!node) {
        node = nodeModel_->getRootNode();
    }

    QMenu *menu = new QMenu;

    const auto type = node->getType();

    if (type != Node::Type::TASK) {
        menu->addAction("Add Folder", [this, index] {
            selectNode(addFolder(index));
        });

        menu->addAction("Add Project", [this, index] {
            selectNode(addProject(index));
        });

        if (!node->hasType(Node::Type::CUSTOMER)) {
            menu->addAction("Add Customer", [this, index] {
                selectNode(addCustomer(index));
            });
        }

        if (type != Node::Type::ROOT) {
            menu->addAction("Add Task", [this, index] {
                selectNode(addTask(index));
            });
        }
    }

    menu->exec(ui->nodeTree->mapToGlobal(point));
}

void MainWindow::selectNode(const QModelIndex &index)
{
    if (index.isValid()) {
        ui->nodeTree->expand(index.parent());
        ui->nodeTree->selectionModel()->select(index,
            QItemSelectionModel::ClearAndSelect);
        ui->nodeTree->edit(index);
    }
}
