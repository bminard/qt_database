#include "gtest/gtest.h"

#include "database.h"
#include "file_management.h"

namespace {

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
        ASSERT_FALSE(fileExists(database));
    }

    /*! @brief Ensure test cases satisfy all postconditions.
     */
    void TearDown() override {
        ASSERT_TRUE(fileExists(database));
    }

    const QString database; //! The full file system path to the test database.
};

/* Check database connection.
 */
TEST_F(TestDatabase, DatabaseConnection) {
    Database::connect(database);
    ASSERT_TRUE(fileExists(database) and fileEmpty(database));
}

/*! @brief Test database fixture for database connections.
 */
class TestDatabaseConnection
    : public TestDatabase
{
protected:
    /*! @brief Generate the test database name and path.
     */
    TestDatabaseConnection()
        : TestDatabase()
    {
    }

    /*! @brief Ensure test cases satisfy all preconditons.
     */
    void SetUp() override {
        TestDatabase::SetUp();
        Database::connect(database);
        ASSERT_TRUE(fileExists(database));
    }

    /*! @brief Ensure test cases satisfy all postconditions.
     */
    void TearDown() override {
        TestDatabase::TearDown();
    }
};

/* Check database initialization.
 */
TEST_F(TestDatabaseConnection, DatabaseInitialization) {
    Database::init();
    ASSERT_TRUE(not fileEmpty(database));
}

/*! @brief Test database fixture for database initialization.
 */
class TestDatabaseInitialize
    : public TestDatabaseConnection
{
protected:
    /*! @brief Generate the test database name and path.
     */
    TestDatabaseInitialize()
        : TestDatabaseConnection()
    {
    }

    /*! @brief Ensure test cases satisfy all preconditons.
     */
    void SetUp() override {
        TestDatabaseConnection::SetUp();
        Database::init();
        ASSERT_TRUE(not fileEmpty(database));
    }

    /*! @brief Ensure test cases satisfy all postconditions.
     */
    void TearDown() override {
        TestDatabaseConnection::TearDown();
    }
};

/* Populate database.
 */
TEST_F(TestDatabaseInitialize, DatabasePopulate) {
    Database::populate();
}

} // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
