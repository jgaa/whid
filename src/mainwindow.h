#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <set>

#include <QItemSelection>
#include <QLabel>
#include <QMainWindow>
#include <QTableView>

#include "database.h"
#include "nodemodel.h"
#include "currentworkmodel.h"
#include "summarymodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void initialize();
    ~MainWindow();

signals:
    void workDone(const QModelIndex& ix, bool askIfNoTimeUsed = true);
    void summaryNeedUpdate();


private slots:
    void nodeTreeContextMenu(const QPoint &point);
    void currentWorkListContextMenu(const QPoint &point);
    void workListContextMenu(const QPoint &point);
    void onTreeSelectionChanged(const QItemSelection& selected,
                                const QItemSelection & deselected);
    void onStartNewButtonClicked();
    void onDoneButtonClicked();
    void onSuspendButtonClicked();
    void onResumeButtonClicked();
    void nodeModelReset();
    void onSummarySelectionChanged(int index);
    void onSummaryOptionsClicked();
    void validateStartBtn();
    void validateResumeDoneSuspendBtn();
    void setTimeUsedToday(int seconds);
    void onPaused(bool paused);
    void onSettings();
    void onAbout();
    void setActionStatesForTree();
    void setActionStatesForWorkList();
    void setActionStatesForCurrentWorkList();

    void on_actionNodeTreeNew_Folder_triggered();
    void on_actionNodeTreeNew_Project_triggered();
    void on_actionNodeTreeNew_Task_triggered();
    void on_actionNodeTreeNew_Customer_triggered();
    void on_actionFilter_and_Group_on_Summary_triggered();
    void on_actionNodeTreeDelete_a_Node_triggered();
    void on_actionTreeViewEdit_a_node_triggered();
    void on_actionEdit_a_Current_Work_Item_triggered();
    void on_actionDelete_a_Current_Work_Item_triggered();
    void on_actionWorkList_Edit_Item_triggered();
    void on_actionWorkList_Delete_Item_triggered();
    void on_actionWorkList_Set_as_Free_triggered();
    void on_actionWorkList_Set_as_Approved_triggered();
    void on_actionWorkList_Set_as_Invoiced_triggered();
    void on_actionWorkList_Set_as_Done_triggered();
    void on_actionWorkList_Set_as_Held_Back_triggered();

private:
    template <typename T>
    QModelIndex addNode(const QModelIndex &index)
    {
        Node *parent = static_cast<Node *>(index.internalPointer());
        if (parent == nullptr) {
            parent = nodeModel_->getRootNode();
        }
        auto node = std::make_shared<T>(parent->shared_from_this());
        node->name = "New Item";
        return nodeModel_->addNode(index, std::move(node));
    }

    QModelIndex addFolder(const QModelIndex& index) {
        return addNode<Folder>(index);
    }

    QModelIndex addCustomer(const QModelIndex& index) {
        return addNode<Customer>(index);
    }

    QModelIndex addProject(const QModelIndex& index) {
        return addNode<Project>(index);
    }

    QModelIndex addTask(const QModelIndex& index) {
        return addNode<Task>(index);
    }

    void selectNode(const QModelIndex& index);
    void deleteFromWorkList(const QItemSelection& selection);
    void commitAllWork();

    // Filter out the summary column as it holds no data
    QModelIndexList getWorkListSelectionIndexes() const;

    // Returns the current selcted ode, only if there is wxactely one node actually selecetd.
    QModelIndex getCurrentSelectedNode(int *selectionCount = nullptr) const;

    Ui::MainWindow *ui;
    std::unique_ptr<Database> db_ = nullptr;
    std::unique_ptr<NodeModel> nodeModel_;
    std::unique_ptr<CurrentWorkModel> currentWorkModel_;
    std::unique_ptr<WorkModel> workModel_;
    std::unique_ptr<SummaryModel> summaryModel_;

    QLabel *statusLabel_ = {};
    QLineEdit *statusTimeUsedToday_ = {};

    QTableView *workList_ = {};

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H
