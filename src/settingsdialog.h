#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

signals:
    void logSettingsChanged();

private:
    Ui::SettingsDialog *ui;

    // QDialog interface
public slots:
    void accept() override;
    void selectDbFile();
private slots:
    void on_actionSelect_Log_File_triggered();
};

#endif // SETTINGSDIALOG_H
