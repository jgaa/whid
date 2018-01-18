#include "nodedialog.h"
#include "ui_nodedialog.h"

NodeDialog::NodeDialog(QWidget *parent, const QModelIndex& ix, Node::ptr_t node) :
    QDialog(parent),
    ui(new Ui::NodeDialog),
    node_{node},
    ix_{ix}
{
    ui->setupUi(this);

    setWindowTitle("Edit " + node->getTypeName());
    ui->nameEdit->setText(node->name);
    ui->notesEdit->setPlainText(node->descr);
    ui->activeCheck->setCheckState(node->active ? Qt::Checked : Qt::Unchecked);
    ui->icon->setPixmap(node->getIcon({100, 100}));
}

NodeDialog::~NodeDialog()
{
    delete ui;
}

void NodeDialog::accept()
{
    node_->name = ui->nameEdit->text();
    node_->active = ui->activeCheck->checkState() == Qt::Checked;
    node_->descr = ui->notesEdit->toPlainText();

    emit dataChanged(ix_, ix_);

    QDialog::accept();
}
