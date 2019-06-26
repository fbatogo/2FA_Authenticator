#include "settingshandler.h"

#include <QDir>
#include <QVariant>
#include <logger.h>

const QString DOT_DIRECTORY = ".Rollin";          // The name of the dot directory we will initially use to store the database file.

SettingsHandler::~SettingsHandler()
{
    // Make sure our settings are flushed to disk.  (Should be called in the QSettings dtor, but call it here to be safe.)
    mSettingsDatabase->sync();

    // Clean up.
    delete mSettingsDatabase;
    mSettingsDatabase = nullptr;
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

    mSettingsDatabase->setValue("Settings/showHotpCounter", mShowHotpCounter);
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

    mSettingsDatabase->setValue("Settings/showIssuer", mShowIssuer);
}

/**
 * @brief SettingsHandler::showHashAlgorithm - Return the boolean that indicates if we
 *      should show the hash algorithm with the OTP value.
 *
 * @return true if the hash algorithm should be shown with the OTP value.  false
 *      otherwise.
 */
bool SettingsHandler::showHashAlgorithm()
{
    return mShowAlgorithm;
}

/**
 * @brief SettingsHandler::setShowHashAlgorithm - Set the setting that indicates if we
 *      should show the hash algorithm with the OTP value.
 *
 * @param newvalue - true if we should show the hash algorithm.  false otherwise.
 */
void SettingsHandler::setShowHashAlgorithm(bool newvalue)
{
    mShowAlgorithm = newvalue;

    mSettingsDatabase->setValue("Settings/showHashAlgorithm", mShowAlgorithm);
}

/**
 * @brief SettingsHandler::logToFile - Return the boolean that indicates if we should
 *      be writing log data to a log file, or not.
 *
 * @return true if we should be writing to a log file.  false if we should not.
 */
bool SettingsHandler::logToFile()
{
    return mLogToFile;
}

/**
 * @brief SettingsHandler::setLogToFile - Change the setting that indicates if we should
 *      be writing to a log file, or not.
 *
 * @param newvalue - Set to true if we should be writing to a log file.  false if we should
 *      not.
 */
void SettingsHandler::setLogToFile(bool newvalue)
{
    mLogToFile = newvalue;

    mSettingsDatabase->setValue("Settings/logToFile", mLogToFile);

    // Change the state of the logger.
    Logger::getInstance()->setLogToFile(mLogToFile);
}

/**
 * @brief SettingsHandler::databaseLocation - Return the location that the database file is
 *      written to.
 *
 * @return QString containing the path to the location of the database file.
 */
QString SettingsHandler::databaseLocation()
{
    // If we don't have a database location explicitly defined, return the dataPath().
    if (mDatabaseLocation.isEmpty()) {
        return dataPath();
    }

    // Otherwise, return the custom path.
    return mDatabaseLocation;
}

/**
 * @brief SettingsHandler::setDatabaseLocation - Change the location that the database file
 *      will be written to by moving the existing database to the new location, and updating
 *      the settings value.
 *
 * @param newLocation - QString containing the new location to read/write the database file
 *      to/from.
 *
 * @return true if the new database location was set, and the database was moved to it.
 *      false if the database location wasn't set because the database file couldn't
 *      be moved for some reason.   In the case that false is returned, the new
 *      directory setting will be ignored, and the old value will continue to be used.
 */
bool SettingsHandler::setDatabaseLocation(const QString &newLocation)
{
    QString dbPath;

    // If the path hasn't changed, don't do anything.
    if (mDatabaseLocation == newLocation) {
        return true;
    }

    // If the newLocation string is empty, reset the path back to the default.
    if (newLocation.isEmpty()) {

    }

    // XXX Move the database to the new location.

    // Update the settings data.
    mDatabaseLocation = newLocation;

    mSettingsDatabase->setValue("Settings/databaseLocation", mDatabaseLocation);

    // XXX Change this once the implementation is finished.
    return false;
}

/**
 * @brief SettingsHandler::databaseDirectoryExistsOrIsCreated - Check to see if the
 *      directory specified to store the database file exists.  If it doesn't attempt
 *      to create it.
 *
 * @return true if the directory exists, or was created.  false if it doesn't exist,
 *      or couldn't be created.
 */
bool SettingsHandler::databaseDirectoryExistsOrIsCreated()
{
    return directoryExistsOrIsCreated(databaseLocation());
}

/**
 * @brief SettingsHandler::databaseFilename - Get the file name that should be used to
 *      store the database.
 *
 * @return QString containing the file name that the database should be stored to.
 */
QString SettingsHandler::databaseFilename()
{
    return mDatabaseFilename;
}

