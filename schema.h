#ifndef _SCHEMA_H_
#define _SCHEMA_H_

#include <vector>

#include "database.h"

/*! @brief Class interface to the whole-part heirarchy of the database schema.
 * @note The application should use this class to manage all database migrations.
 *
 * The Schema provides the interface used to initiate a database migration. It
 * can be a table revision or a collection of tables (and their revisions). It
 * fulfills the requirements of the *Composite* component in the Composite
 * design pattern.
 */
class Schema
{
public:
    /*! @brief Virtual destructor for the database schema object.
     */
    virtual ~Schema() = 0;

    /*! @brief Carry out database migration from revision \f$n\f$ to \f$n + 1, n > 0\f$.
     */
    virtual const bool migrate(Database&) = 0;

protected:
    /*! @brief Construct the database schema object.
     */
    Schema() noexcept;
};

/*! @brief Extend the Schema interface to a single table revision.
 *
 * The TableRevision provides the interface used to create a table revision. It
 * contains a single SQL statement which, when executed, changes the table
 * structure. It fulfills the requirements of the *Leaf* component in the
 * Composite design pattern.
 */
class TableRevision
    : public Schema
{
public:
    typedef const char *sql_statement_type;

    /*! @brief Construct the query defining the revised table structure.
     * @param s an SQL statement
     */
    TableRevision(sql_statement_type s)
        : sql_statement(s)
    {
    }

    /*! @brief Destructor for the table revision object.
     */
    ~TableRevision() { }

    virtual const bool migrate(Database&) override;

private:
    sql_statement_type sql_statement; // The SQL statement used to revise the table structure.
};

/*! @brief Extend the Schema interface to a collection of table revisions.
 *
 * The TableRevisionHistory provides the interface used to manage the table
 * revision history. It contains a collection of table structure revisions (see
 * the TableRevision class) which when executed together update (or create) the
 * table to it's current structure. It fulfills the requirements of the
 * *Composite* component in the Composite design pattern.
 */
class TableRevisionHistory
    : public Schema
{
public:
    typedef Schema* value_type; //!< Schema container value type.
    typedef std::unique_ptr<value_type> pointer_type;  //!< Schema pointer type.
    typedef std::vector<pointer_type> container_type;  //!< Schema container type.

    virtual ~TableRevisionHistory();
    virtual const bool migrate(Database&) override;
    virtual void add(value_type);

protected:
    TableRevisionHistory();

private:
    container_type revision_history;
};

#endif // _SCHEMA_H_
