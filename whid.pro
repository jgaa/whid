#-------------------------------------------------
#
# Project created by QtCreator 2018-01-05T09:37:10
#
#-------------------------------------------------

CONFIG += c++14

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = whid
TEMPLATE = app
VERSION = 2.0.0.1

macx {
    ICON = res/icons/whid.icns
} win32 {
    ICON = res/icons/whid.ico
} unix:!macx {
    ICON = res/icons/whid.svg
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        src/main.cpp \
        src/mainwindow.cpp \
        src/database.cpp \
        src/nodemodel.cpp \
        src/currentworkmodel.cpp \
        src/workmodel.cpp \
        src/utility.cpp \
        src/summarymodel.cpp \
        src/weekselectiondialog.cpp \
        src/nodedialog.cpp \
        src/workdialog.cpp \
        src/settingsdialog.cpp \
        src/aboutdialog.cpp

HEADERS += \
        src/mainwindow.h \
        src/database.h \
        src/nodemodel.h \
        src/currentworkmodel.h \
        src/workmodel.h \
        src/utility.h \
        src/summarymodel.h \
        src/weekselectiondialog.h \
        src/nodedialog.h \
        src/workdialog.h \
        src/settingsdialog.h \
        src/aboutdialog.h \
        src/version.h

FORMS += \
        ui/mainwindow.ui \
        ui/weekselectiondialog.ui \
        ui/nodedialog.ui \
        ui/workdialog.ui \
        ui/settingsdialog.ui \
        ui/aboutdialog.ui

RESOURCES += \
    resources.qrc

DISTFILES += \
    scripts/package-macos.sh
