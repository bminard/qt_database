#include "gtest/gtest.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

#include "database.h"
#include "file_management.h"
#include "schema.h"

namespace {

using Database::Schema;
using Database::TableRevision;
using Database::TableRevisionHistory;

/*******************************************************************************
 * Schema tests.
 ******************************************************************************/

/*! @brief Construct the test database.
 */
class TestDatabase
    : public DatabaseFileName
{
public:
    TestDatabase() 
        : DatabaseFileName()
        , database_name(get_name())
        , database(database_name)
    {
    }

    ~TestDatabase()
    {
    }

protected:
    const QString database_name; //! The full file system path to the test database.
    Database::Database database; //! The database to use in the tests.
};

/*! @brief Wrapper for Schema to enable the Schema constructor to be tested.
 */
class EmptySchema
    : public Schema 
{
public: 
    /*! @brief Construct an empty schema object.
     */
    EmptySchema(): Schema() {
    }

    /*! @brief Perform a migration on the empty schema.
     */
    const bool migrate(Database::Database&) {
        return true;
    }
};

TEST(TestSchema, Constructor) {
    EXPECT_NO_THROW(EmptySchema());
}

/*! @brief Schema test fixture.
 */
class SchemaTestFixture
    : public ::testing::Test
    , public TestDatabase
{
protected:
    /*! @brief Set up the schema object.
     */
    void SetUp() override {
        s = new EmptySchema;
    }

    /*! @brief Tear down the schema object.
     */
    void TearDown() override {
        delete s;
    }

    EmptySchema *s; //! The empty schema object.
};

TEST_F(SchemaTestFixture, Migrate) {
    ASSERT_EQ(true, s->migrate(database));
}

/*******************************************************************************
 * TableRevision class tests.
 ******************************************************************************/

/*! @brief A list of SQL statements to test the TableRevision with.
 */
class SQLStatementTestFixture
    : public ::testing::TestWithParam<TableRevision::sql_statement_type>
{
protected:
    TableRevision::sql_statement_type sql_statement;
};

TEST_P(SQLStatementTestFixture, Constructor) {
    EXPECT_NO_THROW(TableRevision(GetParam()));
}

INSTANTIATE_TEST_SUITE_P(
    TableRevisionTests, SQLStatementTestFixture, ::testing::Values("", "brian", "CREATE TABLE people (id INTEGER PRIMARY KEY, name TEXT)")
);

/*! @brief Test database fixture.
 */
class TestDatabaseFixture
    : public ::testing::Test
    , public TestDatabase
{
protected:
    /*! @brief Ensure test cases satisfy all preconditons.
     */
    void SetUp() override {
        ASSERT_TRUE(fileExists(database_name));
    }

    /*! @brief Ensure test cases satisfy all postconditions.
     */
    void TearDown() override {
        ASSERT_TRUE(fileExists(database_name));
    }
};

TEST_F(TestDatabaseFixture, MirgrateFails) {
    TableRevision r("");
    ASSERT_FALSE(r.migrate(database));
}

TEST_F(TestDatabaseFixture, MirgratePasses) {
    TableRevision r("CREATE TABLE people (id INTEGER PRIMARY KEY, name TEXT)");
    ASSERT_TRUE(r.migrate(database));
}

/*******************************************************************************
 * TableRevisionHistory class tests.
 ******************************************************************************/

/*! @brief Test the TableRevisionHistory class.
 */
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

TEST(TableRevisionTest, Constructor) {
    ASSERT_NO_THROW(TableHistory history);
}

class TableRevisionTestFixture
    : public ::testing::Test
    , public TestDatabase
{
protected:

    void SetUp() {
        history = new TableHistory;
    }

    void TearDown() {
        delete history;
    }

   TableHistory *history;
};

TEST_F(TableRevisionTestFixture, MigrateWhenNoHistoryExists) {
    ASSERT_TRUE(history->migrate(database));
}

class Table
    : public TableRevision
{
public:
    typedef TableRevision::sql_statement_type sql_statement_type;

    Table(sql_statement_type s)
        : TableRevision(s) { }

    ~Table() { }
};

TEST_F(TableRevisionTestFixture, Add) {
    Table *revision = new Table("CREATE TABLE people (id INTEGER PRIMARY KEY, name TEXT)");
    ASSERT_NO_THROW(history->add(revision));
}

TEST_F(TableRevisionTestFixture, MigrateOne) {
    Table *revision = new Table("CREATE TABLE people (id INTEGER PRIMARY KEY, name TEXT)");
    ASSERT_NO_THROW(history->add(revision));
    ASSERT_TRUE(history->migrate(database));
}

TEST_F(TableRevisionTestFixture, MigrateTwo) {
    Table *revision0 = new Table("CREATE TABLE people (id INTEGER PRIMARY KEY, name TEXT)");
    Table *revision1 = new Table("INSERT INTO people(name) VALUES('Eddie Guerrero')");
    ASSERT_NO_THROW(history->add(revision0));
    ASSERT_NO_THROW(history->add(revision1));
    ASSERT_TRUE(history->migrate(database));

    QSqlQuery query;
    query.prepare("SELECT name FROM people WHERE name = (:name)");
    const QVariant s(QString("Eddie Guerrero"));
    query.bindValue(":name", s);
    if (query.exec()) {
       if (query.next()) {
          ASSERT_TRUE(1);
       }
       else {
          ASSERT_TRUE(0);
       }
    }
}

} // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
