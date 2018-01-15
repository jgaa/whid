#include "workdialog.h"
#include "ui_workdialog.h"
#include "utility.h"

WorkDialog::WorkDialog(QWidget *parent, const QModelIndex& ix,
                       const Work::ptr_t& work, bool isCurrent) :
    QDialog(parent),
    ui(new Ui::WorkDialog),
    ix_{ix},
    work_{work},
    isCurrent_{isCurrent}
{
    ui->setupUi(this);

    ui->nameEdit->setText(work->name);
    ui->fromDateTime->setDateTime(work->start);
    ui->toDateTime->setDateTime(work->end);
    ui->noteEdit->setPlainText(work->note);

    ui->openBtn->setProperty("v", static_cast<int>(Work::Status::OPEN));
    ui->doneBtn->setProperty("v", static_cast<int>(Work::Status::DONE));
    ui->interruptedBtn->setProperty("v", static_cast<int>(Work::Status::INTERRUPTED));
    ui->heldBackBtn->setProperty("v", static_cast<int>(Work::Status::HELD_BACK));
    ui->freeBtn->setProperty("v", static_cast<int>(Work::Status::FREE));
    ui->approvedBtn->setProperty("v", static_cast<int>(Work::Status::APPROVED));
    ui->invoicedBtn->setProperty("v", static_cast<int>(Work::Status::INVOICED));

    ui->openBtn->setEnabled(isCurrent_);
    ui->toDateTime->setEnabled(!isCurrent_);

    switch(work->status) {
        case Work::Status::OPEN:
            ui->openBtn->setChecked(true);
            break;
        case Work::Status::DONE:
            ui->doneBtn->setChecked(true);
            break;
        case Work::Status::INTERRUPTED:
            ui->interruptedBtn->setChecked(true);
            break;
        case Work::Status::HELD_BACK:
            ui->heldBackBtn->setChecked(true);
            break;
        case Work::Status::FREE:
            ui->freeBtn->setChecked(true);
            break;
        case Work::Status::APPROVED:
            ui->approvedBtn->setChecked(true);
            break;
        case Work::Status::INVOICED:
            ui->invoicedBtn->setChecked(true);
            break;
    }
    ui->pauseEdit->setText(toHourMin(work->paused));
}

WorkDialog::~WorkDialog()
{
    delete ui;
}

void WorkDialog::accept()
{
    work_->name = ui->nameEdit->text();
    work_->start = ui->fromDateTime->dateTime();
    work_->end = ui->toDateTime->dateTime();
    work_->note = ui->noteEdit->toPlainText();
    work_->setStatus(ui->statusGroup->checkedButton()->property("v").toInt());
    work_->paused = parseDuration(ui->pauseEdit->text());

    emit dataChanged(ix_, work_);

    QDialog::accept();
}
