#ifndef UTILITY_H
#define UTILITY_H

#include <thread>

#include <QModelIndexList>
#include <QString>

QString toHourMin(const int duration);
int parseDuration(const QString& value);
bool isOneRow(const QModelIndexList& list);
void scheduleAtMidnight(std::function<void()> func);

#endif // UTILITY_H
