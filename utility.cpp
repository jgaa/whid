
#include "utility.h"

#include <QAbstractEventDispatcher>
#include <QDate>
#include <QTimer>
#include <QDebug>

QString toHourMin(const int duration)
{
    auto minutes = duration / 60;
    auto hours = minutes / 60;
    minutes -= (hours * 60);

    QString val;
    return val.sprintf("%02d:%02d", hours, minutes);
}

int parseDuration(const QString &value)
{
    int minutes = {}, hours = {};
    bool have_seen_column = false;

    for(const auto& ch : value) {
        if (ch >= '0' && ch <= '9') {
            minutes *= 10;
            minutes += (ch.toLatin1() - '0');
        } else if (ch == ':') {
            if (have_seen_column) {
                throw std::runtime_error("Invalid input");
            }
            hours = minutes * 60 * 60;
            minutes = 0;
        } else {
            throw std::runtime_error("Invalid input");
        }
    }

    return (minutes * 60) + hours;
}

bool isOneRow(const QModelIndexList &list)
{
    int row = -1;
    for(const auto& r: list) {
        if (row == -1) {
            row = r.row();
        } else {
            if (row != r.row()) {
                return false;
            }
        }
    }

    return row >= 0;
}

void scheduleAtMidnight(std::function<void ()> func)
{
    const auto ms_one_day{24 * 60 * 60 * 1000};
    const auto duration = ms_one_day - QTime::currentTime().msecsSinceStartOfDay();

    auto timer = new QTimer(QAbstractEventDispatcher::instance());
    timer->start(duration);
    qDebug() << QDateTime::currentDateTime().toString()
             << "scheduleAtMidnight: Scheduled in " << duration << "milliseconds";
    QObject::connect(timer, &QTimer::timeout, [func{move(func)}, &timer]{
        func();
        timer->deleteLater();
        qDebug() << QDateTime::currentDateTime().toString() << "scheduleAtMidnight: Processed event";
    });
}
