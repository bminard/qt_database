#include "gtest/gtest.h"

#include "database.h"
#include "file_management.h"
#include "schema.h"

namespace {

using Database::Schema;
using Database::TableRevision;

/*! @brief Wrapper for Schema class to enable the Schema constructor to be called.
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
    const bool migrate() {
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
    ASSERT_EQ(true, s->migrate());
}

/*! @brief Create a list of SQL statements to test the TableRevision with.
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
class TestDatabase
    : public ::testing::Test
    , public DatabaseFileName
{
protected:
    /*! @brief Generate the test database name and path.
     */
    TestDatabase()
        : DatabaseFileName()
        , database(get_name())
    {
    }

    /*! @brief Ensure test cases satisfy all preconditons.
     */
    void SetUp() override {
        Database::connect(database);
        ASSERT_TRUE(fileExists(database));
    }

    /*! @brief Ensure test cases satisfy all postconditions.
     */
    void TearDown() override {
        ASSERT_TRUE(fileExists(database));
    }

    const QString database; //! The full file system path to the test database.
};

TEST_F(TestDatabase, MirgrateFails) {
    TableRevision r("");
    ASSERT_FALSE(r.migrate());
}

TEST_F(TestDatabase, MirgratePasses) {
    TableRevision r("CREATE TABLE people (id INTEGER PRIMARY KEY, name TEXT)");
    ASSERT_TRUE(r.migrate());
}

} // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
