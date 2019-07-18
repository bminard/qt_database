#include <exception>

#include <QDebug>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>

#include "database.h"

const QString Database::DRIVER("QSQLITE"); //! The SQLite database driver.

/*! @brief Open the named database using the provided connection name.
 * @param database_name the name of the database to open.
 * @param connection_name the connection name identifying the database connection
 * @throw logic_error if the requisite database driver is not installed
 * @throw runtime_error if the database cannot be opened
 *
 * Upon successful completion, the named database is opened and available for
 * use by the application whenever the application connects to the database
 * using the connection name.
 */
Database::Database(const QString& database_name, const QString& connection_name)
    : connection_name(connection_name)
{
    if(not QSqlDatabase::isDriverAvailable(DRIVER)) {
        throw std::logic_error(QString("Database::acquire_database_driver() - ERROR: no driver %1 available").arg(DRIVER).toStdString().c_str());
    }

    QSqlDatabase db = QSqlDatabase::addDatabase(DRIVER, connection_name);
    db.setDatabaseName(database_name);
    if(!db.open()) {
        throw std::runtime_error(QString("Database::connect() - ERROR: %1").arg(db.lastError().text()).toStdString().c_str());
    }
}

/*! @brief Destroy the database object.
 *
 * This method removes the database's connection name from the list of avaiable databases.
 */
Database::~Database()
{
    QSqlDatabase::removeDatabase(connection_name);
}

/*! @brief Execute the provided SQL statement on the database.
 * @param sql_statement he SQL statement to execute on the database.
 * @note SQLite must execute one SQL statement at a time.
 * @return true if the query completes successfully; false otherwise
 */
const bool Database::execute(const QString& sql_statement) {
    QSqlDatabase db = QSqlDatabase::database(connection_name); 
    QSqlQuery query(db);     
    return query.exec(sql_statement);
}
