#include <testsuitebase.h>

#include "keystorage/database/secretdatabase.h"
#include "keystorage/keyentry.h"
#include "logger.h"

#include <QFileInfo>

#include <iostream>

#define TEST_DB "test.db"

class SecretDatabaseTests : public TestSuiteBase, public SecretDatabase {
protected:
    SecretDatabaseTests() {};

    void SetUp() {
        Logger::getInstance()->log("SecretDatabaseTests Setup called!");
        QFileInfo testFile(TEST_DB);

        // Make sure we can open the database.
        ASSERT_TRUE(open(TEST_DB));

        // Make sure the file exists now.
        ASSERT_TRUE(testFile.exists());
        ASSERT_TRUE(testFile.isFile());

        // Check that our schema version is what we expect.
        ASSERT_EQ(schemaVersion(), 1);
    }

    void TearDown() {
        Logger::getInstance()->log("SecretDatabaseTests TearDown called!");
        EXPECT_TRUE(close());

        // And, remove the test database file.
        EXPECT_TRUE(QFile(TEST_DB).remove());
    }
};

TEST_F(SecretDatabaseTests, AddDatabaseEntryKeyEntryTest)
{
    KeyEntry toWrite;
    KeyEntry readBack;

    // Make sure our toWrite value is invalid to start with.
    EXPECT_EQ(toWrite.valid(), false);

    // Try to add an invalid entry.
    EXPECT_TRUE(!add(toWrite));

    // Build the secret entry that we want to write.
    toWrite.setIdentifier("id2");
    toWrite.setSecret("mysecret2");
    toWrite.setKeyType(KEYENTRY_KEYTYPE_BASE32);
    toWrite.setOtpType(KEYENTRY_OTPTYPE_HOTP);
    toWrite.setOutNumberCount(7);

    // Make sure the object appears to be valid.
    EXPECT_TRUE(toWrite.valid());

    // Attempt to write the entry to the database.
    EXPECT_TRUE(add(toWrite));

    // Read back what we just wrote.
    EXPECT_TRUE(getByIdentifier("id2", readBack));

    // Make sure the KeyEntry indicates it is valid.
    EXPECT_TRUE(readBack.valid());

    // And, make sure all the expected values are set.
    EXPECT_EQ(readBack.identifier(), QString("id2"));
    EXPECT_EQ(readBack.secret().toString(), std::string("mysecret2"));
    EXPECT_EQ(readBack.keyType(), KEYENTRY_KEYTYPE_BASE32);
    EXPECT_EQ(readBack.otpType(), KEYENTRY_OTPTYPE_HOTP);
    EXPECT_EQ(readBack.outNumberCount(), (unsigned int)7);
}

TEST_F(SecretDatabaseTests, GetAllEntriesTest)
{
    std::vector<KeyEntry> allEntries;
    KeyEntry currentEntry;

    // Attempt to read all of the entries.
    EXPECT_TRUE(getAll(allEntries));

    // There should be two entries in the database.
    EXPECT_EQ(allEntries.size(), static_cast<size_t>(1));

    // Make sure they look how we expect them to.
    for (unsigned int i = 0; i < allEntries.size(); i++) {
        currentEntry = allEntries.at(i);

        // Make sure it indicates it is valid.
        EXPECT_TRUE(currentEntry.valid());

        if (currentEntry.identifier() == "id2") {
            EXPECT_EQ(currentEntry.secret().toString(), std::string("mysecret2"));
            EXPECT_EQ(currentEntry.keyType(), KEYENTRY_KEYTYPE_BASE32);
            EXPECT_EQ(currentEntry.otpType(), KEYENTRY_OTPTYPE_HOTP);
            EXPECT_EQ(currentEntry.outNumberCount(), (unsigned int)7);
        } else {
            // Unexpected entry!
            FAIL() << "Unexpected entry in the database!";
        }
    }
}

TEST_F(SecretDatabaseTests, UpdateDatabaseEntryTest)
{
    KeyEntry readBack;
    KeyEntry newEntry;

    // try to get an empty identifier.
    EXPECT_TRUE(!getByIdentifier("", readBack));

    // Find the id2 entry in the database.
    EXPECT_TRUE(getByIdentifier("id2", readBack));

    // Make sure the data is what we expect.
    EXPECT_EQ(readBack.identifier(), QString("id2"));
    EXPECT_EQ(readBack.secret().toString(), std::string("mysecret2"));
    EXPECT_EQ(readBack.keyType(), KEYENTRY_KEYTYPE_BASE32);
    EXPECT_EQ(readBack.otpType(), KEYENTRY_OTPTYPE_HOTP);
    EXPECT_EQ(readBack.outNumberCount(), (unsigned int)7);

    // Copy the data, and update the identifier name.
    newEntry = readBack;

    newEntry.setIdentifier("id3");

    EXPECT_TRUE(update(readBack, newEntry));

    // Then, attempt to locate the record that should now have an identifier of "id3".
    EXPECT_TRUE(getByIdentifier("id3", readBack));

    // Make sure the data is what we expect.
    EXPECT_EQ(readBack.identifier(), QString("id3"));
    EXPECT_EQ(readBack.secret().toString(), std::string("mysecret2"));
    EXPECT_EQ(readBack.keyType(), KEYENTRY_KEYTYPE_BASE32);
    EXPECT_EQ(readBack.otpType(), KEYENTRY_OTPTYPE_HOTP);
    EXPECT_EQ(readBack.outNumberCount(), (unsigned int)7);

    // Then copy the data and update all of the values.
    newEntry = readBack;

    newEntry.setIdentifier("id4");
    newEntry.setSecret("mysecret4");
    newEntry.setKeyType(KEYENTRY_KEYTYPE_HEX);
    newEntry.setOtpType(KEYENTRY_OTPTYPE_TOTP);
    newEntry.setOutNumberCount(6);

    EXPECT_TRUE(update(readBack, newEntry));

    // Then, attempt to locate the newly updated record.
    EXPECT_TRUE(getByIdentifier("id4", readBack));

    // Make sure the data is what we expect.
    EXPECT_EQ(readBack.identifier(), QString("id4"));
    EXPECT_EQ(readBack.secret().toString(), std::string("mysecret4"));
    EXPECT_EQ(readBack.keyType(), KEYENTRY_KEYTYPE_HEX);
    EXPECT_EQ(readBack.otpType(), KEYENTRY_OTPTYPE_TOTP);
    EXPECT_EQ(readBack.outNumberCount(), (unsigned int)6);
}

TEST_F(SecretDatabaseTests, DeleteDatabaseEntryTest)
{
    // Attempt to delete an entry that doesn't exist.
    EXPECT_TRUE(!deleteByIdentifier("Invalid Identifier"));
}

TEST_F(SecretDatabaseTests, MiscTests)
{
    // Copy ctor test.
    SecretDatabase copiedDb(*this);

    // Attempt to assign a SecretDatabase to itself.
    copiedDb = copiedDb;

    // Assign from a different SecretDatabase to another value.
    copiedDb = (*this);
}
