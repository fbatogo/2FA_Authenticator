#include "settingshandlertests.h"

#include <QtTest>
#include "../settingshandler.h"

void SettingsHandlerTests::getSetSettingsTests()
{
    QVERIFY(nullptr != SettingsHandler::getQmlSingleton(nullptr, nullptr));

    QVERIFY(SettingsHandler::getInstance()->dataDirectoryExistsOrIsCreated());
    QVERIFY(!SettingsHandler::getInstance()->dataPath().isEmpty());

    // Set (most) values to their defaults.
    SettingsHandler::getInstance()->setShowIssuer(false);
    SettingsHandler::getInstance()->setLogToFile(false);
    SettingsHandler::getInstance()->setShowHashAlgorithm(false);
    SettingsHandler::getInstance()->setShowHotpCounterValue(false);

    // Read them back.
    QVERIFY(!SettingsHandler::getInstance()->showIssuer());
    QVERIFY(!SettingsHandler::getInstance()->logToFile());
    QVERIFY(!SettingsHandler::getInstance()->showHashAlgorithm());
    QVERIFY(!SettingsHandler::getInstance()->showHotpCounterValue());

    // Set them all to a new value.
    SettingsHandler::getInstance()->setShowIssuer(true);
    SettingsHandler::getInstance()->setLogToFile(true);
    SettingsHandler::getInstance()->setShowHashAlgorithm(true);
    SettingsHandler::getInstance()->setShowHotpCounterValue(true);

    // Read them back.
    QVERIFY(SettingsHandler::getInstance()->showIssuer());
    QVERIFY(SettingsHandler::getInstance()->logToFile());
    QVERIFY(SettingsHandler::getInstance()->showHashAlgorithm());
    QVERIFY(SettingsHandler::getInstance()->showHotpCounterValue());

    // Set values back to their defaults.
    SettingsHandler::getInstance()->setShowIssuer(false);
    SettingsHandler::getInstance()->setLogToFile(false);
    SettingsHandler::getInstance()->setShowHashAlgorithm(false);
    SettingsHandler::getInstance()->setShowHotpCounterValue(false);

    // Read them back.
    QVERIFY(!SettingsHandler::getInstance()->showIssuer());
    QVERIFY(!SettingsHandler::getInstance()->logToFile());
    QVERIFY(!SettingsHandler::getInstance()->showHashAlgorithm());
    QVERIFY(!SettingsHandler::getInstance()->showHotpCounterValue());
    QVERIFY(!SettingsHandler::getInstance()->databaseLocation().contains("dbtest"));
}

void SettingsHandlerTests::databasePathLocationTests()
{
    QString oldLocation;

    // Set the db location to the default.
    QVERIFY(SettingsHandler::getInstance()->setDatabaseLocation(""));

    // Read the setting back, and store it for later use.
    QVERIFY(SettingsHandler::getInstance()->databaseLocation().endsWith(".Rollin"));

    // Store the full path and file name for later use.
    oldLocation = SettingsHandler::getInstance()->fullDatabasePathAndFilename();

    // Make sure the database exists at that location.
    QVERIFY(QFile(SettingsHandler::getInstance()->fullDatabasePathAndFilename()).exists());

    // Change the path to a new location.
    QVERIFY(SettingsHandler::getInstance()->setDatabaseLocation("dbtest"));   // A relative directory called "dbtest".

    // Read the value back.
    QCOMPARE(SettingsHandler::getInstance()->databaseLocation(), "dbtest");

    // Make sure the database file was moved to that location...
    QVERIFY(QFile(SettingsHandler::getInstance()->fullDatabasePathAndFilename()).exists());

    // ... and that it no longer exists at the old location.
    QVERIFY(!QFile(oldLocation).exists());

    // Store the full path and file name for later use.
    oldLocation = SettingsHandler::getInstance()->fullDatabasePathAndFilename();

    // Move the database file back to the default location.
    QVERIFY(SettingsHandler::getInstance()->setDatabaseLocation(""));

    // Read the setting back.
    QVERIFY(!SettingsHandler::getInstance()->databaseLocation().endsWith(".Rollin"));

    // Make sure the database exists at that location...
    QVERIFY(QFile(SettingsHandler::getInstance()->fullDatabasePathAndFilename()).exists());

    // ... and that it no longer exists at the old location.
    QVERIFY(!QFile(oldLocation).exists());
}

void SettingsHandlerTests::databaseFileLocationTests()
{
    QString oldLocation;

    // Set the db file name to the default.
    QVERIFY(SettingsHandler::getInstance()->setDatabaseFilename(""));

    // Read the setting back, and store it for later use.
    QVERIFY(SettingsHandler::getInstance()->databaseFilename() == "keydatabase.db");

    // Store the full path and file name for later use.
    oldLocation = SettingsHandler::getInstance()->fullDatabasePathAndFilename();

    // Make sure the database exists at that location.
    QVERIFY(QFile(SettingsHandler::getInstance()->fullDatabasePathAndFilename()).exists());

    // Change the filename.
    QVERIFY(SettingsHandler::getInstance()->setDatabaseFilename("unittest.db"));

    // Read the value back.
    QCOMPARE(SettingsHandler::getInstance()->databaseFilename(), "unittest.db");

    // Make sure the database file was moved to that location...
    QVERIFY(QFile(SettingsHandler::getInstance()->fullDatabasePathAndFilename()).exists());

    // ... and that it no longer exists at the old location.
    QVERIFY(!QFile(oldLocation).exists());

    // Store the full path and file name for later use.
    oldLocation = SettingsHandler::getInstance()->fullDatabasePathAndFilename();

    // Move the database file back to the default location.
    QVERIFY(SettingsHandler::getInstance()->setDatabaseFilename(""));

    // Read the setting back.
    QVERIFY(SettingsHandler::getInstance()->databaseFilename() == "keydatabase.db");

    // Make sure the database exists at that location...
    QVERIFY(QFile(SettingsHandler::getInstance()->fullDatabasePathAndFilename()).exists());

    // ... and that it no longer exists at the old location.
    QVERIFY(!QFile(oldLocation).exists());
}
