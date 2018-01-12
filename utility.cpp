
#include "utility.h"

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
