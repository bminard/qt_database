#include "gtest/gtest.h"

#include "database.h"
#include "file_management.h"

namespace {

/*! @brief Test database fixture.
 */
class TestDatabaseNameFixture
    : public ::testing::Test
    , public DatabaseFileName
{
protected:
    /*! @brief Generate the test database name and path.
     */
    TestDatabaseNameFixture()
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

TEST_F(TestDatabaseNameFixture, ConstructorWithDatabaseNameOnly) {
    ASSERT_NO_THROW(Database::Database db(database));
    ASSERT_TRUE(fileExists(database) and fileEmpty(database));
}

TEST_F(TestDatabaseNameFixture, ConstructorWithDefaultDatabaseConnectionName) {
    Database::Database db(database);
    QSqlDatabase sqlDb = QSqlDatabase::database();
    ASSERT_EQ(QLatin1String(QSqlDatabase::defaultConnection), sqlDb.connectionName());
}

class TestDatabaseNonDefaultConnectionFixture
    : public TestDatabaseNameFixture
{

protected:
    void SetUp() override {
        TestDatabaseNameFixture::SetUp();
        connection = "Test Database";
    }

    void TearDown() override {
        TestDatabaseNameFixture::TearDown();
    }

    QString connection; //! The database connection name.
};

TEST_F(TestDatabaseNonDefaultConnectionFixture, Constructor) {
    ASSERT_NO_THROW(Database::Database db(database, connection));
    ASSERT_TRUE(fileExists(database) and fileEmpty(database));
}

TEST_F(TestDatabaseNonDefaultConnectionFixture, ConstructorCheckConnectionName) {
    Database::Database db(database, connection);
    QSqlDatabase sqlDb = QSqlDatabase::database(connection);
    ASSERT_NE(QLatin1String(QSqlDatabase::defaultConnection), sqlDb.connectionName());
}

TEST_F(TestDatabaseNonDefaultConnectionFixture, ExecuteWithoutQuery) {
    Database::Database db(database, connection);
    ASSERT_FALSE(db.execute(""));
}

TEST_F(TestDatabaseNonDefaultConnectionFixture, ExecuteWithMalformedQuery) {
    Database::Database db(database, connection);
    ASSERT_FALSE(db.execute("CREAT TABLE people (id INTEGER PRIMARY KEY, name TEXT)"));
}

TEST_F(TestDatabaseNonDefaultConnectionFixture, ExecuteSucess) {
    Database::Database db(database, connection);
    ASSERT_TRUE(db.execute("CREATE TABLE people (id INTEGER PRIMARY KEY, name TEXT)"));
}

/*--- Legacy ---*/

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
