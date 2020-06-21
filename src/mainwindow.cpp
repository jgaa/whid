#include <QDebug>
#include <QLineEdit>
#include <QMessageBox>
#include <QSettings>
#include <QCloseEvent>
#include <QMutableListIterator>
#include <memory>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "weekselectiondialog.h"
#include "nodedialog.h"
#include "workdialog.h"
#include "utility.h"
#include "settingsdialog.h"
#include "aboutdialog.h"
#include "summaryfilterdialog.h"

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
    QIcon appicon(":res/icons/whid.svg");
    setWindowIcon(appicon);
    onPaused(false);

    QSettings settings;

    if (settings.value("restore-window-state", true).toBool()) {
        restoreGeometry(settings.value("windowGeometry").toByteArray());
        restoreState(settings.value("windowState").toByteArray());
    }

#ifndef QT_DEBUG
    ui->refreshSummaryButton->setVisible(false);
#endif

    db_ = make_unique<Database>();
    nodeModel_ = make_unique<NodeModel>();
    currentWorkModel_ = make_unique<CurrentWorkModel>();
    currentWorkModel_->initialize();
    workModel_ = make_unique<WorkModel>(*nodeModel_);
    reportModel_ = make_unique<ReportModel>();
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

    ui->reportList->setModel(reportModel_.get());
    ui->reportList->setColumnWidth(0, 200);
    ui->reportList->setColumnWidth(1, 80);
    ui->reportList->setColumnWidth(2, 80);
    ui->reportList->setColumnWidth(3, 80);

    ui->summaryView->setModel(summaryModel_.get());
    ui->summaryView->setColumnWidth(0, 180);
    ui->summaryView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

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

    ui->summarySelectionCombo->addItem("Current Week", static_cast<int>(SummaryModel::When::CURRENT));
    ui->summarySelectionCombo->addItem("Previous Week", static_cast<int>(SummaryModel::When::PREVIOUS));
    ui->summarySelectionCombo->addItem("Selected Week", static_cast<int>(SummaryModel::When::WEEK_NUMBER));
    ui->summarySelectionCombo->setCurrentIndex(0);
    ui->summarySelectionLabel->setText("");

    statusLabel_ = new QLabel(this);
    statusLabel_->setText("Worked Today: ");
    statusTimeUsedToday_ = new QLineEdit(this);
    statusTimeUsedToday_->setMaximumWidth(60);
    statusTimeUsedToday_->setMinimumWidth(60);
    statusTimeUsedToday_->setReadOnly(true);
    statusTimeUsedToday_->setFrame(false);
    ui->statusBar->addPermanentWidget(statusLabel_);
    ui->statusBar->addPermanentWidget(statusTimeUsedToday_, 0);

    connect(ui->nodeTree, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(nodeTreeContextMenu(const QPoint &)));
    connect(ui->currentWorkList, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(currentWorkListContextMenu(const QPoint &)));
    connect(ui->workList, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(workListContextMenu(const QPoint &)));
    connect(ui->workList->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)),
            this, SLOT(setActionStatesForWorkList()));
    connect(ui->nodeTree, SIGNAL(activated(const QModelIndex &)), this, SLOT(validateStartBtn()));
    connect(ui->startNewButton, SIGNAL(released()), this, SLOT(onStartNewButtonClicked()));
    connect(ui->suspendButton, SIGNAL(released()), this, SLOT(onSuspendButtonClicked()));
    connect(ui->resumeButton, SIGNAL(released()), this, SLOT(onResumeButtonClicked()));
    connect(ui->doneButton, SIGNAL(released()), this, SLOT(onDoneButtonClicked()));
    connect(ui->currentWorkList, SIGNAL(activated(const QModelIndex &)),
            this, SLOT(validateResumeDoneSuspendBtn()));
    connect(ui->currentWorkList->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)),
            this, SLOT(validateResumeDoneSuspendBtn()));
    connect(currentWorkModel_.get(), SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &, const QVector<int> &)),
            this, SLOT(validateStartBtn()));
    connect(currentWorkModel_.get(), SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &, const QVector<int> &)),
            this, SLOT(validateResumeDoneSuspendBtn()));
    connect(currentWorkModel_.get(), SIGNAL(rowsRemoved(const QModelIndex&, int, int)), this, SLOT(validateStartBtn()));
    connect(currentWorkModel_.get(), SIGNAL(rowsRemoved(const QModelIndex&, int, int)), this, SLOT(validateResumeDoneSuspendBtn()));
    connect(currentWorkModel_.get(), SIGNAL(modelReset()), this, SLOT(validateStartBtn()));
    connect(currentWorkModel_.get(), SIGNAL(modelReset()), this, SLOT(validateResumeDoneSuspendBtn()));
    connect(ui->nodeTree->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)),
            this, SLOT(onTreeSelectionChanged(const QItemSelection&, const QItemSelection&)));
    connect(currentWorkModel_.get(), SIGNAL(workDone(Work::ptr_t)),
            workModel_.get(), SLOT(addWork(Work::ptr_t)));
    connect(nodeModel_.get(), SIGNAL(modelReset()), this, SLOT(nodeModelReset()));
    connect(workModel_.get(), SIGNAL(modelReset()), summaryModel_.get(), SLOT(dataChanged()));
    connect(workModel_.get(), SIGNAL(
            dataChanged(const QModelIndex &, const QModelIndex &, const QVector<int> &)),
            summaryModel_.get(), SLOT(dataChanged()));
    connect(ui->summarySelectionCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onSummarySelectionChanged(int)));
    connect(summaryModel_.get(), SIGNAL(selectionTextChanged(const QString &)), ui->summarySelectionLabel, SLOT(setText(const QString &)));
    connect(ui->summarySelectionOptionsButton, SIGNAL(clicked(bool)), this, SLOT(onSummaryOptionsClicked()));
    connect(workModel_.get(), SIGNAL(workedToday(int)), this, SLOT(setTimeUsedToday(int)));
    connect(currentWorkModel_.get(), SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &, const QVector<int> &)),
            workModel_.get(), SLOT(recalculateWorkToday()));
    connect(ui->refreshSummaryButton, SIGNAL(clicked()), summaryModel_.get(), SLOT(dataChanged()));
    connect(currentWorkModel_.get(), SIGNAL(paused(bool)), this, SLOT(onPaused(bool)));
    connect(ui->action_Settings, SIGNAL(triggered()), this, SLOT(onSettings()));
    connect(this, SIGNAL(workDone(const QModelIndex&, bool)), currentWorkModel_.get(), SLOT(done(const QModelIndex&, bool)));
    connect(ui->action_About, SIGNAL(triggered()), this, SLOT(onAbout()));
    connect(this, SIGNAL(summaryNeedUpdate()), summaryModel_.get(), SLOT(dataChanged()));

    workModel_->recalculateWorkToday();
    setActionStatesForTree();
    setActionStatesForWorkList();
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

    QMenu *menu = new QMenu(this);


    menu->addAction(ui->actionNodeTreeNew_Folder);
    menu->addAction(ui->actionNodeTreeNew_Customer);
    menu->addAction(ui->actionNodeTreeNew_Project);
    menu->addAction(ui->actionNodeTreeNew_Task);
    menu->addSeparator();
    menu->addAction(ui->actionTreeViewEdit_a_node);
    menu->addAction(ui->actionNodeTreeDelete_a_Node);


    menu->exec(ui->nodeTree->mapToGlobal(point));
}

