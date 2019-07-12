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

/* Check container state following initialization using default constructor.
 */
TEST(TestDatabase, Create) {
    FileManager test_file_path("/tmp/database.XXXXXX"); // At least one 'X' must appear in the test file path.
    QString database(test_file_path.get_name());

    create(database);
    ASSERT_TRUE(fileExists(database));
}

} // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
