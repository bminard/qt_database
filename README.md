# Qt Database

Experiments with Qt's SQLite database driver.

---

Updated original example by Davide Coppela to include classes and a schema (based upon the Composite design pattern).

The motivation for this change was the recognition that database migrations could be handled using the Composite design pattern.
In this implementation, the Component is the 'Schema', the Composite the 'TableRevisionHistory' and the Leaf the 'TablleRevision'.

A table revision history is a list of table revisions executed on the database.
In effect this call can be used to provide a database migration in one direction (forward).

The use case I was exploring involved the following.
- Application A, revision 1 uses database schema D, revision 1 and is deployed.
- Later, Application A, revision 2 using database schema D, revision 2 is deployed.

When Application revision 2 is available the following situation results.
- New users install revision 2 without trouble.
- Users upgrading from revision 1 don't have the correct database schema to support revision 2.

What to do?

The Schema class provides a way of managing database migrations for users upgrading from revision 1.

Usage:

```
#include <QFileInfo>

#include "database.h"
#include "schema.h"

class TableHistory
    : public TableRevisionHistory
{
public:
    /*! @brief Create the table history object.
     */
    TableHistory()
        : TableRevisionHistory() { }

    /*! @brief Destroy the table history object.
     */
    ~TableHistory() { }
};

class Table
    : public TableRevision
{
public:
    typedef TableRevision::sql_statement_type sql_statement_type;

    Table(sql_statement_type s)
        : TableRevision(s) { }

    ~Table() { }
};

int main()
{
    QString db_path("test.db");

    Database database(db_path);
    QFileInfo file(db_path);
    if(0 == file.size()) {
        Table revision0("CREATE TABLE people (id INTEGER PRIMARY KEY, name TEXT)");
        Table revision1("INSERT INTO people(name) VALUES('Eddie Guerrero')"); // For a migration, this would be an ALTER TABLE or some such.

        TableHistory history;
        history.add(&revision0);
        history.add(&revision1);
        if(history.migrate(database)) {
            return 0;
        }
    }
    return 1;
}
```

---

Example by Davide Coppola. (With Thanks!)
See his Article at [How to embed a database in your application with SQLite and Qt](http://blog.davidecoppola.com/2016/11/howto-embed-database-in-application-with-sqlite-and-qt/).
Davide's source code at [GitHub](https://github.com/vivaladav/BitsOfBytes/blob/master/howto-embed-database-in-application-with-sqlite-and-qt/QtSqlExample/MainWindow.cpp).
