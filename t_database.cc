#include "gtest/gtest.h"

#include "database.h"
#include <QFileInfo>
#include <cstdio>
#include <string.h>

namespace {

class FileTemplateManager
{
public:
    FileTemplateManager(const char * const t): file_name_template(t) {
        if(!file_name_template)
           throw "file template error";
    }

    ~FileTemplateManager() {
    }

    const char *get_template() const {
        return file_name_template;
    }

private:
    const char * const file_name_template;
};

class FileManager
{
public:
    FileManager(const FileTemplateManager& t): file_name_template(t) {
        file_name = new char[strlen(file_name_template.get_template()) + 1];
        strcpy(file_name, file_name_template.get_template());
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
    FileTemplateManager file_name_template;
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
    FileManager test_file_path(FileTemplateManager("/tmp/database.XXXXXX"));
    QString database(test_file_path.get_name());

    create(database);
    ASSERT_TRUE(fileExists(database));
}

} // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
