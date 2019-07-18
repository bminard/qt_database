#include <QSqlError>
#include <QSqlQuery>

#include "schema.h"

using Database::Schema;
using Database::TableRevision;
using Database::TableRevisionHistory;

/*! @brief Constuctor for the database schema object.
 */
Schema::Schema() noexcept {
}

/*! @brief Destructor for the database schema object.
 */
Schema::~Schema() {
}

/*! @brief Carry out table migration from revision \f$n\f$ to revision \f$n + 1\f$.
 * @note Clients should be using the TableRevisionHistory class to migrate a database schema.
 * @returns true iff the migration succeeds; false otherwise
 * @param database database to use to publish schema
 */
const bool TableRevision::migrate(Database& database) {
    return database.execute(sql_statement);
}

/*! @brief Add a revision to the schema.
 * @note Revisions are replayed in the order they are added.
 * @param r a schema revision
 */
void TableRevisionHistory::add(value_type r) {
    revision_history.push_back(r);
}

/*! @brief Database schema migrator constructor.
 */
TableRevisionHistory::TableRevisionHistory()
    : Schema()
    , revision_history() {
}

/*! @brief Virtual destructor for database schema migrator.
 */
TableRevisionHistory::~TableRevisionHistory() {
}

/*! @brief Carry out database migration from revision \f$0\f$ to revision \f$n\f$.
 * @returns true iff the migration succeeds; false otherwise
 * @param database database to use to publish schema
 *
 * Migrators are called in the same order they were added to the history.
 */
const bool TableRevisionHistory::migrate(Database& database) {
    for(auto structure: revision_history) {
        if(false == structure->migrate(database)) {
            return false;
        }
    }
    return true;
}
