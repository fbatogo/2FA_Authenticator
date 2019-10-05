#include <testsuitebase.h>

#include "keyentriessingleton.h"
#include "keystorage/keyentry.h"
#include "settingshandler.h"
#include "testutils.h"

EMPTY_TEST_SUITE(KeyEntriesSingletonTests);

TEST_F(KeyEntriesSingletonTests, E2ETests)
{
    QString dbPath;
    KeyEntry kEntry;
    KeyEntry newEntry;
    KeyEntry *foundKey = nullptr;

    // If we have an old database file hanging around, delete it.
    dbPath = SettingsHandler::getInstance()->fullDatabasePathAndFilename();
    if (dbPath.isEmpty()) {
        FAIL() << "The database path was empty!";
    }

    // See if the database exists.
    if (TestUtils::fileExists(dbPath.toStdString())) {
        // Delete it.
        EXPECT_TRUE(TestUtils::deleteFile(dbPath.toStdString()));
    }

    // Make sure the database is open.
    EXPECT_TRUE(KeyEntriesSingleton::getInstance()->open());

    // Write a key entry to the database.
    EXPECT_TRUE(KeyEntriesSingleton::getInstance()->addKeyEntry("Test Key", "Test Issuer", "3132333435363738393031323334353637383930", KEYENTRY_KEYTYPE_HEX, KEYENTRY_OTPTYPE_HOTP, 6, KEYENTRY_ALG_SHA1, 30, 0));

    // Get the QML singleton to pick up the code coverage.
    EXPECT_TRUE(nullptr != KeyEntriesSingleton::getQmlSingleton(nullptr, nullptr));

    // Clear the key entry so we can load it back.
    kEntry.clear();

    // Find the key entry by identifier.
    foundKey = KeyEntriesSingleton::getInstance()->fromIdentifier("Test Key");
    if (nullptr == foundKey) {
        FAIL() << "Unable to locate the 'Test Key'!";
        return;
    }

    // Make sure the secret is what we expect, which proves the data was written to the
    // database, and read back.
    EXPECT_EQ(std::string("3132333435363738393031323334353637383930"), foundKey->secret().toString());

    // Make sure there is only a single entry.
    EXPECT_EQ((int)1, KeyEntriesSingleton::getInstance()->count());

    // Get the entry at index 0, which should be the same as what we have.
    if (foundKey != KeyEntriesSingleton::getInstance()->at(0)) {
        FAIL() << "Key entry at index 0 isn't what we expected!";
        return;
    }

    // Try to re-add the key (should fail).
    EXPECT_TRUE(!KeyEntriesSingleton::getInstance()->addKeyEntry("Test Key", "Test Issuer", "secret", 1, 1, 6, KEYENTRY_ALG_SHA1, 30, 0));

    // Change the secret, and update the database record.
    newEntry = (*foundKey);
    newEntry.setSecret("updatedsecret");
    EXPECT_TRUE(KeyEntriesSingleton::getInstance()->updateKeyEntry((*foundKey), newEntry));

    // Clear both key entries, and read back the updated value.
    foundKey = nullptr;
    newEntry.clear();

    // Find the key entry by identifier again.
    foundKey = KeyEntriesSingleton::getInstance()->fromIdentifier("Test Key");
    if (nullptr == foundKey) {
        FAIL() << "Unable to locate the 'Test Key'!";
        return;
    }

    // Make sure the secret is the updated value.
    EXPECT_EQ(std::string("updatedsecret"), foundKey->secret().toString());

    // Verify that all of the other values in the key entry are what we expect.
    EXPECT_EQ(QString("Test Key"), foundKey->identifier());
    EXPECT_EQ(QString("Test Issuer"), foundKey->issuer());
    EXPECT_EQ(KEYENTRY_KEYTYPE_HEX, foundKey->keyType());
    EXPECT_EQ(KEYENTRY_OTPTYPE_HOTP, foundKey->otpType());
    EXPECT_EQ((unsigned int)30, foundKey->timeStep());
    EXPECT_EQ(KEYENTRY_ALG_SHA1, foundKey->algorithm());
    EXPECT_TRUE(foundKey->codeValid());
    EXPECT_EQ((unsigned int)0, foundKey->timeOffset());
    EXPECT_TRUE(!foundKey->currentCode().isEmpty());        // Should be empty.  Not calculated yet.
    EXPECT_EQ((unsigned int)0, foundKey->hotpCounter());
    EXPECT_TRUE(foundKey->invalidReason().isEmpty());      // Should be empty.  Data is all valid.
    EXPECT_EQ((unsigned int)6, foundKey->outNumberCount());
    EXPECT_TRUE(!foundKey->printableCurrentCode().isEmpty());   // Should be empty.  Not calculated.

    // Attempt to increment an invalid  HOTP counter.
    EXPECT_TRUE(!KeyEntriesSingleton::getInstance()->incrementHotpCounter(""));
    EXPECT_TRUE(!KeyEntriesSingleton::getInstance()->incrementHotpCounter("Invalid entry"));

    // Update the key entry, but use invalid inputs for each thing along the way.
    EXPECT_TRUE(!KeyEntriesSingleton::getInstance()->updateKeyEntry(nullptr, "", "", -1, -1, -1, -1, -1, -1));
    EXPECT_TRUE(!KeyEntriesSingleton::getInstance()->updateKeyEntry(foundKey, "", "", -1, -1, -1, -1, -1, -1));
    EXPECT_TRUE(!KeyEntriesSingleton::getInstance()->updateKeyEntry(foundKey, "New Id", "", -1, -1, 1, -1, -1, -1));
    EXPECT_TRUE(!KeyEntriesSingleton::getInstance()->updateKeyEntry(foundKey, "New Id", "New Secret", -1, -1, -1, -1, -1, -1));
    EXPECT_TRUE(!KeyEntriesSingleton::getInstance()->updateKeyEntry(foundKey, "New Id", "New Secret", KEYENTRY_KEYTYPE_BASE32, -1, -1, -1, -1, -1));
    EXPECT_TRUE(!KeyEntriesSingleton::getInstance()->updateKeyEntry(foundKey, "New Id", "New Secret", KEYENTRY_KEYTYPE_BASE32, KEYENTRY_OTPTYPE_HOTP, -1, -1, -1, -1));
    EXPECT_TRUE(!KeyEntriesSingleton::getInstance()->updateKeyEntry(foundKey, "New Id", "New Secret", KEYENTRY_KEYTYPE_BASE32, KEYENTRY_OTPTYPE_HOTP, 6, -1, -1, -1));
    EXPECT_TRUE(!KeyEntriesSingleton::getInstance()->updateKeyEntry(foundKey, "New Id", "New Secret", KEYENTRY_KEYTYPE_BASE32, KEYENTRY_OTPTYPE_HOTP, 6, KEYENTRY_ALG_SHA1, -1, -1));
    EXPECT_TRUE(!KeyEntriesSingleton::getInstance()->updateKeyEntry(foundKey, "New Id", "New Secret", KEYENTRY_KEYTYPE_BASE32, KEYENTRY_OTPTYPE_HOTP, 6, KEYENTRY_ALG_SHA1, 30, -1));

    // Clear the singleton.
    KeyEntriesSingleton::getInstance()->clear();

    // Try to delete an empty key identifier.
    EXPECT_TRUE(!KeyEntriesSingleton::getInstance()->deleteKeyEntry(""));

    // Then, attempt to delete the entry.
    EXPECT_TRUE(KeyEntriesSingleton::getInstance()->deleteKeyEntry("Test Key"));

    // Attempt to find it again, which should fail.
    foundKey = nullptr;
    foundKey = KeyEntriesSingleton::getInstance()->fromIdentifier("Test Key");
    if (nullptr != foundKey) {
        FAIL() << "Located 'Test Key' when we shouldn't have!";
        return;
    }

    // Get all of the keys again.  The count should be 0.
    EXPECT_EQ((int)0, KeyEntriesSingleton::getInstance()->count());
}