void MainWindow::currentWorkListContextMenu(const QPoint &point)
{
    QMenu *menu = new QMenu;
    menu->addAction(ui->actionEdit_a_Current_Work_Item);
    menu->addAction(ui->actionDelete_a_Current_Work_Item);
    menu->exec(ui->currentWorkList->mapToGlobal(point));
}

void MainWindow::workListContextMenu(const QPoint &point)
{
    auto index = ui->workList->indexAt(point);
    if (!index.isValid()) {
        return;
    }

    QMenu *menu = new QMenu;

    menu->addAction(ui->actionWorkList_Edit_Item);
    menu->addAction(ui->actionWorkList_Delete_Item);
    menu->addSeparator();
    menu->addAction(ui->actionWorkList_Set_as_Done);
    menu->addAction(ui->actionWorkList_Set_as_Free);
    menu->addAction(ui->actionWorkList_Set_as_Held_Back);
    menu->addAction(ui->actionWorkList_Set_as_Approved);
    menu->addAction(ui->actionWorkList_Set_as_Invoiced);

    menu->exec(ui->workList->mapToGlobal(point));
}

void MainWindow::onTreeSelectionChanged(const QItemSelection &, const QItemSelection &)
{
    auto selection =  ui->nodeTree->selectionModel()->selectedIndexes();
    if (selection.isEmpty()) {
        workModel_->setFilter("id=-1");
        reportModel_->setFilter("id -1");
    } else {
        const auto filter = nodeModel_->createFilter(selection, "node");
        workModel_->setFilter(filter);
        reportModel_->setFilter(filter);
    }
    workModel_->select();
    validateStartBtn();
    setActionStatesForTree();
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
    validateResumeDoneSuspendBtn();
}

