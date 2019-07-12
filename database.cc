// Example from Davide Coppola. Article at http://blog.davidecoppola.com/2016/11/howto-embed-database-in-application-with-sqlite-and-qt/.
// See https://github.com/vivaladav/BitsOfBytes/blob/master/howto-embed-database-in-application-with-sqlite-and-qt/QtSqlExample/MainWindow.cpp

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>

#include "database.h"

/*! @brief Create an SQLite database.
 * @param path the path to the database
 */
void create(const QString &path) {
    const QString DRIVER("QSQLITE");
    if(QSqlDatabase::isDriverAvailable(DRIVER)) {
        QSqlDatabase db = QSqlDatabase::addDatabase(DRIVER);

        db.setDatabaseName(path);

        if(!db.open()) {
            qWarning() << "create - ERROR: " << db.lastError().text();
        }
    }
    else {
        qWarning() << "create - ERROR: no driver " << DRIVER << " available";
    }
}
