// Example from Davide Coppola. Article at http://blog.davidecoppola.com/2016/11/howto-embed-database-in-application-with-sqlite-and-qt/.
// See https://github.com/vivaladav/BitsOfBytes/blob/master/howto-embed-database-in-application-with-sqlite-and-qt/QtSqlExample/MainWindow.cpp

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>

int main()
{
    const QString DRIVER("QSQLITE");
    if(QSqlDatabase::isDriverAvailable(DRIVER)) {
        QSqlDatabase db = QSqlDatabase::addDatabase(DRIVER);

        db.setDatabaseName(":memory:");

        if(!db.open()) {
            qWarning() << "MainWindow::DatabaseConnect - ERROR: " << db.lastError().text();
        }
    }
    else {
        qWarning() << "MainWindow::DatabaseConnect - ERROR: no driver " << DRIVER << " available";
    }
}
