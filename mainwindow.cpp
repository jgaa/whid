#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

Q_DECLARE_METATYPE(Node::wptr_t)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void MainWindow::initialize()
{
    db_ = make_unique<Database>();
    nodeModel_ = make_unique<NodeModel>();
    currentWorkModel_ = make_unique<CurrentWorkModel>();
    currentWorkModel_->initialize();
    ui->nodeTree->setModel(nodeModel_.get());
    ui->nodeTree->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->startNewButton->setIcon(QIcon(":/res/icons/start.svg"));
    ui->resumeButton->setIcon(QIcon(":/res/icons/resume.svg"));
    ui->doneButton->setIcon(QIcon(":/res/icons/done.svg"));
    ui->suspendButton->setIcon(QIcon(":/res/icons/pause.svg"));
    ui->currentWorkList->setModel(currentWorkModel_.get());
    ui->currentWorkList->horizontalHeader()->setStretchLastSection(true);
    for(int col = 0; col < 5l; ++col) {
        ui->currentWorkList->setColumnWidth(col, 60 + (col == 0 ? 16 : 0));
    }

    connect(ui->nodeTree, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(customContextMenu(const QPoint &)));
    connect(ui->nodeTree, SIGNAL(activated(const QModelIndex &)),
            this, SLOT(onTreeNodeActivated(const QModelIndex &)));
    connect(ui->startNewButton, SIGNAL(released()), this, SLOT(onStartNewButtonClicked()));
    connect(ui->suspendButton, SIGNAL(released()), this, SLOT(onSuspendButtonClicked()));
    connect(ui->resumeButton, SIGNAL(released()), this, SLOT(onResumeButtonClicked()));
    connect(ui->doneButton, SIGNAL(released()), this, SLOT(onDoneButtonClicked()));
    connect(ui->currentWorkList, SIGNAL(activated(const QModelIndex &)),
            this, SLOT(onCurrentWorkListActivated(const QModelIndex &)));
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

void MainWindow::onTreeNodeActivated(const QModelIndex &index)
{
    if (index.isValid()) {
        Node *node = static_cast<Node *>(index.internalPointer());
        if (node->getType() == Node::Type::TASK) {

            // TODO: Handle stop / pause

            ui->startNewButton->setEnabled(true);
            ui->startNewButton->setProperty("selected",
                QVariant::fromValue(Node::wptr_t(node->shared_from_this())));
            return;
        }
    }

    ui->startNewButton->setEnabled(false);
    ui->startNewButton->setProperty("selected", {});
}

void MainWindow::onStartNewButtonClicked()
{
    // TODO: Handle done

    auto work = make_shared<Work>();
    work->name = "Untitled";
    work->start = work->end = QDateTime::currentDateTime();
    auto createdIx = currentWorkModel_->addWork(work);
    ui->currentWorkList->selectRow(createdIx.row());

    auto ix = currentWorkModel_->index(createdIx.row(), CurrentWorkModel::HN_NAME, {});
    ui->currentWorkList->edit(ix);
}

void MainWindow::onCurrentWorkListActivated(const QModelIndex &index)
{
    const auto cw = currentWorkModel_->getCurrentWork(index);

    if (cw) {
        ui->doneButton->setEnabled(true);

        if (cw->current_state == CurrentWork::ACTIVE) {
            ui->resumeButton->setEnabled(false);
            ui->suspendButton->setEnabled(true);
        } else {
            ui->resumeButton->setEnabled(true);
            ui->suspendButton->setEnabled(false);
        }
    } else {
        ui->resumeButton->setEnabled(false);
        ui->doneButton->setEnabled(false);
    }
}

void MainWindow::onDoneButtonClicked()
{
    currentWorkModel_->done(ui->currentWorkList->currentIndex());
}

void MainWindow::onSuspendButtonClicked()
{
    currentWorkModel_->suspend(ui->currentWorkList->currentIndex());
}

void MainWindow::onResumeButtonClicked()
{
    currentWorkModel_->resume(ui->currentWorkList->currentIndex());
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