void MainWindow::onDoneButtonClicked()
{
    emit workDone(ui->currentWorkList->currentIndex());
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
    setActionStatesForTree();
}

void MainWindow::onSummarySelectionChanged(int index)
{
    array<SummaryModel::When, 3> when = {{
        SummaryModel::When::CURRENT,
        SummaryModel::When::PREVIOUS,
        SummaryModel::When::WEEK_NUMBER
    }};

    const auto selected_when = when.at(static_cast<size_t>(index));

    summaryModel_->whenChanged(selected_when);

    switch(selected_when) {
        case SummaryModel::When::CURRENT:
        case SummaryModel::When::PREVIOUS:
            ui->summarySelectionOptionsButton->setEnabled(false);
            break;
        case SummaryModel::When::WEEK_NUMBER:
            ui->summarySelectionOptionsButton->setEnabled(true);
            break;
    }
}

void MainWindow::onSummaryOptionsClicked()
{
    auto dlg = new WeekSelectionDialog(this, summaryModel_->getWeekSelection());
    connect(dlg, SIGNAL(selectedDateChanged(const QDate&)), summaryModel_.get(), SLOT(setSelectedWeek(const QDate&)));
    dlg->setAttribute( Qt::WA_DeleteOnClose );
    dlg->exec();
}

