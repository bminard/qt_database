#include "gtest/gtest.h"

#include "database.h"
#include <QFileInfo>
#include <cstdio>
#include <string.h>

namespace {

class FileManager
{
public:
    FileManager(const char * const t) {
        file_name = new char[strlen(t) + 1];
        strcpy(file_name, t);
        mktemp(file_name);
    }

    const char * const get_name() const {
        return file_name;
    }

    ~FileManager() {
        if(std::remove(file_name) != 0)
            std::perror( "Error deleting file" );
        delete file_name;
    }

private:
    char *file_name;
};

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
    FileManager test_file_path("/tmp/database.XXXXXX");
    QString database(test_file_path.get_name());

    create(database);
    ASSERT_TRUE(fileExists(database));
}

} // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
