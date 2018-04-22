#include "workdialog.h"
#include "ui_workdialog.h"
#include "utility.h"

#include <QMessageBox>
#include <QSound>

using namespace std;

WorkDialog::WorkDialog(QWidget *parent, const QModelIndex& ix,
                       const Work::ptr_t& work, bool isCurrent) :
    QDialog(parent),
    ui(new Ui::WorkDialog),
    ix_{ix},
    work_{work},
    isCurrent_{isCurrent},
    ownWork_{make_shared<Work>(*work)}
{
    ui->setupUi(this);

    setWindowTitle("Edit Work");

    ui->nameEdit->setText(ownWork_->name);
    ui->fromDateTime->setDateTime(ownWork_->start);
    ui->toDateTime->setDateTime(ownWork_->end);
    ui->noteEdit->setPlainText(ownWork_->note);

    ui->openBtn->setProperty("v", static_cast<int>(Work::Status::OPEN));
    ui->doneBtn->setProperty("v", static_cast<int>(Work::Status::DONE));
    ui->interruptedBtn->setProperty("v", static_cast<int>(Work::Status::INTERRUPTED));
    ui->heldBackBtn->setProperty("v", static_cast<int>(Work::Status::HELD_BACK));
    ui->freeBtn->setProperty("v", static_cast<int>(Work::Status::FREE));
    ui->approvedBtn->setProperty("v", static_cast<int>(Work::Status::APPROVED));
    ui->invoicedBtn->setProperty("v", static_cast<int>(Work::Status::INVOICED));

    ui->openBtn->setEnabled(isCurrent_);
    ui->toDateTime->setEnabled(!isCurrent_);

    switch(ownWork_->status) {
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
    ui->pauseEdit->setText(toHourMin(ownWork_->paused));
    ui->usedEdit->setText(toHourMin(ownWork_->getUsed()));

    connect(ui->pauseEdit, SIGNAL(textEdited(const QString &)), this, SLOT(updateUsed()));
    connect(ui->fromDateTime, SIGNAL(dateTimeChanged(const QDateTime &)), this, SLOT(updateUsed()));
    connect(ui->toDateTime, SIGNAL(dateTimeChanged(const QDateTime &)), this, SLOT(updateUsed()));
}

WorkDialog::~WorkDialog()
{
    delete ui;
}

void WorkDialog::accept()
{
    try {
        flushToWork();
    } catch (const runtime_error& ) {
        if (QMessageBox::warning(this, "Whid",
                "Invalid input.\n"
                "Do you want to quit the edit anyway?",
                QMessageBox::Yes | QMessageBox::No,
                QMessageBox::No) == QMessageBox::No) {
            return;
        }
    }

    // Emit the original instance
    *work_ = *ownWork_;
    emit dataChanged(ix_, work_);

    QDialog::accept();
}

void WorkDialog::updateUsed()
{
    try {
        flushToWork();
    } catch (const runtime_error&) {
        QSound::play(":/res/sounds/error.wav");
    }
    ui->usedEdit->setText(toHourMin(ownWork_->getUsed()));
}

void WorkDialog::flushToWork()
{
    ownWork_->name = ui->nameEdit->text();
    ownWork_->start = ui->fromDateTime->dateTime();
    ownWork_->end = ui->toDateTime->dateTime();
    ownWork_->note = ui->noteEdit->toPlainText();
    ownWork_->setStatus(ui->statusGroup->checkedButton()->property("v").toInt());
    ownWork_->paused = parseDuration(ui->pauseEdit->text());
}
