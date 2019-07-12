#include <QFileInfo>

#include "database.h"

int main()
{
    QString db_path("test.db");

    Database::connect(db_path);
}
