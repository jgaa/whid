#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMainWindow>
#include "database.h"
#include "nodemodel.h"


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

private:
    Ui::MainWindow *ui;
    std::unique_ptr<Database> db_ = nullptr;
    std::unique_ptr<NodeModel> nodeModel_;
};

#endif // MAINWINDOW_H
