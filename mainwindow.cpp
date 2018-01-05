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
}

MainWindow::~MainWindow()
{
    delete ui;
}
