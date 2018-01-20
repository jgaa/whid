#ifndef LOGGING_H
#define LOGGING_H

#include <memory>

#include <QFile>



class Logging
{
public:
    Logging();
    ~Logging();

    static void logMessageHandler(QtMsgType type,
                                  const QMessageLogContext &context,
                                  const QString &msg);

    void onLogMessageHandler(QtMsgType type,
                             const QMessageLogContext &context,
                             const QString &msg);

private:
    void open();

    std::unique_ptr<QFile> logFile_;
    static Logging *instance_;
};

#endif // LOGGING_H
