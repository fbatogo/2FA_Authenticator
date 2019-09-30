#include <testsuitebase.h>

#include "settingshandler.h"
#include <testutils.h>
#include "keystorage/keystorage.h"

SIMPLE_TEST_SUITE(KeyStorageTests, KeyStorage);

TEST_F(KeyStorageTests, E2ETests)
{
    KeyStorage storageTest;
    QString dbPath;
    KeyEntry kEntry;
    KeyEntry newEntry;
    QList<KeyEntry> allKeys;

    // Try to delete an entry when the database isn't open.
    EXPECT_TRUE(!storageTest.deleteKeyByIdentifier("Test Key"));

    EXPECT_TRUE(!storageTest.available());

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

    // Init the key storage.
    EXPECT_TRUE(storageTest.initStorage());

    // Build a key entry to write to the database.
    kEntry.clear();
    kEntry.setIdentifier("Test Key");
    kEntry.setIssuer("Test Issuer");
    kEntry.setSecret("secret");
    kEntry.setKeyType(1);
    kEntry.setOtpType(1);
    kEntry.setTimeStep(30);
    kEntry.setAlgorithm(1);
    kEntry.setCodeValid(true);
    kEntry.setStartTime(123);
    kEntry.setTimeOffset(456);
    kEntry.setCurrentCode("1234567");
    kEntry.setHotpCounter(9);
    kEntry.setOutNumberCount(8);
    kEntry.setPrintableCurrentCode("123 4567");

    // Make it invalid and try to write it.
    kEntry.setInvalidReason("Invalid");
    EXPECT_TRUE(!kEntry.valid());

    EXPECT_TRUE(!storageTest.addKey(kEntry));

    // Make it valid again.
    kEntry.setInvalidReason("");
    EXPECT_TRUE(kEntry.valid());

    // Write it to the database.
    EXPECT_TRUE(storageTest.addKey(kEntry));

    // Clear the key entry so we can load it back.
    kEntry.clear();

    // Find the key entry by identifier.
    EXPECT_TRUE(storageTest.keyByIdentifier("Test Key", kEntry));

    // Make sure the secret is what we expect, which proves the data was written to the
    // database, and read back.
    EXPECT_EQ(std::string("secret"), kEntry.secret().toString());

    // Attempt to get all the keys in the database.
    EXPECT_TRUE(storageTest.getAllKeys(allKeys));

    // Make sure there is only a single entry.
    EXPECT_EQ((int)1, allKeys.size());

    // Try to re-add the key (should fail).
    EXPECT_TRUE(!storageTest.addKey(kEntry));

    // Change the secret, and update the database record.
    newEntry = kEntry;
    newEntry.setSecret("updatedsecret");
    EXPECT_TRUE(storageTest.updateKey(kEntry, newEntry));

    // Make one of the entries invalid, and try to update it again.
    newEntry.setInvalidReason("Invalid");
    EXPECT_TRUE(!newEntry.valid());
    EXPECT_TRUE(!storageTest.updateKey(kEntry, newEntry));

    // Clear both key entries, and read back the updated value.
    newEntry.clear();
    kEntry.clear();

    EXPECT_TRUE(storageTest.keyByIdentifier("Test Key", kEntry));

    // Make sure the secret is the updated value.
    EXPECT_EQ(std::string("updatedsecret"), kEntry.secret().toString());

    // Verify that all of the other values in the key entry are what we expect.
    EXPECT_EQ(QString("Test Key"), kEntry.identifier());
    EXPECT_EQ(QString("Test Issuer"), kEntry.issuer());
    EXPECT_EQ((unsigned int)1, kEntry.keyType());
    EXPECT_EQ((unsigned int)1, kEntry.otpType());
    EXPECT_EQ((unsigned int)30, kEntry.timeStep());
    EXPECT_EQ((unsigned int)1, kEntry.algorithm());
    EXPECT_TRUE(!kEntry.codeValid());
    EXPECT_EQ((unsigned int)0, kEntry.startTime());
    EXPECT_EQ((unsigned int)456, kEntry.timeOffset());
    EXPECT_TRUE(kEntry.currentCode().isEmpty());        // Should be empty.  Not calculated yet.
    EXPECT_EQ((unsigned int)9, kEntry.hotpCounter());
    EXPECT_TRUE(kEntry.invalidReason().isEmpty());      // Should be empty.  Data is all valid.
    EXPECT_EQ((unsigned int)8, kEntry.outNumberCount());
    EXPECT_TRUE(kEntry.printableCurrentCode().isEmpty());   // Should be empty.  Not calculated.

    // Then, attempt to delete the entry.
    EXPECT_TRUE(storageTest.deleteKeyByIdentifier("Test Key"));

    // Attempt to find it again, which should fail.
    kEntry.clear();
    EXPECT_TRUE(!storageTest.keyByIdentifier("Test Key", kEntry));

    // Get all of the keys again.  The count should be 0.
    EXPECT_TRUE(storageTest.getAllKeys(allKeys));
    EXPECT_EQ((int)0, allKeys.size());
}
