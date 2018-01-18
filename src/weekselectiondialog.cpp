#include "weekselectiondialog.h"
#include "ui_weekselectiondialog.h"
#include <QPushButton>

WeekSelectionDialog::WeekSelectionDialog(QWidget *parent, const QDate& currentDate) :
    QDialog(parent),
    ui(new Ui::WeekSelectionDialog)
{
    ui->setupUi(this);
    ui->calendarWidget->setSelectedDate(currentDate);
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onOk()));
}

WeekSelectionDialog::~WeekSelectionDialog()
{
    delete ui;
}

void WeekSelectionDialog::onOk()
{
    emit selectedDateChanged(ui->calendarWidget->selectedDate());
    done(1);
}

