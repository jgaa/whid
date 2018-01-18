#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>

class Database
{
public:
    enum WhidTable {
        WT_VERSION = 0
    };

    Database();
    const int currentVersion = 1;

private:
    void createDatabase();
    void exec(const char *sql);

    QSqlDatabase db_;
};

#endif // DATABASE_H
