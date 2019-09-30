#include <testsuitebase.h>

#include "../settingshandler.h"

#include "logger.h"
#include "keyentriessingleton.h"

EMPTY_TEST_SUITE(SettingsHandlerTests);

TEST_F(SettingsHandlerTests, GetSetSettingsTests)
{
    EXPECT_TRUE(nullptr != SettingsHandler::getQmlSingleton(nullptr, nullptr));

    EXPECT_TRUE(SettingsHandler::getInstance()->dataDirectoryExistsOrIsCreated());
    EXPECT_TRUE(!SettingsHandler::getInstance()->dataPath().isEmpty());

    // Set (most) values to their defaults.
    SettingsHandler::getInstance()->setShowIssuer(false);
    SettingsHandler::getInstance()->setLogToFile(false);
    SettingsHandler::getInstance()->setShowHashAlgorithm(false);
    SettingsHandler::getInstance()->setShowHotpCounterValue(false);

    // Read them back.
    EXPECT_TRUE(!SettingsHandler::getInstance()->showIssuer());
    EXPECT_TRUE(!SettingsHandler::getInstance()->logToFile());
    EXPECT_TRUE(!SettingsHandler::getInstance()->showHashAlgorithm());
    EXPECT_TRUE(!SettingsHandler::getInstance()->showHotpCounterValue());

    // Set them all to a new value.
    SettingsHandler::getInstance()->setShowIssuer(true);
    SettingsHandler::getInstance()->setLogToFile(true);
    SettingsHandler::getInstance()->setShowHashAlgorithm(true);
    SettingsHandler::getInstance()->setShowHotpCounterValue(true);

    // Read them back.
    EXPECT_TRUE(SettingsHandler::getInstance()->showIssuer());
    EXPECT_TRUE(SettingsHandler::getInstance()->logToFile());
    EXPECT_TRUE(SettingsHandler::getInstance()->showHashAlgorithm());
    EXPECT_TRUE(SettingsHandler::getInstance()->showHotpCounterValue());

    // Set values back to their defaults.
    SettingsHandler::getInstance()->setShowIssuer(false);
    SettingsHandler::getInstance()->setLogToFile(false);
    SettingsHandler::getInstance()->setShowHashAlgorithm(false);
    SettingsHandler::getInstance()->setShowHotpCounterValue(false);

    // Read them back.
    EXPECT_TRUE(!SettingsHandler::getInstance()->showIssuer());
    EXPECT_TRUE(!SettingsHandler::getInstance()->logToFile());
    EXPECT_TRUE(!SettingsHandler::getInstance()->showHashAlgorithm());
    EXPECT_TRUE(!SettingsHandler::getInstance()->showHotpCounterValue());
    EXPECT_TRUE(!SettingsHandler::getInstance()->databaseLocation().contains("dbtest"));
}

TEST_F(SettingsHandlerTests, DatabasePathLocationTests)
{
    QString oldLocation;

    // Make sure a database file exists.
    KeyEntriesSingleton::getInstance()->open();

    // Set the db location to the default.
    EXPECT_TRUE(SettingsHandler::getInstance()->setDatabaseLocation(""));

    // Read the setting back, and store it for later use.
    EXPECT_TRUE(SettingsHandler::getInstance()->databaseLocation().endsWith(".Rollin/"));

    // Store the full path and file name for later use.
    oldLocation = SettingsHandler::getInstance()->fullDatabasePathAndFilename();

    // Make sure the database exists at that location.
    EXPECT_TRUE(QFile(SettingsHandler::getInstance()->fullDatabasePathAndFilename()).exists());

    // Change the path to a new location.
    EXPECT_TRUE(SettingsHandler::getInstance()->setDatabaseLocation("dbtest"));   // A relative directory called "dbtest".

    // Read the value back.
    EXPECT_EQ(SettingsHandler::getInstance()->databaseLocation(), QString("dbtest"));

    // Make sure the database file was moved to that location...
    EXPECT_TRUE(QFile(SettingsHandler::getInstance()->fullDatabasePathAndFilename()).exists());

    // ... and that it no longer exists at the old location.
    EXPECT_TRUE(!QFile(oldLocation).exists());

    // Store the full path and file name for later use.
    oldLocation = SettingsHandler::getInstance()->fullDatabasePathAndFilename();

    // Move the database file back to the default location.
    EXPECT_TRUE(SettingsHandler::getInstance()->setDatabaseLocation(""));

    // Read the setting back.
    EXPECT_TRUE(!SettingsHandler::getInstance()->databaseLocation().endsWith(".Rollin"));

    // Make sure the database exists at that location...
    EXPECT_TRUE(QFile(SettingsHandler::getInstance()->fullDatabasePathAndFilename()).exists());

    // ... and that it no longer exists at the old location.
    EXPECT_TRUE(!QFile(oldLocation).exists());
}

TEST_F(SettingsHandlerTests, DatabaseFileLocationTests)
{
    QString oldLocation;

    // Set the db file name to the default.
    EXPECT_TRUE(SettingsHandler::getInstance()->setDatabaseFilename(""));

    // Read the setting back, and store it for later use.
    EXPECT_TRUE(SettingsHandler::getInstance()->databaseFilename() == "keydatabase.db");

    // Store the full path and file name for later use.
    oldLocation = SettingsHandler::getInstance()->fullDatabasePathAndFilename();

    // Make sure the database exists at that location.
    EXPECT_TRUE(QFile(SettingsHandler::getInstance()->fullDatabasePathAndFilename()).exists());

    // Change the filename.
    EXPECT_TRUE(SettingsHandler::getInstance()->setDatabaseFilename("unittest.db"));

    // Read the value back.
    EXPECT_EQ(SettingsHandler::getInstance()->databaseFilename(), QString("unittest.db"));

    // Make sure the database file was moved to that location...
    EXPECT_TRUE(QFile(SettingsHandler::getInstance()->fullDatabasePathAndFilename()).exists());

    // ... and that it no longer exists at the old location.
    EXPECT_TRUE(!QFile(oldLocation).exists());

    // Store the full path and file name for later use.
    oldLocation = SettingsHandler::getInstance()->fullDatabasePathAndFilename();

    // Move the database file back to the default location.
    EXPECT_TRUE(SettingsHandler::getInstance()->setDatabaseFilename(""));

    // Read the setting back.
    EXPECT_TRUE(SettingsHandler::getInstance()->databaseFilename() == "keydatabase.db");

    // Make sure the database exists at that location...
    EXPECT_TRUE(QFile(SettingsHandler::getInstance()->fullDatabasePathAndFilename()).exists());

    // ... and that it no longer exists at the old location.
    EXPECT_TRUE(!QFile(oldLocation).exists());
}
