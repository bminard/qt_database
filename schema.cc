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

/*! @brief Execution the migration for this table revision.
 */
const bool TableRevision::migrate() {
    return false; // database.execute(sql_statement);
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

/*! @brief Carry out database migration from revision \f$n\f$ to revision \f$n + 1\f$.
 * @returns true iff the migration succeeds; false otherwise
 *
 * Migrators are called in the same order they were added.
 */
const bool TableRevisionHistory::migrate() {
    for(auto structure: revision_history) {
        if(false == structure->migrate()) {
            return false;
        }
    }
    return true;
}

/*! @brief Add a revision to the schema.
 * @note Revisions are replayed in the order they are added.
 * @param r a schema revision
 */
void TableRevisionHistory::add(value_type r) {
    revision_history.push_back(r);
}
