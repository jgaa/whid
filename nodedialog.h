#ifndef NODEDIALOG_H
#define NODEDIALOG_H

#include <QDialog>

#include "nodemodel.h"

namespace Ui {
class NodeDialog;
}

class NodeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NodeDialog(QWidget *parent, const QModelIndex& ix, Node::ptr_t node);
    ~NodeDialog();

signals:
    void dataChanged(const QModelIndex &, const QModelIndex &, const QVector<int> & = {});

private:
    Ui::NodeDialog *ui;
    Node::ptr_t node_;
    const QModelIndex ix_;

    // QDialog interface
public slots:
    void accept() override;
};

#endif // NODEDIALOG_H
