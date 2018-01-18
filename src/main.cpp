#include "mainwindow.h"
#include "version.h"
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QSettings>
#include <QStandardPaths>

void initSettings() {

    QSettings settings;

    auto data_path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (!QDir(data_path).exists()) {
        qDebug() << "Creating path: " << data_path;
        QDir().mkpath(data_path);
    }

    switch(auto version = settings.value("version").toInt()) {
        case 0: { // First use

            qInfo() << "First use - initializing settings.";
            settings.setValue("version", 1);

            QString dbpath = data_path;
#ifdef QT_DEBUG
            dbpath += "/whid-debug.db";
#else
            dbpath += "/whid.db";
#endif
            settings.setValue("dbpath", dbpath);

        } break;
    default:
        qDebug() << "Settings are OK at version " << version;
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    a.setOrganizationName("TheLastViking");
    a.setOrganizationDomain("lastviking.eu");

#ifdef QT_DEBUG
    a.setApplicationName("Whid-debug");
    //a.setApplicationVersion(WHID_VERSION);
#else
    a.setApplicationName("Whid");
#endif

    initSettings();

    try {
        w.initialize();
    } catch(const std::exception& ex) {
        qWarning() << "Caught exception during initialization: "
                   << ex.what();
        return -1;
    }

    w.show();

    return a.exec();
}
