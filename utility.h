#ifndef UTILITY_H
#define UTILITY_H

#include <QModelIndexList>
#include <QString>

QString toHourMin(const int duration);
int parseDuration(const QString& value);
bool isOneRow(const QModelIndexList& list);

#endif // UTILITY_H
