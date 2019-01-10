#include "secretdatabasetests.h"

#include <QTest>
#include <QFileInfo>
#include "../keystorage/database/secretdatabase.h"
#include "../keystorage/keyentry.h"

#define TEST_DB "./test.db"

void SecretDatabaseTests::databaseTests()
{
    QFileInfo testFile(TEST_DB);

    // Make sure our environment is clean.
    removeOldDatabaseFile();

    // Test that we can open the database.
    openDatabaseTest();

    // Test the discrete adding of a record.
    addDatabaseEntryDiscreteTest();

    // And the KeyEntry version.
    addDatabaseEntryKeyEntryTest();

    // Try getting all of the entries.
    getAllEntriesTest();

    // Test that we can close the database.
    closeDatabaseTest();
}

void SecretDatabaseTests::removeOldDatabaseFile()
{
    QFileInfo testFile(TEST_DB);

    // If it exists, we want to delete it.
    if (testFile.exists()) {
        // It exists.  Make sure it is actually a file.
        if (!testFile.isFile()) {
            QFAIL("test.db is not a file!  Please correct this and try again.");
        }

        // Delete it, so we can start with a clean slate.
        QVERIFY(QFile("./test.db").remove());
    }
}

void SecretDatabaseTests::openDatabaseTest()
{
    QFileInfo testFile(TEST_DB);

    // Make sure we can open the database.
    QVERIFY(mTestDatabase.open(TEST_DB));

    // Make sure the file exists now.
    QVERIFY(testFile.exists());
    QVERIFY(testFile.isFile());

    // Check that our schema version is what we expect.
    QCOMPARE(mTestDatabase.schemaVersion(), 1);
}

void SecretDatabaseTests::closeDatabaseTest()
{
    QVERIFY(mTestDatabase.close());

    // And, remove the test database file.
    QVERIFY(QFile(TEST_DB).remove());
}

void SecretDatabaseTests::addDatabaseEntryDiscreteTest()
{
    KeyEntry readBack;

    // Attempt to write an entry to the database.
    QVERIFY(mTestDatabase.add("id1", "mysecret", SECRETDATABASE_KEYTYPE_HEX, SECRETDATABASE_OTPTYPE_TOTP, 6));

    // Attempt to locate an entry that doesn't exist.
    QCOMPARE(mTestDatabase.getByIdentifier("invalidid", readBack), false);

    // Make sure the KeyEntry indicates it is invalid.
    QCOMPARE(readBack.valid(), false);

    // Read back what we just wrote.
    QVERIFY(mTestDatabase.getByIdentifier("id1", readBack));

    // Make sure the KeyEntry indicates it is valid.
    QVERIFY(readBack.valid());

    // And, make sure all the expected values are set.
    QCOMPARE(readBack.identifier(), "id1");
    QCOMPARE(readBack.secret(), "mysecret");
    QCOMPARE(readBack.keyType(), SECRETDATABASE_KEYTYPE_HEX);
    QCOMPARE(readBack.otpType(), SECRETDATABASE_OTPTYPE_TOTP);
    QCOMPARE(readBack.outNumberCount(), 6);
}

void SecretDatabaseTests::addDatabaseEntryKeyEntryTest()
{
    KeyEntry toWrite;
    KeyEntry readBack;

    // Make sure our toWrite value is invalid to start with.
    QCOMPARE(toWrite.valid(), false);

    // Build the secret entry that we want to write.
    toWrite.setIdentifier("id2");
    toWrite.setSecert("mysecret2");
    toWrite.setKeyType(SECRETDATABASE_KEYTYPE_BASE32);
    toWrite.setOtpType(SECRETDATABASE_OTPTYPE_HOTP);
    toWrite.setOutNumberCount(7);

    // Make sure the object appears to be valid.
    QVERIFY(toWrite.valid());

    // Attempt to write the entry to the database.
    QVERIFY(mTestDatabase.add(toWrite));

    // Read back what we just wrote.
    QVERIFY(mTestDatabase.getByIdentifier("id2", readBack));

    // Make sure the KeyEntry indicates it is valid.
    QVERIFY(readBack.valid());

    // And, make sure all the expected values are set.
    QCOMPARE(readBack.identifier(), "id2");
    QCOMPARE(readBack.secret(), "mysecret2");
    QCOMPARE(readBack.keyType(), SECRETDATABASE_KEYTYPE_BASE32);
    QCOMPARE(readBack.otpType(), SECRETDATABASE_OTPTYPE_HOTP);
    QCOMPARE(readBack.outNumberCount(), 7);
}

void SecretDatabaseTests::getAllEntriesTest()
{
    std::vector<KeyEntry> allEntries;
    KeyEntry currentEntry;

    // Attempt to read all of the entries.
    QVERIFY(mTestDatabase.getAll(allEntries));

    // There should be two entries in the database.
    QCOMPARE(allEntries.size(), 2);

    // Make sure they look how we expect them to.
    for (size_t i = 0; i < allEntries.size(); i++) {
        currentEntry = allEntries.at(i);

        // Make sure it indicates it is valid.
        QVERIFY(currentEntry.valid());

        if (currentEntry.identifier() == "id1") {
            // Make sure everything is set how we expect.
            QCOMPARE(currentEntry.secret(), "mysecret");
            QCOMPARE(currentEntry.keyType(), SECRETDATABASE_KEYTYPE_HEX);
            QCOMPARE(currentEntry.otpType(), SECRETDATABASE_OTPTYPE_TOTP);
            QCOMPARE(currentEntry.outNumberCount(), 6);
        } else if (currentEntry.identifier() == "id2") {
            QCOMPARE(currentEntry.secret(), "mysecret2");
            QCOMPARE(currentEntry.keyType(), SECRETDATABASE_KEYTYPE_BASE32);
            QCOMPARE(currentEntry.otpType(), SECRETDATABASE_OTPTYPE_HOTP);
            QCOMPARE(currentEntry.outNumberCount(), 7);
        } else {
            // Unexpected entry!
            QFAIL("Unexpected entry in the database!");
        }
    }
}

