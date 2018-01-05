#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

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
