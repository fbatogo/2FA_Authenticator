#include "databasekeystorage.h"

#include <QDir>
#include <logger.h>
#include "settingshandler.h"

DatabaseKeyStorage::DatabaseKeyStorage()
{
    mSecretDatabase = nullptr;
}

DatabaseKeyStorage::~DatabaseKeyStorage()
{
    // Free the database object, if it isn't already freed.
    if (nullptr != mSecretDatabase) {
        freeKeyStorage();
    }
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

bool DatabaseKeyStorage::isOpen()
{
    if (nullptr == mSecretDatabase) {
        // The database isn't allocated, so it isn't open.
        return false;
    }

    return mSecretDatabase->isOpen();
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

    // Create the database object.
    mSecretDatabase = new SecretDatabase();
    if (nullptr == mSecretDatabase) {
        LOG_ERROR("Unable to allocate a new SecretDatabase object!");
        return false;
    }

    // Open/create the SecretDatabase file.
    return mSecretDatabase->open(SettingsHandler::getInstance()->fullDatabasePathAndFilename());
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
    if (nullptr == mSecretDatabase) {
        LOG_ERROR("The database is not open while attempting to get a key by identifier!");
        return false;
    }

    return mSecretDatabase->getByIdentifier(identifier, result);
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
    if (nullptr == mSecretDatabase) {
        LOG_ERROR("The secret database isn't open while attempting to get all key data!");
        return false;
    }

    return mSecretDatabase->getAll(result);
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
    if (nullptr == mSecretDatabase) {
        LOG_ERROR("The secret database isn't open while attempting to add a key entry!");
        return false;
    }

    return mSecretDatabase->add(entry);
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
    if (nullptr == mSecretDatabase) {
        LOG_ERROR("The secret database isn't open while attempting to update a key entry!");
        return false;
    }

    return mSecretDatabase->update(currentEntry, newEntry);
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
    if (nullptr == mSecretDatabase) {
        LOG_ERROR("The database was not initialized before attempting to delete a key by identifier!");
        return false;
    }

    if (!mSecretDatabase->isOpen()) {
        LOG_ERROR("Unable to delete the key with identifier '" + identifier + "'.  The database isn't open!");
        return false;
    }

    return mSecretDatabase->deleteByIdentifier(identifier);
}

/**
 * @brief DatabaseKeyStorage::freeKeyStorage - "Free" the key storage database by closing
 *      the database itself.
 *
 * @return true if the database was closed.  false otherwise.
 */
bool DatabaseKeyStorage::freeKeyStorage()
{
    if (nullptr == mSecretDatabase) {
        LOG_DEBUG("The secret database wasn't open/allocated when trying to free.  Ignoring.");
        return false;
    }

    if (!mSecretDatabase->close()) {
        LOG_ERROR("Failed to close the secret key database!");
        return false;
    }

    // Delete the object in memory.
    delete mSecretDatabase;
    mSecretDatabase = nullptr;

    return true;
}
