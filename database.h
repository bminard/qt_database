#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <QSqlDatabase>
#include <QString>

namespace Database {

/*! @brief Create a database object for managing database connections.
 *
 * All databases have a name and may optionally rely upon a connection name.
 */
class Database
{
public:
    Database(const QString&, const QString& = QLatin1String(QSqlDatabase::defaultConnection));
    ~Database();

    const bool execute(const QString&);

private:
    static const QString DRIVER; //! The name of the database driver required by this class.
    const QString connection_name; //! The database connection name.
};

/*--- Legacy ---*/

void connect(const QString&);
void init();
void populate();

} // name space

#endif // _DATABASE_H_
