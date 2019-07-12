#include <QFileInfo>

#include "database.h"

int main()
{
    QString db_path("test.db");

    Database::connect(db_path);
    QFileInfo database(db_path);
    if(0 == database.size()) {
        Database::init();
    }
    else {
        // Future home of database migrations.
    }
}
