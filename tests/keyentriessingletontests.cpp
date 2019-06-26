#include "keyentriessingletontests.h"

#include <QtTest>

#include "keyentriessingleton.h"
#include "keystorage/keyentry.h"
#include "settingshandler.h"
#include "testutils.h"

void KeyEntriesSingletonTests::e2eTests()
{
    QString dbPath;
    KeyEntry kEntry;
    KeyEntry newEntry;
    KeyEntry *foundKey = nullptr;
    QList<KeyEntry> allKeys;

    // If we have an old database file hanging around, delete it.
    dbPath = SettingsHandler::getInstance()->fullDatabasePathAndFilename();
    if (dbPath.isEmpty()) {
        QFAIL("The database path was empty!");
    }

    // See if the database exists.
    if (TestUtils::fileExists(dbPath.toStdString())) {
        // Delete it.
        QVERIFY(TestUtils::deleteFile(dbPath.toStdString()));
    }

    // Write a key entry to the database.
    QVERIFY(KeyEntriesSingleton::getInstance()->addKeyEntry("Test Key", "Test Issuer", "3132333435363738393031323334353637383930", KEYENTRY_KEYTYPE_HEX, KEYENTRY_OTPTYPE_HOTP, 6, KEYENTRY_ALG_SHA1, 30, 0));

    // Get the QML singleton to pick up the code coverage.
    QVERIFY(nullptr != KeyEntriesSingleton::getQmlSingleton(nullptr, nullptr));

    // Clear the key entry so we can load it back.
    kEntry.clear();

    // Find the key entry by identifier.
    foundKey = KeyEntriesSingleton::getInstance()->fromIdentifier("Test Key");
    if (nullptr == foundKey) {
        QFAIL("Unable to locate the 'Test Key'!");
        return;
    }

    // Make sure the secret is what we expect, which proves the data was written to the
    // database, and read back.
    QCOMPARE(std::string("3132333435363738393031323334353637383930"), foundKey->secret().toString());

    // Make sure there is only a single entry.
    QCOMPARE((int)1, KeyEntriesSingleton::getInstance()->count());

    // Get the entry at index 0, which should be the same as what we have.
    if (foundKey != KeyEntriesSingleton::getInstance()->at(0)) {
        QFAIL("Key entry at index 0 isn't what we expected!");
        return;
    }

    // Try to re-add the key (should fail).
    QVERIFY(!KeyEntriesSingleton::getInstance()->addKeyEntry("Test Key", "Test Issuer", "secret", 1, 1, 6, KEYENTRY_ALG_SHA1, 30, 0));

    // Change the secret, and update the database record.
    newEntry = (*foundKey);
    newEntry.setSecret("updatedsecret");
    QVERIFY(KeyEntriesSingleton::getInstance()->updateKeyEntry((*foundKey), newEntry));

    // Clear both key entries, and read back the updated value.
    foundKey = nullptr;
    newEntry.clear();

    // Find the key entry by identifier again.
    foundKey = KeyEntriesSingleton::getInstance()->fromIdentifier("Test Key");
    if (nullptr == foundKey) {
        QFAIL("Unable to locate the 'Test Key'!");
        return;
    }

    // Make sure the secret is the updated value.
    QCOMPARE(std::string("updatedsecret"), foundKey->secret().toString());

    // Verify that all of the other values in the key entry are what we expect.
    QCOMPARE(QString("Test Key"), foundKey->identifier());
    QCOMPARE(QString("Test Issuer"), foundKey->issuer());
    QCOMPARE(KEYENTRY_KEYTYPE_HEX, foundKey->keyType());
    QCOMPARE(KEYENTRY_OTPTYPE_HOTP, foundKey->otpType());
    QCOMPARE((unsigned int)30, foundKey->timeStep());
    QCOMPARE(KEYENTRY_ALG_SHA1, foundKey->algorithm());
    QVERIFY(foundKey->codeValid());
    QCOMPARE((unsigned int)0, foundKey->timeOffset());
    QVERIFY(!foundKey->currentCode().isEmpty());        // Should be empty.  Not calculated yet.
    QCOMPARE((unsigned int)0, foundKey->hotpCounter());
    QVERIFY(foundKey->invalidReason().isEmpty());      // Should be empty.  Data is all valid.
    QCOMPARE((unsigned int)6, foundKey->outNumberCount());
    QVERIFY(!foundKey->printableCurrentCode().isEmpty());   // Should be empty.  Not calculated.

    // Attempt to increment an invalid  HOTP counter.
    QVERIFY(!KeyEntriesSingleton::getInstance()->incrementHotpCounter(""));
    QVERIFY(!KeyEntriesSingleton::getInstance()->incrementHotpCounter("Invalid entry"));

    // Update the key entry, but use invalid inputs for each thing along the way.
    QVERIFY(!KeyEntriesSingleton::getInstance()->updateKeyEntry(nullptr, "", "", -1, -1, -1, -1, -1, -1));
    QVERIFY(!KeyEntriesSingleton::getInstance()->updateKeyEntry(foundKey, "", "", -1, -1, -1, -1, -1, -1));
    QVERIFY(!KeyEntriesSingleton::getInstance()->updateKeyEntry(foundKey, "New Id", "", -1, -1, 1, -1, -1, -1));
    QVERIFY(!KeyEntriesSingleton::getInstance()->updateKeyEntry(foundKey, "New Id", "New Secret", -1, -1, -1, -1, -1, -1));
    QVERIFY(!KeyEntriesSingleton::getInstance()->updateKeyEntry(foundKey, "New Id", "New Secret", KEYENTRY_KEYTYPE_BASE32, -1, -1, -1, -1, -1));
    QVERIFY(!KeyEntriesSingleton::getInstance()->updateKeyEntry(foundKey, "New Id", "New Secret", KEYENTRY_KEYTYPE_BASE32, KEYENTRY_OTPTYPE_HOTP, -1, -1, -1, -1));
    QVERIFY(!KeyEntriesSingleton::getInstance()->updateKeyEntry(foundKey, "New Id", "New Secret", KEYENTRY_KEYTYPE_BASE32, KEYENTRY_OTPTYPE_HOTP, 6, -1, -1, -1));
    QVERIFY(!KeyEntriesSingleton::getInstance()->updateKeyEntry(foundKey, "New Id", "New Secret", KEYENTRY_KEYTYPE_BASE32, KEYENTRY_OTPTYPE_HOTP, 6, KEYENTRY_ALG_SHA1, -1, -1));
    QVERIFY(!KeyEntriesSingleton::getInstance()->updateKeyEntry(foundKey, "New Id", "New Secret", KEYENTRY_KEYTYPE_BASE32, KEYENTRY_OTPTYPE_HOTP, 6, KEYENTRY_ALG_SHA1, 30, -1));

    // Clear the singleton.
    KeyEntriesSingleton::getInstance()->clear();

    // Try to delete an empty key identifier.
    QVERIFY(!KeyEntriesSingleton::getInstance()->deleteKeyEntry(""));

    // Then, attempt to delete the entry.
    QVERIFY(KeyEntriesSingleton::getInstance()->deleteKeyEntry("Test Key"));

    // Attempt to find it again, which should fail.
    foundKey = nullptr;
    foundKey = KeyEntriesSingleton::getInstance()->fromIdentifier("Test Key");
    if (nullptr != foundKey) {
        QFAIL("Located 'Test Key' when we shouldn't have!");
        return;
    }

    // Get all of the keys again.  The count should be 0.
    QCOMPARE((int)0, KeyEntriesSingleton::getInstance()->count());
}
