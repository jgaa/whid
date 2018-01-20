#include "logging.h"
#include <iostream>
#include <assert.h>
#include <array>
#include <QSettings>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>

using namespace std;

Logging *Logging::instance_ = {};

Logging::Logging()
{
    assert(!instance_);
    instance_ = this;
}

Logging::~Logging()
{
    assert(instance_);
    instance_ = {};
}

void Logging::logMessageHandler(QtMsgType type,
                                const QMessageLogContext &context,
                                const QString &msg)
{
    if (instance_) {
        instance_->onLogMessageHandler(type, context, msg);
    }
}

void Logging::onLogMessageHandler(QtMsgType type,
                                  const QMessageLogContext &context,
                                  const QString &msg)
{
    array<QString, 6> message_types = {{
        "debug", "warn", "error", "fatal", "info", "system"
    }};

    const auto now = QDateTime::currentDateTime().toString(Qt::ISODateWithMs);
    QSettings settings;

    if (settings.value("log-enabled", false).toBool()) {

        if (!logFile_) {
            open();
        }

        QTextStream stream(logFile_.get());

        stream
            << now
            << ' '
            << message_types.at(type)
            << ' '
            << context.function
            << ' '
            << msg;
    }

    clog << now.toStdString()
         << ' '
         << message_types.at(type).toStdString()
         << ' '
         << context.function
         << ' '
         << msg.toStdString()
         << endl;
}

void Logging::open()
{
    QSettings settings;

    const auto path = settings.value("log-path", "whid.log").toString();
    QIODevice::OpenMode options = QIODevice::WriteOnly;
    QString mode = "truncate";
    if (settings.value("log-append", false).toBool()) {
        options |= QIODevice::Append;
        mode = "append";
    } else {
        options |= QIODevice::Truncate;
    }
    qDebug() << "Opening log-file: " << path << " [" << mode << ']';
    logFile_ = make_unique<QFile>(path);
    logFile_->open(options);
}
