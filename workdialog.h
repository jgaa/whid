#ifndef WORKDIALOG_H
#define WORKDIALOG_H

#include <QDialog>
#include <QModelIndexList>

#include "workmodel.h"

namespace Ui {
class WorkDialog;
}

class WorkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WorkDialog(QWidget *parent, const QModelIndex& ix,
                        const Work::ptr_t& work,
                        bool isCurrent = false);
    ~WorkDialog();

signals:
    void dataChanged(const QModelIndex& ix, const Work::ptr_t& work);

public slots:
    void accept() override;

private slots:
    void updateUsed();

private:
    void flushToWork();

    Ui::WorkDialog *ui;
    QModelIndex ix_;
    const Work::ptr_t& work_;
    const bool isCurrent_;
    Work::ptr_t ownWork_;
};

#endif // WORKDIALOG_H