TEST_F(KeyEntriesSingletonTests, OpenCloseTests)
{
    KeyEntry *testEntry;

    // Make sure the key database is open to start with.
    KeyEntriesSingleton::getInstance()->open();

    // Make sure our test entries don't exist.  Ignore the result value because a "false"
    // return value should only indicate that they don't already exist.
    KeyEntriesSingleton::getInstance()->deleteKeyEntry("Open/Close Test");
    KeyEntriesSingleton::getInstance()->deleteKeyEntry("Open/Close Test 2");

    // Try to add a key.
    EXPECT_TRUE(KeyEntriesSingleton::getInstance()->addKeyEntry("Open/Close Test", "Open/Close Test Issuer", "3332333435363738393031323334353637383930", KEYENTRY_KEYTYPE_HEX, KEYENTRY_OTPTYPE_HOTP, 6, KEYENTRY_ALG_SHA256, 30, 0));

    // And, we should be able to read it back.
    testEntry = KeyEntriesSingleton::getInstance()->fromIdentifier("Open/Close Test");

    // And, the resulting entry shouldn't be null.
    EXPECT_TRUE(nullptr != testEntry);

    // Make sure the issuer name for the returned object is what we expect.
    EXPECT_EQ(testEntry->issuer(), QString("Open/Close Test Issuer"));

    // Then, close the key entry backing store.
    EXPECT_TRUE(KeyEntriesSingleton::getInstance()->close());

    EXPECT_TRUE(!KeyEntriesSingleton::getInstance()->isOpen());

    // Try to add another key entry.  (Should fail.)
    EXPECT_TRUE(!KeyEntriesSingleton::getInstance()->addKeyEntry("Open/Close Test 2", "Open/Close Test Issuer 2", "3332333435363738393031323334353637383930", KEYENTRY_KEYTYPE_HEX, KEYENTRY_OTPTYPE_HOTP, 6, KEYENTRY_ALG_SHA256, 30, 0));

    // Try to read it back (should return nullptr)
    testEntry = KeyEntriesSingleton::getInstance()->fromIdentifier("Open/Close Test 2");

    EXPECT_TRUE(nullptr == testEntry);

    // Then, reopen the backing store.
    EXPECT_TRUE(KeyEntriesSingleton::getInstance()->open());

    // Attempt to read the key entry that we wrote while the store was closed.  (Should return nullptr.)
    testEntry = KeyEntriesSingleton::getInstance()->fromIdentifier("Open/Close Test 2");

    EXPECT_TRUE(nullptr == testEntry);


    // Now, attempt to add in the entry we tried to add while the store was closed.
    EXPECT_TRUE(KeyEntriesSingleton::getInstance()->addKeyEntry("Open/Close Test 2", "Open/Close Test Issuer 2", "3332333435363738393031323334353637383930", KEYENTRY_KEYTYPE_HEX, KEYENTRY_OTPTYPE_HOTP, 6, KEYENTRY_ALG_SHA256, 30, 0));

    // Then, read it back and verify the issuer is what we expect.
    testEntry = KeyEntriesSingleton::getInstance()->fromIdentifier("Open/Close Test 2");

    // And, the resulting entry shouldn't be null.
    EXPECT_TRUE(nullptr != testEntry);

    // Make sure the issuer name for the returned object is what we expect.
    EXPECT_EQ(testEntry->issuer(), QString("Open/Close Test Issuer 2"));

    // Clean up the entries so later tests don't have to worry about them.
    EXPECT_TRUE(KeyEntriesSingleton::getInstance()->deleteKeyEntry("Open/Close Test"));
    EXPECT_TRUE(KeyEntriesSingleton::getInstance()->deleteKeyEntry("Open/Close Test 2"));
}
