#include "secretdatabasetests.h"

#include <QTest>
#include <QFileInfo>
#include <secretdatabase.h>

void SecretDatabaseTests::openDatabaseTest()
{
    QFileInfo testFile("./test.db");
    SecretDatabase testDb;

    // If it already exists, we want to delete it.
    testFile.setFile("./test.db");
    if (testFile.exists()) {
        // It exists.  Make sure it is actually a file.
        if (!testFile.isFile()) {
            QFAIL("test.db is not a file!  Please correct this and try again.");
        }
    }

    // Make sure we can open the database.
    QVERIFY(testDb.open("./test.db"));

    // Make sure the file exists now.
    QVERIFY(testFile.exists());
    QVERIFY(testFile.isFile());

    // Then, attempt to close the database.
    QVERIFY(testDb.close());
}
