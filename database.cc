#include <exception>

#include <QDebug>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>

#include "database.h"

const QString Database::Database::DRIVER("QSQLITE"); //! The SQLite database driver.

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
Database::Database::Database(const QString& database_name, const QString& connection_name)
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
Database::Database::~Database()
{
    QSqlDatabase::removeDatabase(connection_name);
}

/*! @brief Execute the provided SQL statement on the database.
 * @param sql_statement he SQL statement to execute on the database.
 * @note SQLite must execute one SQL statement at a time.
 * @return true if the query completes successfully; false otherwise
 */
const bool Database::Database::execute(const QString& sql_statement) {
    QSqlDatabase db = QSqlDatabase::database(connection_name); 
    QSqlQuery query(db);     
    return query.exec(sql_statement);
}

/*--- Legacy ---*/

/*! @brief Create an SQLite database.
 * @param path the path to the database
 */
void Database::connect(const QString& path) {
    const QString DRIVER("QSQLITE");
    if(QSqlDatabase::isDriverAvailable(DRIVER)) {
        QSqlDatabase db = QSqlDatabase::addDatabase(DRIVER);

        db.setDatabaseName(path);

        if(!db.open()) {
            qWarning() << "connect - ERROR: " << db.lastError().text();
        }
    }
    else {
        qWarning() << "connect - ERROR: no driver " << DRIVER << " available";
    }
}

/*! @brief Initialize the database.
 * @note Call this function only if the database needs to be created from
 *      scratch (i.e., is empty).
 */
void Database::init() {
	QSqlQuery query("CREATE TABLE people (id INTEGER PRIMARY KEY, name TEXT)");

	if(!query.isActive())
		qWarning() << "MainWindow::DatabaseInit - ERROR: " << query.lastError().text();
}

/*! @brief Populate the database.
 */
void Database::populate() {
	QSqlQuery query;

	if(!query.exec("INSERT INTO people(name) VALUES('Eddie Guerrero')"))
		qWarning() << "MainWindow::DatabasePopulate - ERROR: " << query.lastError().text();
}
