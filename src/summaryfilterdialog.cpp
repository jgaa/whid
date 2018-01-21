#include "src/summaryfilterdialog.h"
#include "ui_summaryfilterdialog.h"

#include <QSettings>

SummaryFilterDialog::SummaryFilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SummaryFilterDialog)
{
    ui->setupUi(this);

    QSettings settings;
    ui->hideTasksBtn->setCheckState(
                settings.value("summary-hide-tasks", true).toBool()
                ? Qt::Checked : Qt::Unchecked);
}

SummaryFilterDialog::~SummaryFilterDialog()
{
    delete ui;
}


void SummaryFilterDialog::accept()
{
    QSettings settings;
    settings.setValue("summary-hide-tasks",
                      ui->hideTasksBtn->checkState() == Qt::Checked);

    QDialog::accept();
}
