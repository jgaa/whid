#ifndef WEEKSELECTIONDIALOG_H
#define WEEKSELECTIONDIALOG_H

#include <QAbstractButton>
#include <QDialog>
#include <QDate>

namespace Ui {
class WeekSelectionDialog;
}

class WeekSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WeekSelectionDialog(QWidget *parent, const QDate& currentDate);
    ~WeekSelectionDialog();

public slots:
    void onOk();

signals:
    void selectedDateChanged(const QDate& date);

private:
    Ui::WeekSelectionDialog *ui;
};

#endif // WEEKSELECTIONDIALOG_H