void MainWindow::selectNode(const QModelIndex &index)
{
    if (index.isValid()) {
        ui->nodeTree->expand(index.parent());
        ui->nodeTree->selectionModel()->select(index,
            QItemSelectionModel::ClearAndSelect);
        ui->nodeTree->setCurrentIndex(index);
        ui->nodeTree->scrollTo(index);
        ui->nodeTree->edit(index);
        validateStartBtn();
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

void MainWindow::commitAllWork()
{
    int items = currentWorkModel_->rowCount({});

    while(--items >= 0) {
        emit workDone(currentWorkModel_->index(items, 0, {}), false);
    }
}

QModelIndex MainWindow::getCurrentSelectedNode(int *selectionCount) const
{
    auto selection = ui->nodeTree->selectionModel()->selectedIndexes();

    if (selectionCount) {
        *selectionCount = selection.size();
    }

    if (selection.size() == 1) {
        return selection.first();
    }

    return {};
}

void MainWindow::validateStartBtn()
{
    if (ui->nodeTree->selectionModel()->selectedIndexes().size() == 1) {
        const auto selected = ui->nodeTree->selectionModel()->currentIndex();
        if (selected.isValid()) {
            Node *node = static_cast<Node *>(selected.internalPointer());
            if (node->getType() == Node::Type::TASK) {
                ui->startNewButton->setEnabled(true);
                return;
            }
        }
    }

    ui->startNewButton->setEnabled(false);
}

void MainWindow::validateResumeDoneSuspendBtn()
{
    qDebug() << "validateResumeDoneSuspendBtn";
    const auto cw = currentWorkModel_->getCurrentWork(ui->currentWorkList->selectionModel()->currentIndex());

    if (cw && ui->currentWorkList->selectionModel()->selectedIndexes().size()) {
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
        ui->suspendButton->setEnabled(false);
    }

    setActionStatesForWorkList();
}

void MainWindow::setTimeUsedToday(int seconds)
{
    qDebug() << "Updated used today";
    seconds += currentWorkModel_->getUsed();
    statusTimeUsedToday_->setText(toHourMin(seconds));
}

void MainWindow::onPaused(bool paused)
{
    static const QString pausedStr{" PAUSED"};

    setWindowTitle(QCoreApplication::applicationName() + (paused ? pausedStr : QString{}));
}

void MainWindow::onSettings()
{
    auto dlg = new SettingsDialog(this);
    dlg->setAttribute( Qt::WA_DeleteOnClose );
    dlg->exec();
}

void MainWindow::onAbout()
{
    auto dlg = new AboutDialog(this);
    dlg->setAttribute( Qt::WA_DeleteOnClose );
    dlg->exec();

}

void MainWindow::setActionStatesForTree()
{
    auto selection =  ui->nodeTree->selectionModel()->selectedIndexes();
    const Node *node = nullptr;
    bool enable_edit = false;
    if (selection.size() == 1) {
        node = static_cast<const Node *>(selection.first().internalPointer());
        enable_edit = true;
    }

    bool enable_task = (node && node->getType() != Node::Type::TASK);
    bool enable_fcp = (selection.size() <= 1)
        && (!node || node->getType() != Node::Type::TASK);

    ui->actionNodeTreeNew_Folder->setEnabled(enable_fcp);
    ui->actionNodeTreeNew_Customer->setEnabled(enable_fcp);
    ui->actionNodeTreeNew_Project->setEnabled(enable_fcp);
    ui->actionNodeTreeNew_Task->setEnabled(enable_task);
    ui->actionTreeViewEdit_a_node->setEnabled(enable_edit);
    ui->actionNodeTreeDelete_a_Node->setEnabled(selection.size() >= 1);

    // disable
}

QModelIndexList MainWindow::getWorkListSelectionIndexes() const
{
    auto indexes = ui->workList->selectionModel()->selectedIndexes();
    const auto sum_row = workModel_->rowCount({}) -1;

    QMutableListIterator<QModelIndex> i(indexes);
    while (i.hasNext()) {
        if (i.next().row() == sum_row) {
            i.remove();
        }
    }

    return indexes;
}

void MainWindow::setActionStatesForWorkList()
{
    const auto indexes = getWorkListSelectionIndexes();
    const bool have_selected = !indexes.isEmpty();
    const bool is_onwe_row = isOneRow(indexes);

    ui->actionWorkList_Delete_Item->setEnabled(is_onwe_row);
    ui->actionWorkList_Set_as_Done->setEnabled(have_selected);
    ui->actionWorkList_Set_as_Free->setEnabled(have_selected);
    ui->actionWorkList_Set_as_Held_Back->setEnabled(have_selected);
    ui->actionWorkList_Set_as_Approved->setEnabled(have_selected);
    ui->actionWorkList_Set_as_Invoiced->setEnabled(have_selected);
    ui->actionWorkList_Edit_Item->setEnabled(is_onwe_row);
}

void MainWindow::setActionStatesForCurrentWorkList()
{

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings;
    if (currentWorkModel_->rowCount({}) > 0) {
        const auto quit_strategy = settings.value("quit-strategy", "commit").toString();

        if (quit_strategy == "quit") {
            qWarning() << "Quitting withiout saving active work items";
        } else if (quit_strategy == "ask") {
            switch(QMessageBox::question(this,
                "Unsaved work",
                "You have active work-item(s), please choose what to do",
                QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel)) {

                case QMessageBox::Save:
                    commitAllWork();
                    break;
                case QMessageBox::Cancel:
                    event->ignore();
                    return;
                default:
                    qWarning() << "Quitting withiout saving active work items";
            }

        } else {
            commitAllWork();
        }
    }

    settings.setValue("windowGeometry", saveGeometry());
    settings.setValue("windowState", saveState());
}


void MainWindow::on_actionNodeTreeNew_Folder_triggered()
{
    int count = {};
    auto ix = getCurrentSelectedNode(&count);
    if (count <= 1) {
        selectNode(addFolder(ix));
    }
}

void MainWindow::on_actionNodeTreeNew_Project_triggered()
{
    int count = {};
    auto ix = getCurrentSelectedNode(&count);
    if (count <= 1) {
        selectNode(addProject(ix));
    }
}

void MainWindow::on_actionNodeTreeNew_Task_triggered()
{
    auto ix = getCurrentSelectedNode();
    if (ix.isValid()) {
        selectNode(addTask(ix));
    }
}

void MainWindow::on_actionNodeTreeNew_Customer_triggered()
{
    int count = {};
    auto ix = getCurrentSelectedNode(&count);
    if (count <= 1) {
        selectNode(addCustomer(ix));
    }
}

void MainWindow::on_actionFilter_and_Group_on_Summary_triggered()
{
    auto dlg = new SummaryFilterDialog(this);
    dlg->setAttribute( Qt::WA_DeleteOnClose );
    dlg->exec();
    emit summaryNeedUpdate();
}

void MainWindow::on_actionNodeTreeDelete_a_Node_triggered()
{
    int count = {};
    getCurrentSelectedNode(&count);
    if (count > 0) {
        nodeModel_->deleteNodes(ui->nodeTree->selectionModel()->selectedIndexes());
    }
}

void MainWindow::on_actionTreeViewEdit_a_node_triggered()
{
    auto ix = getCurrentSelectedNode();
    if (ix.isValid()) {
        if (auto node = static_cast<Node *>(ix.internalPointer())) {
            auto dlg = new NodeDialog(this, ix, node->shared_from_this());
            connect(dlg, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &, const QVector<int> &)),
                    nodeModel_.get(), SLOT(dataChanged(const QModelIndex &, const QModelIndex &, const QVector<int> &)));
            dlg->setAttribute( Qt::WA_DeleteOnClose );
            dlg->exec();
        }
    }
}

