#include "settingshandler.h"

#include <QDir>
#include <logger.h>

#define DOT_DIRECTORY       ".Rollin"          // The name of the dot directory we will use in the user home directory.

SettingsHandler::~SettingsHandler()
{
    // Make sure our settings are flushed to disk.  (Should be called in the QSettings dtor, but call it here to be safe.)
    mSettingsDatabase.sync();
}

/**
 * @brief SettingsHandler::getInstance - Return the C/C++ singleton for the settings handler.
 *
 * @return SettingsHandler pointer to the singleton for the settings.
 */
SettingsHandler *SettingsHandler::getInstance()
{
    static SettingsHandler singleton;

    return &singleton;
}

/**
 * @brief SettingsHandler::getQmlSingleton - Return the singlton used by the QML code.
 *
 * @param engine - A pointer to the QML engine that will be used for the QML layouts.
 * @param scriptEngine - A pointer to the QML script engine that will be used.
 *
 * @return QObject pointer to the singleton to use with the QML code.
 */
QObject *SettingsHandler::getQmlSingleton(QQmlEngine *, QJSEngine *)
{
    SettingsHandler *cSingleton;

    cSingleton = getInstance();

    QQmlEngine::setObjectOwnership(cSingleton, QQmlEngine::CppOwnership);

    return static_cast<QObject *>(cSingleton);
}

/**
 * @brief SettingsHandler::showHotpCounterValue - Contain the setting that indicates if we
 *      should show the HOTP counter value, in addition to the current code.
 *
 * @return true if we should show the HOTP code.  false otherwise.
 */
bool SettingsHandler::showHotpCounterValue()
{
    return mShowHotpCounter;
}

/**
 * @brief SettingsHandler::setShowHotpCounterValue - Set the boolean that indicates if we
 *      should display the HOTP counter value in addition to the OTP.
 *
 * @param newvalue - The new value that indicates if we should show the HOTP counter
 *      value in addition to the OTP value.
 */
void SettingsHandler::setShowHotpCounterValue(bool newvalue)
{
    mShowHotpCounter = newvalue;

    mSettingsDatabase.setValue("main/showHotpCounter", mShowHotpCounter);
}

/**
 * @brief SettingsHandler::showIssuer - Return the setting that indicates if we should show
 *      the OTP issuer along with the current counter value.
 *
 * @return true if we should show the OTP issue along with the counter value.  false otherwise.
 */
bool SettingsHandler::showIssuer()
{
    return mShowIssuer;
}

/**
 * @brief SettingsHandler::setShowIssuer - Set the boolean that indicates if we should
 *      display the issuer along with the OTP value.
 *
 * @param newvalue - The new value that indicates if we should show the issuer along with the
 *      OTP value.
 */
void SettingsHandler::setShowIssuer(bool newvalue)
{
    mShowIssuer = newvalue;

    mSettingsDatabase.setValue("main/showIssuer", mShowIssuer);
}

/**
 * @brief SettingsHandler::dataPath - Get the path the should be used to store data for this app.
 *
 * @return QString containing the path that should be used to store data files for this app.
 */
QString SettingsHandler::dataPath()
{
    QString dotDirectory;

    // Start by getting the user home directory.
    dotDirectory = QDir::homePath();

    if (!dotDirectory.endsWith("/")) {
        // Add the slash.
        dotDirectory += "/";
    }

    // Then, add the directory name.
    dotDirectory += DOT_DIRECTORY;
    dotDirectory += "/";

    return dotDirectory;
}

/**
 * @brief SettingsHandler::dataDirectoryExistsOrIsCreated - Check to see if our data directory
 *      exists.  If it exists, return true.  If it doesn't exist, attempt to create it and
 *      return true if it was created, or false if not.
 *
 * @return true if the directory already exists or was created.  false if it doesn't exist and
 *      couldn't be created.
 */
bool SettingsHandler::dataDirectoryExistsOrIsCreated()
{
    QDir targetDir(dataPath());

    if (!targetDir.exists()) {
        LOG_DEBUG("The dot directory doesn't exist.  Attempting to create it...");

        if (!targetDir.mkpath(".")) {
            LOG_ERROR("Unable to create the dot directory to store the key entry database!");
            return false;
        }
    }

    return true;
}

SettingsHandler::SettingsHandler() :
    mSettingsDatabase(QSettings::IniFormat, QSettings::UserScope, "Rollin", "Rollin")
{
    // Start by assuming that this object is invalid.
    mValid = false;

    // Set our default values.
    mShowHotpCounter = false;
    mShowIssuer = false;

    // Make sure the data directory exists, so that we can read or write our settings.
    if (!dataDirectoryExistsOrIsCreated()) {
        LOG_ERROR("Unable to create the data directory!");

        // Nothing we can do.
        return;
    }

    // Override the path to store our settings at.
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, dataPath());
}
