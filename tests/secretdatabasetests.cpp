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

    // And the KeyEntry version.
    addDatabaseEntryKeyEntryTest();

    // Try getting all of the entries.
    getAllEntriesTest();

    // Test updating entries in the database.
    updateDatabaseEntryTest();

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

void SecretDatabaseTests::addDatabaseEntryKeyEntryTest()
{
    KeyEntry toWrite;
    KeyEntry readBack;

    // Make sure our toWrite value is invalid to start with.
    QCOMPARE(toWrite.valid(), false);

    // Build the secret entry that we want to write.
    toWrite.setIdentifier("id2");
    toWrite.setSecert("mysecret2");
    toWrite.setKeyType(KEYENTRY_KEYTYPE_BASE32);
    toWrite.setOtpType(KEYENTRY_OTPTYPE_HOTP);
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
    QCOMPARE(readBack.keyType(), KEYENTRY_KEYTYPE_BASE32);
    QCOMPARE(readBack.otpType(), KEYENTRY_OTPTYPE_HOTP);
    QCOMPARE(readBack.outNumberCount(), 7);
}

void SecretDatabaseTests::getAllEntriesTest()
{
    std::vector<KeyEntry> allEntries;
    KeyEntry currentEntry;

    // Attempt to read all of the entries.
    QVERIFY(mTestDatabase.getAll(allEntries));

    // There should be two entries in the database.
    QCOMPARE(allEntries.size(), 1);

    // Make sure they look how we expect them to.
    for (size_t i = 0; i < allEntries.size(); i++) {
        currentEntry = allEntries.at(i);

        // Make sure it indicates it is valid.
        QVERIFY(currentEntry.valid());

        if (currentEntry.identifier() == "id2") {
            QCOMPARE(currentEntry.secret(), "mysecret2");
            QCOMPARE(currentEntry.keyType(), KEYENTRY_KEYTYPE_BASE32);
            QCOMPARE(currentEntry.otpType(), KEYENTRY_OTPTYPE_HOTP);
            QCOMPARE(currentEntry.outNumberCount(), 7);
        } else {
            // Unexpected entry!
            QFAIL("Unexpected entry in the database!");
        }
    }
}

void SecretDatabaseTests::updateDatabaseEntryTest()
{
    KeyEntry readBack;
    KeyEntry newEntry;

    // Find the id2 entry in the database.
    QVERIFY(mTestDatabase.getByIdentifier("id2", readBack));

    // Make sure the data is what we expect.
    QCOMPARE(readBack.identifier(), "id2");
    QCOMPARE(readBack.secret(), "mysecret2");
    QCOMPARE(readBack.keyType(), KEYENTRY_KEYTYPE_BASE32);
    QCOMPARE(readBack.otpType(), KEYENTRY_OTPTYPE_HOTP);
    QCOMPARE(readBack.outNumberCount(), 7);

    // Copy the data, and update the identifier name.
    newEntry = readBack;

    newEntry.setIdentifier("id3");

    QVERIFY(mTestDatabase.update(readBack, newEntry));

    // Then, attempt to locate the record that should now have an identifier of "id3".
    QVERIFY(mTestDatabase.getByIdentifier("id3", readBack));

    // Make sure the data is what we expect.
    QCOMPARE(readBack.identifier(), "id3");
    QCOMPARE(readBack.secret(), "mysecret2");
    QCOMPARE(readBack.keyType(), KEYENTRY_KEYTYPE_BASE32);
    QCOMPARE(readBack.otpType(), KEYENTRY_OTPTYPE_HOTP);
    QCOMPARE(readBack.outNumberCount(), 7);

    // Then copy the data and update all of the values.
    newEntry = readBack;

    newEntry.setIdentifier("id4");
    newEntry.setSecert("mysecret4");
    newEntry.setKeyType(KEYENTRY_KEYTYPE_HEX);
    newEntry.setOtpType(KEYENTRY_OTPTYPE_TOTP);
    newEntry.setOutNumberCount(6);

    QVERIFY(mTestDatabase.update(readBack, newEntry));

    // Then, attempt to locate the newly updated record.
    QVERIFY(mTestDatabase.getByIdentifier("id4", readBack));

    // Make sure the data is what we expect.
    QCOMPARE(readBack.identifier(), "id4");
    QCOMPARE(readBack.secret(), "mysecret4");
    QCOMPARE(readBack.keyType(), KEYENTRY_KEYTYPE_HEX);
    QCOMPARE(readBack.otpType(), KEYENTRY_OTPTYPE_TOTP);
    QCOMPARE(readBack.outNumberCount(), 6);
}

