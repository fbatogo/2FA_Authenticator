#include "settingshandlertests.h"

#include <QtTest>
#include "../settingshandler.h"

void SettingsHandlerTests::getSetSettingsTests()
{
    QVERIFY(nullptr != SettingsHandler::getQmlSingleton(nullptr, nullptr));

    QVERIFY(SettingsHandler::getInstance()->dataDirectoryExistsOrIsCreated());
    QVERIFY(!SettingsHandler::getInstance()->dataPath().isEmpty());

    // Set values to their defaults.
    SettingsHandler::getInstance()->setShowIssuer(false);
    SettingsHandler::getInstance()->setLogToFile(false);
    SettingsHandler::getInstance()->setShowHashAlgorithm(false);
    SettingsHandler::getInstance()->setShowHotpCounterValue(false);

    // Read them back.
    QVERIFY(!SettingsHandler::getInstance()->showIssuer());
    QVERIFY(!SettingsHandler::getInstance()->logToFile());
    QVERIFY(!SettingsHandler::getInstance()->showHashAlgorithm());
    QVERIFY(!SettingsHandler::getInstance()->showHotpCounterValue());

    // Set them all true.
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
}
