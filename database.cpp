
#include <assert.h>

#include <QStandardPaths>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QSettings>
#include <QDir>
#include <QMessageBox>

#include "database.h"

using namespace std;

Database::Database()
{
    const QString DRIVER("QSQLITE");
    QSettings settings;

    const auto new_dbpath = settings.value("new-dbpath").toString();
    if (!new_dbpath.isEmpty()) {
        bool do_use_new = true;
        if (settings.value("new-dbpath-copy").toBool()) {
            const auto old_dbpath = settings.value("dbpath").toString();
            bool do_copy = true;
            if (QFileInfo::exists(new_dbpath)) {
                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setText("I am about to change the database location from "
                               + old_dbpath
                               + " to " + new_dbpath
                               + ". The new database does however already exist!");
                msgBox.setInformativeText("Press YES to overwrite the existing database at the new location, "
                                          "NO to change to the new location and use the existing database that is already there, "
                                          "DISCARD to continue to use the old database at it's old location or "
                                          "Cancel to exit the program without any changes.");
                msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Discard | QMessageBox::Cancel);
                msgBox.setDefaultButton(QMessageBox::Cancel);
                switch(msgBox.exec()) {
                    case QMessageBox::Yes:
                        break;
                    case QMessageBox::No:
                        do_copy = false;
                        break;
                    case QMessageBox::Discard:
                        do_copy = false;
                        do_use_new = false;
                        break;
                    default:
                        throw runtime_error("Database change aborted by user");
                }
            }

            if (do_copy) {
                assert(do_use_new);
                qInfo() << "Copying database from " << old_dbpath << " to " << new_dbpath;
                if (!QFile::copy(old_dbpath, new_dbpath)) {
                    qWarning() << "Failed to the database from " << old_dbpath << " to " << new_dbpath;
                }
            }
        }

        if (do_use_new) {
            settings.setValue("dbpath", new_dbpath);
        }

        settings.remove("new-dbpath");
    }

    const auto dbpath = settings.value("dbpath").toString();
    const bool new_database = !QFileInfo(dbpath).isFile();

    if(!QSqlDatabase::isDriverAvailable(DRIVER)) {
        throw runtime_error("Missing sqlite3 support");
    }

    db_ = QSqlDatabase::addDatabase(DRIVER);
    db_.setDatabaseName(dbpath);

    if (!db_.open()) {
        qWarning() << "Failed to open database: " << dbpath;
        throw runtime_error("Failed to open database");
    }

    if (new_database) {
        qInfo() << "Creating new database at location: " << dbpath;
        createDatabase();
    }

    QSqlQuery query("SELECT * FROM whid");
    if (!query.next()) {
        throw runtime_error("Missing configuration record in database");
    }

    const auto dbver = query.value(WT_VERSION).toInt();
    qDebug() << "Database schema version is " << dbver;
    if (dbver != currentVersion) {
        qWarning() << "Database schema version is "
                   << dbver
                   << " while I expected " << currentVersion;
    }

    // TODO: Remove the below queries before beta (tmp fixes).
    exec("UPDATE node SET parent=NULL WHERE id=0");
    exec("UPDATE node SET parent=0 WHERE parent IS NULL AND id > 0");
}

void Database::createDatabase()
{
    db_.transaction();

    try {
        exec(R"(CREATE TABLE "whid" ( `version` INTEGER NOT NULL ))");
        exec(R"(CREATE TABLE "charge" ( `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, `name` TEXT NOT NULL UNIQUE, `rate` INTEGER NOT NULL DEFAULT 0, `currency` TEXT NOT NULL DEFAULT "eur", `roundup_sec` INTEGER NOT NULL DEFAULT 0, `minimun_time` INTEGER NOT NULL DEFAULT 0 ))");
        exec(R"(CREATE TABLE "node" ( `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, `name` REAL NOT NULL, `type` INTEGER NOT NULL, `status` INTEGER NOT NULL DEFAULT 0, `descr` TEXT, `active` INTEGER NOT NULL DEFAULT 1, `charge` INTEGER, `parent` INTEGER, FOREIGN KEY(`charge`) REFERENCES `charge`(`id`), FOREIGN KEY(`parent`) REFERENCES node(id)))");
        exec(R"(CREATE TABLE "work" ( `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, `status` INTEGER NOT NULL DEFAULT 0, `start` INTEGER NOT NULL, `end` INTEGER NOT NULL, `used` INTEGER NOT NULL, `paused` INTEGER NOT NULL DEFAULT 0, `name` TEXT NOT NULL, `note` TEXT, `charge` INTEGER, `node` INTEGER NOT NULL, FOREIGN KEY(`charge`) REFERENCES `charge`(`id`), FOREIGN KEY(`node`) REFERENCES `node`(`id`) ))");

        exec(R"(INSERT INTO node (id, name, type) VALUES (0, "root", 0))");

        QSqlQuery query(db_);
        query.prepare("INSERT INTO whid (version) VALUES (:version)");
        query.bindValue(":version", currentVersion);
        if(!query.exec()) {
            throw runtime_error("Failed to initialize database");
        }

    } catch(const exception&) {
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
        throw runtime_error(string("SQL query failed: ") + query.lastError().text().toStdString());
    }
}