void MainWindow::on_actionEdit_a_Current_Work_Item_triggered()
{
    auto ix = ui->currentWorkList->currentIndex();
    if (!ix.isValid()) {
        return;
    }

    if (auto cw = currentWorkModel_->getCurrentWork(ix)) {
        if (isOneRow(ui->currentWorkList->selectionModel()->selectedIndexes())) {
            auto dlg = new WorkDialog(this, ix, cw->work, true);
            dlg->setAttribute( Qt::WA_DeleteOnClose );
            dlg->exec();
        }
    }
}

void MainWindow::on_actionDelete_a_Current_Work_Item_triggered()
{
    auto ix = ui->currentWorkList->currentIndex();
    if (ix.isValid()) {
         currentWorkModel_->removeRows(ix.row(), 1, {});
    }
}

void MainWindow::on_actionWorkList_Edit_Item_triggered()
{
    const auto indexes = getWorkListSelectionIndexes();
    if (isOneRow(indexes)) {
        const auto ix = indexes.first();
        if (auto work = workModel_->getWork(ix)) {
            auto dlg = new WorkDialog(this, ix, work);
            connect(dlg, SIGNAL(dataChanged(const QModelIndex&, const Work::ptr_t&)),
                    workModel_.get(), SLOT(updateWork(const QModelIndex&, const Work::ptr_t&)));
            dlg->setAttribute( Qt::WA_DeleteOnClose );
            dlg->exec();
        }
    }
}

void MainWindow::on_actionWorkList_Delete_Item_triggered()
{
    deleteFromWorkList(ui->workList->selectionModel()->selection());
}

void MainWindow::on_actionWorkList_Set_as_Free_triggered()
{
    workModel_->setStatus(getWorkListSelectionIndexes(),
                          Work::Status::FREE);
}

void MainWindow::on_actionWorkList_Set_as_Approved_triggered()
{
    workModel_->setStatus(getWorkListSelectionIndexes(),
                          Work::Status::APPROVED);
}

void MainWindow::on_actionWorkList_Set_as_Invoiced_triggered()
{
    workModel_->setStatus(getWorkListSelectionIndexes(),
                          Work::Status::INVOICED);
}

void MainWindow::on_actionWorkList_Set_as_Done_triggered()
{
    workModel_->setStatus(getWorkListSelectionIndexes(),
                          Work::Status::DONE);
}

void MainWindow::on_actionWorkList_Set_as_Held_Back_triggered()
{
    workModel_->setStatus(getWorkListSelectionIndexes(),
                          Work::Status::HELD_BACK);
}

void MainWindow::on_actionscreen_size_1024x768_triggered()
{
    setFixedSize(1024, 768);
}

void MainWindow::on_actionScreen_Size_1280x720_triggered()
{
    setFixedSize(1280, 720);
}

void MainWindow::on_actionScreen_Size_1920x1080_triggered()
{
    setFixedSize(1920, 1080);
}
