
#include <QStandardPaths>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

#include "database.h"

Database::Database()
{
    const QString DRIVER("QSQLITE");
    auto data_path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    if (!QDir(data_path).exists()) {
        QDir().mkdir(data_path);
    }
#ifdef QT_DEBUG
    data_path += "/whid-debug.db";
#else
    data_path += "/whid.db";
#endif
    const bool new_database = QFileInfo(data_path).isFile() == false;

    if(!QSqlDatabase::isDriverAvailable(DRIVER)) {
        throw std::runtime_error("Missing sqlite3 support");
    }

    db_ = QSqlDatabase::addDatabase(DRIVER);
    db_.setDatabaseName(data_path);

    if (!db_.open()) {
        throw std::runtime_error("Failed to open database");
    }

    if (new_database) {
        createDatabase();
    }

    QSqlQuery query("SELECT * FROM whid");
    if (!query.next()) {
        throw std::runtime_error("Missing configuration record in database");
    }

    const auto dbver = query.value(WT_VERSION).toInt();
    qDebug() << "Database schema version is " << dbver;
    if (dbver != currentVersion) {
        qWarning() << "Database schema version is "
                   << dbver
                   << " while I expected " << currentVersion;
    }
}

void Database::createDatabase()
{
    db_.transaction();

    try {
        exec(R"(CREATE TABLE "whid" ( `version` INTEGER NOT NULL ))");
        exec(R"(CREATE TABLE `charge` ( `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, `name` TEXT NOT NULL UNIQUE, `rate` INTEGER NOT NULL DEFAULT 0, `currency` TEXT NOT NULL DEFAULT "eur", `roundup_sec` INTEGER NOT NULL DEFAULT 0, `minimun_time` INTEGER NOT NULL DEFAULT 0 ))");
        exec(R"(CREATE TABLE "node" ( `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, `name` REAL NOT NULL, `type` INTEGER NOT NULL, `status` INTEGER NOT NULL DEFAULT 0, `descr` TEXT, `active` INTEGER NOT NULL DEFAULT 1, `charge` INTEGER, `parent` INTEGER, FOREIGN KEY(`charge`) REFERENCES `charge`(`id`), FOREIGN KEY(`parent`) REFERENCES node(id)))");
        exec(R"(CREATE TABLE "work" ( `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, `status` INTEGER NOT NULL DEFAULT 0, `start` INTEGER NOT NULL, `end` INTEGER NOT NULL, `used` INTEGER NOT NULL, `paused` INTEGER NOT NULL DEFAULT 0, `name` TEXT NOT NULL, `note` TEXT, `charge` INTEGER, `node` INTEGER NOT NULL, FOREIGN KEY(`charge`) REFERENCES `charge`(`id`), FOREIGN KEY(`node`) REFERENCES `node`(`id`) ))");

        QSqlQuery query(db_);
        query.prepare("INSERT INTO whid (version) VALUES (:version)");
        query.bindValue(":version", currentVersion);
        if(!query.exec()) {
            throw std::runtime_error("Failed to initialize database");
        }

    } catch(const std::exception& ex) {
        db_.rollback();
        throw;
    }

    db_.commit();
}

void Database::exec(const char *sql)
{
    QSqlQuery query(db_);
    query.exec(sql);
    if (query.lastError().type() != QSqlError::NoError) {
        throw std::runtime_error(std::string("SQL query failed: ") + query.lastError().text().toStdString());
    }
}

