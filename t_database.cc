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
    FileTemplateManager test_file_path("/tmp/database.XXXXXX");;
    QString database(test_file_path.get_template());

    create(database);
    ASSERT_TRUE(fileExists(database));
}

} // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