/**
 * @brief SettingsHandler::setDatabaseFilename - Set a new file name to store the database
 *      file as.
 *
 * @param newFilename - The new file name (sans path) that should be used to store the
 *      database.
 *
 * @return true if the database file name was changed, and the database was moved to the
 *      new file name.  false is the database file name couldn't be changed.  If the
 *      file name can't be changed, the change will be discarded, and the old file name
 *      will continue to be used.
 */
bool SettingsHandler::setDatabaseFilename(const QString &newFilename)
{
    // Rename the database file to the new name.

    // Then update the file name stored in the settings.

    // XXX Implement.
    return false;
}

/**
 * @brief SettingsHandler::fullDatabasePathAndFilename - Convenience call to return the
 *      full path and file name of the database file.
 *
 * @return QString containing the full path and file name of the database file.
 */
QString SettingsHandler::fullDatabasePathAndFilename()
{
    return databaseLocation() + "/" + databaseFilename();
}

/**
 * @brief SettingsHandler::dataPath - Get the path the should be used to store data for this app.
 *
 * @return QString containing the path that should be used to store data files for this app.
 */
QString SettingsHandler::dataPath()
{
    QString dotDirectory;

    // XXX Update this so that it stores in the AppData directory on Windows, and a dot directory on *nix.  (Maybe the .config directory on *nix?)

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
 *      exists.  If it exists, returns true.  If it doesn't exist, attempt to create it and
 *      returns true if it was created, or false if not.
 *
 * @return true if the directory already exists or was created.  false if it doesn't exist and
 *      couldn't be created.
 */
bool SettingsHandler::dataDirectoryExistsOrIsCreated()
{
    return directoryExistsOrIsCreated(dataPath());
}

/**
 * @brief SettingsHandler::directoryExistsOrIsCreated - Check to see if a directory
 *      exists.  If it doesn't, then attempt to create it.
 *
 * @param directory - The path of the directory that we want to check the existance of
 *      or to create if it doesn't exist.
 *
 * @return true if the directory exists, or was created.  false if it doesn't exist,
 *      and couldn't be created.
 */
bool SettingsHandler::directoryExistsOrIsCreated(const QString &directory)
{
    QDir targetDir(directory);

    if (!targetDir.exists()) {
        LOG_DEBUG("The directory '" + directory + "' doesn't exist.  Attempting to create it...");

        if (!targetDir.mkpath(".")) {
            LOG_ERROR("Unable to create the directory '" + directory + "'!");
            return false;
        }
    }

    return true;
}

/**
 * @brief SettingsHandler::moveDatabaseToNewTarget - Change the location and/or file name
 *      of the database file.
 *
 * @param oldPath - The current path to the database file.
 * @param newPath - The destination path of the database file.
 *
 * @return true if the database file was moved to the new location.  false if it wasn't.
 */
bool SettingsHandler::moveDatabaseToNewTarget(const QString &oldPath, const QString &newPath)
{
    // XXX Implement!
    return false;
}

SettingsHandler::SettingsHandler()
{
    // Start by assuming that this object is invalid.
    mValid = false;

    // Set our default values.
    mShowHotpCounter = false;
    mShowIssuer = false;
    mLogToFile = false;
    mDatabaseLocation.clear();
    mDatabaseFilename = "keydatabase.db";

    // Make sure the data directory exists, so that we can read or write our settings.
    if (!dataDirectoryExistsOrIsCreated()) {
        LOG_ERROR("Unable to create the data directory!");

        // Nothing we can do.
        return;
    }

    LOG_DEBUG("Data path : " + dataPath());

    // Create the settings object, and force it to write to our target directory.
    mSettingsDatabase = new QSettings(dataPath() + "/Rollin.ini", QSettings::IniFormat);

    // Attempt to load the values that were set last.
    mShowIssuer = mSettingsDatabase->value("Settings/showIssuer", false).toBool();
    mShowHotpCounter = mSettingsDatabase->value("Settings/showHotpCounter", false).toBool();
    mShowAlgorithm = mSettingsDatabase->value("Settings/showHashAlgorithm", false).toBool();
    mLogToFile = mSettingsDatabase->value("Settings/logToFile", false).toBool();
    mDatabaseLocation = mSettingsDatabase->value("Settings/databasePath", "").toString();               // An empty string will map to the DOT_DIRECTORY value at the top of this file.
    mDatabaseFilename = mSettingsDatabase->value("Settings/databaseFilename", "keydatabase.db").toString();

    LOG_DEBUG("Saving settings to  : " + mSettingsDatabase->fileName());

    // XXX Open the database file using the information available?

    // XXX Need to use a lock file to indicate when the database file is in use.  This
    // is to allow us to handle the situation where the database file is stored someplace
    // where two instances of the app might use it at the same time, such as a file
    // server or syncing system (like Syncthing).
}
