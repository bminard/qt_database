#include <QSqlError>
#include <QSqlQuery>

#include "schema.h"

using Database::Schema;
using Database::TableRevision;

/*! @brief Destructor for the database schema object.
 */
Schema::~Schema() {
}

/*! @brief Execution the migration for this table revision.
 */
const bool TableRevision::migrate() {
    QSqlQuery query(sql_statement);
    if(!query.isActive()) {
        return false;
    }
    return true;
}
