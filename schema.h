#ifndef _SCHEMA_H_
#define _SCHEMA_H_

#include <vector>

namespace Database {

/*! @brief Class interface to the whole-part heirarchy of the database schema.
 * @note The application should use this class to manage all database migrations.
 *
 * The Schema provides the interface used to initiate a database migration. It
 * can be a table reiviosn or a collection of tables (and their revisions). It
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
    virtual const bool migrate() = 0;

protected:
    /*! @brief Construct the database schema object.
     */
    Schema() noexcept { }
};

} // name space

#endif // _SCHEMA_H_
