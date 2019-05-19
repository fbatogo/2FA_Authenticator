#include "secretdatabasetests.h"

#include <QTest>
#include <QFileInfo>
#include "../keystorage/keyentry.h"

#define TEST_DB "test.db"

// Make sure we have a database open and ready to go.
void SecretDatabaseTests::initTestCase()
{
    QFileInfo testFile(TEST_DB);

    // Make sure we can open the database.
    QVERIFY(testDatabase.open(TEST_DB));

    // Make sure the file exists now.
    QVERIFY(testFile.exists());
    QVERIFY(testFile.isFile());

    // Check that our schema version is what we expect.
    QCOMPARE(testDatabase.schemaVersion(), 1);
}

void SecretDatabaseTests::cleanupTestCase()
{
    QVERIFY(testDatabase.close());

    // And, remove the test database file.
    QVERIFY(QFile(TEST_DB).remove());
}

void SecretDatabaseTests::addDatabaseEntryKeyEntryTest()
{
    KeyEntry toWrite;
    KeyEntry readBack;

    // Make sure our toWrite value is invalid to start with.
    QCOMPARE(toWrite.valid(), false);

    // Try to add an invalid entry.
    QVERIFY(!testDatabase.add(toWrite));

    // Build the secret entry that we want to write.
    toWrite.setIdentifier("id2");
    toWrite.setSecret("mysecret2");
    toWrite.setKeyType(KEYENTRY_KEYTYPE_BASE32);
    toWrite.setOtpType(KEYENTRY_OTPTYPE_HOTP);
    toWrite.setOutNumberCount(7);

    // Make sure the object appears to be valid.
    QVERIFY(toWrite.valid());

    // Attempt to write the entry to the database.
    QVERIFY(testDatabase.add(toWrite));

    // Read back what we just wrote.
    QVERIFY(testDatabase.getByIdentifier("id2", readBack));

    // Make sure the KeyEntry indicates it is valid.
    QVERIFY(readBack.valid());

    // And, make sure all the expected values are set.
    QCOMPARE(readBack.identifier(), QString("id2"));
    QCOMPARE(readBack.secret().toString(), std::string("mysecret2"));
    QCOMPARE(readBack.keyType(), KEYENTRY_KEYTYPE_BASE32);
    QCOMPARE(readBack.otpType(), KEYENTRY_OTPTYPE_HOTP);
    QCOMPARE(readBack.outNumberCount(), (size_t)7);
}

void SecretDatabaseTests::getAllEntriesTest()
{
    std::vector<KeyEntry> allEntries;
    KeyEntry currentEntry;

    // Attempt to read all of the entries.
    QVERIFY(testDatabase.getAll(allEntries));

    // There should be two entries in the database.
    QCOMPARE(allEntries.size(), static_cast<size_t>(1));

    // Make sure they look how we expect them to.
    for (size_t i = 0; i < allEntries.size(); i++) {
        currentEntry = allEntries.at(i);

        // Make sure it indicates it is valid.
        QVERIFY(currentEntry.valid());

        if (currentEntry.identifier() == "id2") {
            QCOMPARE(currentEntry.secret().toString(), std::string("mysecret2"));
            QCOMPARE(currentEntry.keyType(), KEYENTRY_KEYTYPE_BASE32);
            QCOMPARE(currentEntry.otpType(), KEYENTRY_OTPTYPE_HOTP);
            QCOMPARE(currentEntry.outNumberCount(), (size_t)7);
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

    // try to get an empty identifier.
    QVERIFY(!testDatabase.getByIdentifier("", readBack));

    // Find the id2 entry in the database.
    QVERIFY(testDatabase.getByIdentifier("id2", readBack));

    // Make sure the data is what we expect.
    QCOMPARE(readBack.identifier(), QString("id2"));
    QCOMPARE(readBack.secret().toString(), std::string("mysecret2"));
    QCOMPARE(readBack.keyType(), KEYENTRY_KEYTYPE_BASE32);
    QCOMPARE(readBack.otpType(), KEYENTRY_OTPTYPE_HOTP);
    QCOMPARE(readBack.outNumberCount(), (size_t)7);

    // Copy the data, and update the identifier name.
    newEntry = readBack;

    newEntry.setIdentifier("id3");

    QVERIFY(testDatabase.update(readBack, newEntry));

    // Then, attempt to locate the record that should now have an identifier of "id3".
    QVERIFY(testDatabase.getByIdentifier("id3", readBack));

    // Make sure the data is what we expect.
    QCOMPARE(readBack.identifier(), QString("id3"));
    QCOMPARE(readBack.secret().toString(), std::string("mysecret2"));
    QCOMPARE(readBack.keyType(), KEYENTRY_KEYTYPE_BASE32);
    QCOMPARE(readBack.otpType(), KEYENTRY_OTPTYPE_HOTP);
    QCOMPARE(readBack.outNumberCount(), (size_t)7);

    // Then copy the data and update all of the values.
    newEntry = readBack;

    newEntry.setIdentifier("id4");
    newEntry.setSecret("mysecret4");
    newEntry.setKeyType(KEYENTRY_KEYTYPE_HEX);
    newEntry.setOtpType(KEYENTRY_OTPTYPE_TOTP);
    newEntry.setOutNumberCount(6);

    QVERIFY(testDatabase.update(readBack, newEntry));

    // Then, attempt to locate the newly updated record.
    QVERIFY(testDatabase.getByIdentifier("id4", readBack));

    // Make sure the data is what we expect.
    QCOMPARE(readBack.identifier(), QString("id4"));
    QCOMPARE(readBack.secret().toString(), std::string("mysecret4"));
    QCOMPARE(readBack.keyType(), KEYENTRY_KEYTYPE_HEX);
    QCOMPARE(readBack.otpType(), KEYENTRY_OTPTYPE_TOTP);
    QCOMPARE(readBack.outNumberCount(), (size_t)6);
}

void SecretDatabaseTests::deleteDatabaseEntryTest()
{
    // Attempt to delete an entry that doesn't exist.
    QVERIFY(!testDatabase.deleteByIdentifier("Invalid Identifier"));
}

void SecretDatabaseTests::miscTests()
{
    // Copy ctor test.
    SecretDatabase copiedDb(testDatabase);

    // Attempt to assign a SecretDatabase to itself.
    copiedDb = copiedDb;

    // Assign from a different SecretDatabase to another value.
    copiedDb = testDatabase;
}
