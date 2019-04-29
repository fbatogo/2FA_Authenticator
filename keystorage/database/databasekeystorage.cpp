#include "databasekeystorage.h"

#include <QDir>
#include <logger.h>
#include "settingshandler.h"

DatabaseKeyStorage::DatabaseKeyStorage()
{
}

DatabaseKeyStorage::DatabaseKeyStorage(DatabaseKeyStorage &toCopy)
{
    copy(toCopy);
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
    // Make sure the .Rollin directory exists.
    if (!SettingsHandler::getInstance()->dataDirectoryExistsOrIsCreated()) {
        LOG_ERROR("Unable to create the directory to store the key entry database!");
        return false;
    }

    // Open/create the SecretDatabase file.
    return mSecretDatabase.open(SettingsHandler::getInstance()->dataPath() + "keydatabase.db");
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
 * @brief DatabaseKeyStorage::deleteKeyByIdentifier - Delete a key from key storage, based
 *      on the identifier for the key.
 *
 * @param identifier - The identifier for the key to delete.
 *
 * @return true if the key was deleted.  false on error.
 */
bool DatabaseKeyStorage::deleteKeyByIdentifier(const QString &identifier)
{
    if (!mSecretDatabase.isOpen()) {
        LOG_ERROR("Unable to delete the key with identifier '" + identifier + "'.  The database isn't open!");
        return false;
    }

    return mSecretDatabase.deleteByIdentifier(identifier);
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
 * @brief DatabaseKeyStorage::operator = - Copy the values of the provided object in to the current
 *      object.
 * @param toCopy - The object to copy to this one.
 *
 * @return this object with the requested data copied to it.
 */
DatabaseKeyStorage &DatabaseKeyStorage::operator=(const DatabaseKeyStorage &toCopy)
{
    if (this == &toCopy) {
        return (*this);
    }

    copy(toCopy);

    return (*this);
}

/**
 * @brief DatabaseKeyStorage::copy - The implementation for copying data from another object in to
 *      this one.
 *
 * @param toCopy - The object to copy data from.
 */
void DatabaseKeyStorage::copy(const DatabaseKeyStorage &toCopy)
{
    mSecretDatabase = toCopy.mSecretDatabase;
}
