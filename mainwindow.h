#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMainWindow>
#include "database.h"
#include "nodemodel.h"
#include "currentworkmodel.h"

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

private slots:
    void customContextMenu(const QPoint &point);
    void onTreeNodeActivated(const QModelIndex &index);
    void onStartNewButtonClicked();
    void onCurrentWorkListActivated(const QModelIndex &index);
    void onDoneButtonClicked();
    void onSuspendButtonClicked();
    void onResumeButtonClicked();

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

    Ui::MainWindow *ui;
    std::unique_ptr<Database> db_ = nullptr;
    std::unique_ptr<NodeModel> nodeModel_;
    std::unique_ptr<CurrentWorkModel> currentWorkModel_;
};

#endif // MAINWINDOW_H
