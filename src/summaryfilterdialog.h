#ifndef SUMMARYFILTERDIALOG_H
#define SUMMARYFILTERDIALOG_H

#include <QDialog>

namespace Ui {
class SummaryFilterDialog;
}

class SummaryFilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SummaryFilterDialog(QWidget *parent = 0);
    ~SummaryFilterDialog();

private:
    Ui::SummaryFilterDialog *ui;

    // QDialog interface
public slots:
    void accept() override;
};

#endif // SUMMARYFILTERDIALOG_H
