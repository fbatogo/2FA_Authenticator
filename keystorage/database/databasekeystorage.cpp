#include "databasekeystorage.h"

#include <QDir>
#include <logger.h>

#define DOT_DIRECTORY       ".2FA"          // The name of the dot directory we will use in the user home directory.

DatabaseKeyStorage::DatabaseKeyStorage()
{
}

DatabaseKeyStorage::~DatabaseKeyStorage()
{
    // Make sure we close the database when this object is destroyed.
    freeKeyStorage();
}

/**
 * @brief DatabaseKeyStorage::storageId - Return the integer that identifies this
 *      key entry storage method.
 *
 * @return KEYSTORAGE_METHOD_DATABASE.
 */
int DatabaseKeyStorage::storageId()
{
    return KEYSTORAGE_METHOD_DATABASE;
}

/**
 * @brief DatabaseKeyStorage::initKeyStorage - Make sure the path for the database
 *      exists and open the database.
 *
 * @return true if the path exists, and the database was opened.  false on error.
 */
bool DatabaseKeyStorage::initKeyStorage()
{
    // Make sure the .2FA directory exists.
    if (!dotDirectoryExistsOrIsCreated()) {
        LOG_ERROR("Unable to create the directory to store the key entry database!");
        return false;
    }

    // Open/create the SecretDatabase file.
    return mSecretDatabase.open(getDotDirectory() + "keydatabase.db");
}

/**
 * @brief DatabaseKeyStorage::keyByIdentifier - Locate a key in the database using the
 *      identifier for the key.
 *
 * @param identifier - The "identifier" (usually a site name, or similar) that we want
 *      to get a key entry for.
 * @param result[OUT] - If this method returns true, this variable contains the key
 *      entry data for the specified "identifier".
 *
 * @return true if the key was found.  false otherwise.
 */
bool DatabaseKeyStorage::keyByIdentifier(const QString &identifier, KeyEntry &result)
{
    return mSecretDatabase.getByIdentifier(identifier, result);
}

/**
 * @brief DatabaseKeyStorage::getAllKeys - Get all off the key entries that are stored
 *      in the database.
 *
 * @param result[OUT] - If this method returns true, this variable will contain a vector
 *      with all of the key entry values.
 *
 * @return true if all of the key entry values were read.  false otherwise.
 */
bool DatabaseKeyStorage::getAllKeys(std::vector<KeyEntry> &result)
{
    return mSecretDatabase.getAll(result);
}

/**
 * @brief DatabaseKeyStorage::addKey - Add a new key entry to the database.
 *
 * @param entry - The new key entry to add to the database.
 *
 * @return true if the key entry was added to the database.  false otherwise.
 */
bool DatabaseKeyStorage::addKey(const KeyEntry &entry)
{
    return mSecretDatabase.add(entry);
}

/**
 * @brief DatabaseKeyStorage::updateKey - Update a key entry that exists in the database.
 *
 * @param currentEntry - The current entry that is in the database.
 * @param newEntry - The entry that should replace the current entry.
 *
 * @return true if the entry was updated in the database.  false otherwise.
 */
bool DatabaseKeyStorage::updateKey(const KeyEntry &currentEntry, const KeyEntry &newEntry)
{
    return mSecretDatabase.update(currentEntry, newEntry);
}

/**
 * @brief DatabaseKeyStorage::freeKeyStorage - Clean up the database object by closing the database
 *      file, if it is open.
 *
 * @return true if the database was closed.  false on error.
 */
bool DatabaseKeyStorage::freeKeyStorage()
{
    if (mSecretDatabase.isOpen()) {
        // Close the SecretDatabase file.
        return mSecretDatabase.close();
    }

    // Otherwise, just retur success.
    return true;
}

/**
 * @brief DatabaseKeyStorage::getDotDirectory - Return the path to the user's .2FA directory.
 *
 * @return QString containing the path to the users .2FA directory.
 */
QString DatabaseKeyStorage::getDotDirectory()
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
 * @brief DatabaseKeyStorage::dotDirectoryExistsOrIsCreated - Check to see if the .2FA directory
 *      exists in the user home directory.  If it doesn't, then attempt to create it.
 *
 * @return true if the directory exists, or was created.  false if the directory doesn't exist,
 *      and couldn't be created.
 */
bool DatabaseKeyStorage::dotDirectoryExistsOrIsCreated()
{    
    QDir targetDir(getDotDirectory());

    if (!targetDir.exists()) {
        LOG_DEBUG("The dot directory doesn't exist.  Attempting to create it...");

        if (!targetDir.mkpath(".")) {
            LOG_ERROR("Unable to create the dot directory to store the key entry database!");
            return false;
        }
    }

    return true;
}

