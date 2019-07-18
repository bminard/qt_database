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
        Table revision1("INSERT INTO people(name) VALUES('Eddie Guerrero')");

        TableHistory history;
        history.add(&revision0);
        history.add(&revision1);
        if(history.migrate(database)) {
            return 0;
        }
    }
    return 1;
}
