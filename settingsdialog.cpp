#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QSettings>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    QSettings settings;

    ui->dbPathEdit->setText(settings.value("dbpath").toString());

}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}
