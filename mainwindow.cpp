#include <QDebug>
#include <QMessageBox>

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
    setWindowTitle("Whid");
    db_ = make_unique<Database>();
    nodeModel_ = make_unique<NodeModel>();
    currentWorkModel_ = make_unique<CurrentWorkModel>();
    currentWorkModel_->initialize();
    workModel_ = make_unique<WorkModel>(*nodeModel_);
    summaryModel_ = make_unique<SummaryModel>(*nodeModel_);
    ui->nodeTree->setModel(nodeModel_.get());
    ui->nodeTree->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->nodeTree->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->startNewButton->setIcon(QIcon(":/res/icons/start.svg"));
    ui->resumeButton->setIcon(QIcon(":/res/icons/resume.svg"));
    ui->doneButton->setIcon(QIcon(":/res/icons/done.svg"));
    ui->suspendButton->setIcon(QIcon(":/res/icons/pause.svg"));
    ui->currentWorkList->setModel(currentWorkModel_.get());
    ui->currentWorkList->horizontalHeader()->setStretchLastSection(true);
    for(int col = 0; col < 5l; ++col) {
        ui->currentWorkList->setColumnWidth(col, 60 + (col == 0 ? 16 : 0));
    }
    ui->currentWorkList->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->workList->setModel(workModel_.get());
    ui->workList->hideColumn(workModel_->fieldIndex("id"));
    ui->workList->hideColumn(workModel_->fieldIndex("note"));
    ui->workList->hideColumn(workModel_->fieldIndex("charge"));
    ui->workList->hideColumn(workModel_->fieldIndex("node"));
    ui->workList->horizontalHeader()->setStretchLastSection(true);

    ui->workList->setColumnWidth(workModel_->fieldIndex("status"), 250);
    ui->workList->setColumnWidth(workModel_->fieldIndex("start"), 130);
    ui->workList->setColumnWidth(workModel_->fieldIndex("end"), 60);

    ui->workList->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->summaryView->setModel(summaryModel_.get());
    ui->summaryView->setColumnWidth(0, 180);

    // Prevent resize of the main window to resize the node-tree horizontally.
    ui->topHorizontalSplitter->setStretchFactor(0,0);
    ui->topHorizontalSplitter->setStretchFactor(1,1);

    ui->bottomHorizontalSplitter->setStretchFactor(0,1);
    ui->bottomHorizontalSplitter->setStretchFactor(1,0);

    ui->splitterMain->setStretchFactor(0,1);
    ui->splitterMain->setStretchFactor(1,0);

    ui->startNewButton->setStyleSheet("text-align: left");
    ui->resumeButton->setStyleSheet("text-align: left");
    ui->doneButton->setStyleSheet("text-align: left");
    ui->suspendButton->setStyleSheet("text-align: left");

    connect(ui->nodeTree, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(nodeTreeContextMenu(const QPoint &)));
    connect(ui->currentWorkList, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(currentWorkListContextMenu(const QPoint &)));
    connect(ui->workList, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(workListContextMenu(const QPoint &)));
    connect(ui->nodeTree, SIGNAL(activated(const QModelIndex &)),
            this, SLOT(onTreeNodeActivated(const QModelIndex &)));
    connect(ui->startNewButton, SIGNAL(released()), this, SLOT(onStartNewButtonClicked()));
    connect(ui->suspendButton, SIGNAL(released()), this, SLOT(onSuspendButtonClicked()));
    connect(ui->resumeButton, SIGNAL(released()), this, SLOT(onResumeButtonClicked()));
    connect(ui->doneButton, SIGNAL(released()), this, SLOT(onDoneButtonClicked()));
    connect(ui->currentWorkList, SIGNAL(activated(const QModelIndex &)),
            this, SLOT(onCurrentWorkListActivated(const QModelIndex &)));
    connect(ui->nodeTree->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)),
            this, SLOT(onTreeSelectionChanged(const QItemSelection&, const QItemSelection&)));
    connect(ui->workList->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)),
            this, SLOT(onWorkListSelectionChanged(const QItemSelection&, const QItemSelection&)));
    connect(currentWorkModel_.get(), SIGNAL(workDone(Work::ptr_t)),
            workModel_.get(), SLOT(addWork(Work::ptr_t)));
    connect(nodeModel_.get(), SIGNAL(modelReset()), this, SLOT(nodeModelReset()));
    connect(workModel_.get(), SIGNAL(modelReset()), summaryModel_.get(), SLOT(dataChanged));
    connect(workModel_.get(), SIGNAL(
            dataChanged(const QModelIndex &, const QModelIndex &, const QVector<int> &())),
            summaryModel_.get(), SLOT(dataChanged));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::nodeTreeContextMenu(const QPoint &point)
{
    auto index = ui->nodeTree->indexAt(point);

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

    if (node->getType() != Node::Type::ROOT) {
        menu->addAction("Delete", [this, index] {
            nodeModel_->deleteNodes(ui->nodeTree->selectionModel()->selectedIndexes());
        });
        menu->addSeparator();
    }

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

void MainWindow::currentWorkListContextMenu(const QPoint &point)
{
    auto index = ui->currentWorkList->indexAt(point);
    if (!index.isValid()) {
        return;
    }

    if (/*auto cw = */currentWorkModel_->getCurrentWork(index)) {
        QMenu *menu = new QMenu;
        menu->addAction("Delete", [this, index] {
            currentWorkModel_->removeRows(index.row(), 1, {}); });

        menu->exec(ui->currentWorkList->mapToGlobal(point));
    }
}

void MainWindow::workListContextMenu(const QPoint &point)
{
    auto index = ui->workList->indexAt(point);
    if (!index.isValid()) {
        return;
    }

    QMenu *menu = new QMenu;
    menu->addAction("Delete", [this, index] {
        deleteFromWorkList(ui->workList->selectionModel()->selection());
    });

    menu->addSection("Status");
    menu->addAction("Set as Done", [this, index] {
        workModel_->setStatus(ui->workList->selectionModel()->selectedIndexes(),
                              Work::Status::DONE);
    });
    menu->addAction("Set as Free", [this, index] {
        workModel_->setStatus(ui->workList->selectionModel()->selectedIndexes(),
                              Work::Status::FREE);
    });
    menu->addAction("Set as Held Back", [this, index] {
        workModel_->setStatus(ui->workList->selectionModel()->selectedIndexes(),
                              Work::Status::HELD_BACK);
    });
    menu->addAction("Set as Approved", [this, index] {
        workModel_->setStatus(ui->workList->selectionModel()->selectedIndexes(),
                              Work::Status::APPROVED);
    });
    menu->addAction("Set as Invoiced", [this, index] {
        workModel_->setStatus(ui->workList->selectionModel()->selectedIndexes(),
                              Work::Status::INVOICED);
    });

    menu->exec(ui->workList->mapToGlobal(point));
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

void MainWindow::onTreeSelectionChanged(const QItemSelection &, const QItemSelection &)
{
    auto selection =  ui->nodeTree->selectionModel()->selectedIndexes();
    if (selection.isEmpty()) {
        workModel_->setFilter("id=-1");
    } else {
        workModel_->setFilter(nodeModel_->createFilter(
            selection, "node"));
    }
    workModel_->select();
}

void MainWindow::onWorkListSelectionChanged(const QItemSelection &,
                                            const QItemSelection &)
{

}

void MainWindow::onStartNewButtonClicked()
{
    auto work = make_shared<Work>();
    work->name = "Untitled";
    work->start = work->end = QDateTime::currentDateTime();

    auto currentNodeIx = ui->nodeTree->currentIndex();
    if (!currentNodeIx.isValid()) {
        qWarning() << "No active node";
        return;
    }
    work->node = static_cast<Node *>(currentNodeIx.internalPointer())->shared_from_this();

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

void MainWindow::nodeModelReset()
{
    ui->nodeTree->selectionModel()->clear();
    workModel_->setFilter("id=-1");
    workModel_->select();
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

void MainWindow::deleteFromWorkList(const QItemSelection& selection)
{
    vector<std::pair<int, int>> selections;

    int count = {};
    for(const auto& s: selection ) {
        selections.push_back({s.top(), (s.bottom() - s.top()) + 1});
        count += selections.back().second;
    }

    // Sort the sections highest first, so the remaining row values remain valid
    // during the delete iterations.
    sort(selections.begin(), selections.end(), [](const auto& left, const auto& right) {
        return left.first > right.first;
    });

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setText("You have selected " + QString::number(count) + " row(s)");
    msgBox.setInformativeText("Press OK to delete the selected row(s)?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    if(msgBox.exec() == QMessageBox::Cancel) {
        return;
    }

    workModel_->setEditStrategy(QSqlTableModel::OnManualSubmit);
    for(const auto& s: selections) {
        qDebug() << "Deleting rows: " << s.first << " " << s.second;
        workModel_->removeRows(s.first, s.second, {});
    }

    workModel_->submitAll();
    workModel_->select();
    workModel_->setEditStrategy(QSqlTableModel::OnFieldChange);
}


