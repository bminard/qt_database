#include "gtest/gtest.h"

#include "database.h"
#include <QFileInfo>
#include <cstdio>
#include <string.h>

namespace {

/*! @brief Manage temporary files.
 *
 * This class ensure that files are created using unique names and removed when
 * they are no longer needed.
 */
class FileManager
{
public:

    /*! @brief Construct the temporary file name.
     * @param t a file name template.
     * @note The file name template must conform to the requirements of
     *     man 3 mktemp. This parameter cannot be null or empty and must
     *      include at least one 'X'.
     * @throw whenever a unique file name cannot be generated.
     */
    FileManager(const char * const t) {
        file_name = new char[strlen(t) + 1];
        strcpy(file_name, t);
        if(0 == strlen(mktemp(file_name))) {
            throw "cannot find a unique file name";
        }
    }

    /*! @brief Return the generated file name.
     * @return the temporary file's name
     */
    const char * const get_name() const {
        return file_name;
    }

    /*! @brief Destroy the temporary file, if it exists.
     */
    ~FileManager() {
        if(std::remove(file_name) != 0)
            std::perror( "Error deleting file" );
        delete file_name;
    }

private:
    char *file_name; //! The temporary file's name.
};

/* @brief Determine if the file is empty.
 * @param path is the path to a file
 * @return true iff file is empty (zero length); false otherwise
 */
const bool fileEmpty(const QString path) {
    QFileInfo check_file(path);
    if (check_file.exists() and 0 == check_file.size()) {
        return true;
    }
    return false;
}

/* @brief Determine if a file exists.
 * @param path is the path to a file
 * @return true if file exists; false otherwise
 */
const bool fileExists(const QString& path) {
    QFileInfo check_file(path);
    if (check_file.exists() && check_file.isFile()) {
        return true;
    }
    return false;
}

/*! @brief Test database fixture.
 */
class TestDatabase
    : public ::testing::Test
    , public FileManager
{
protected:
    /*! @brief Generate the test database name and path.
     */
    TestDatabase()
        : FileManager("/tmp/database.XXXXXX")
        , database(get_name())
    {
    }

    /*! @brief Ensure test cases satisfy all preconditons.
     */
    void SetUp() {
        ASSERT_FALSE(fileExists(database));
    }

    /*! @brief Ensure test cases satisfy all postconditions.
     */
    void TearDown() {
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
    void SetUp() {
        Database::connect(database);
        ASSERT_TRUE(fileExists(database));
    }
};

/* Check database initialization.
 */
TEST_F(TestDatabaseConnection, DatabaseInitialization) {
    Database::init();
    ASSERT_TRUE(not fileEmpty(database));
}

} // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
