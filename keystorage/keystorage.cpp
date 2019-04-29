#include "keystorage.h"

#include <logger.h>

#include "database/databasekeystorage.h"

KeyStorage::KeyStorage()
{
    mKeyStorageDrivers.clear();

    // Add all of the available storage methods to our vector.
    mKeyStorageDrivers.push_back(new DatabaseKeyStorage());

    mAvailable = false;
}

KeyStorage::KeyStorage(KeyStorage &toCopy)
{
    copy(toCopy);
}

KeyStorage::~KeyStorage()
{
    // Clean up the keystorage drivers that are registered.
    for (size_t i = 0; i < mKeyStorageDrivers.size(); i++) {
        delete mKeyStorageDrivers.at(i);
    }
}

/**
 * @brief KeyStorage::available - Return the flag that indicates if we should be able to read/write
 *      KeyEntry objects using the key storage.
 *
 * @return true if the key storage is available.  false otherwise.
 */
bool KeyStorage::available()
{
    return mAvailable;
}

/**
 * @brief KeyStorage::initStorage - Iterate through all of the key storage drivers and
 *      initialize them.
 *
 * @return true if all of the key storage methods were initialized.  false
 *      otherwise.
 */
bool KeyStorage::initStorage()
{
    for (size_t i = 0; i < mKeyStorageDrivers.size(); i++) {
        if (!mKeyStorageDrivers.at(i)->initKeyStorage()) {
            LOG_ERROR("Failed to initialize key storage driver with an id of " + QString::number(mKeyStorageDrivers.at(i)->storageId()) + ".");
            return false;
        }
    }

    return true;
}

/**
 * @brief KeyStorage::keyByIdentifier - Attempt to locate a key entry based on the identifier provided.
 *
 * @param identifier - The identifier to search for in various key storage methods.
 * @param result[OUT] - If this method returns true, this variable will contain the key entry that was
 *      read.
 *
 * @return true if the key entry was found and returned.  false if it wasn't found or couldn't be returned.
 */
bool KeyStorage::keyByIdentifier(const QString &identifier, KeyEntry &result)
{
    int driverId;

    return findKeyByIdentifier(identifier, result, driverId);
}

/**
 * @brief KeyStorage::getAllKeys - Attempt to get a list of all key entries stored in all storage methods.
 *
 * @note The resulting list may contain key entries that are marked as 'invalid'.  This is
 *      to allow the UI to display as much information about the invalid keys as possible.
 *
 * @param result[OUT] - If this method returns true, this variable will contain a vector of all of the
 *      key entries found.  If this method returns false, any values in this variable should be ignored.
 *
 * @return true if all key entries were returned.  false if all key entries couldn't be returned.
 */
bool KeyStorage::getAllKeys(QList<KeyEntry> &result)
{
    std::vector<KeyEntry> readKeys;

    // Make sure the return vector is empty to start with.
    result.clear();

    for (size_t i = 0; i < mKeyStorageDrivers.size(); i++) {
        if (!mKeyStorageDrivers.at(i)->getAllKeys(readKeys)) {
            LOG_ERROR("Failed to read all keys from the key storage driver with an id of " + QString::number(mKeyStorageDrivers.at(i)->storageId()) + ".");
            return false;
        }

        // Iterate the latest results, and add them to the result variable.
        for (size_t i = 0; i < readKeys.size(); i++) {
            result.push_back(readKeys.at(i));
        }
    }

    return true;
}

/**
 * @brief KeyStorage::addKey - Add a new key entry to the named key storage method.  If the key storage method
 *      is 0 then the key entry will be written to whichever key storage method is the first in the list.
 *
 * @param entry - A KeyEntry object that contains the data to be written to the specified key storage method.
 * @param keyStorageMethod - One of the KEYSTORAGE_METHOD_* values in keystorage.h.
 *
 * @return true if the key data was stored in the specified key storage method.  false on error.
 */
bool KeyStorage::addKey(const KeyEntry &entry, int keyStorageMethod)
{
    KeyEntry temp;

    if (!entry.valid()) {
        LOG_ERROR("Refusing to add an invalid key entry to key storage.");
        return false;
    }

    // See if the entry alreeady exists somewhere.
    if (keyByIdentifier(entry.identifier(), temp)) {
        LOG_ERROR("Cannot add a key entry that already exists in a key provider!  Did you mean to update?");
        return false;
    }

    if (keyStorageMethod == KEYSTORAGE_METHOD_DEFAULT) {
        // Just use the first one in the list.
        return mKeyStorageDrivers.at(0)->addKey(entry);
    }

    // Otherwise, search for the driver we want to use.
    for (size_t i = 0; i < mKeyStorageDrivers.size(); i++) {
        if (mKeyStorageDrivers.at(i)->storageId() == keyStorageMethod) {
            // Found it.  Add it to this storage method.
            return mKeyStorageDrivers.at(i)->addKey(entry);
        }
    }

    LOG_ERROR("Unable to locate a key storage method for id " + QString::number(keyStorageMethod) + ".");
    return false;
}

/**
 * @brief KeyStorage::updateKey - Search the key storage providers, locate the key with the specified identifier,
 *      and update it with the values in newEntry.
 *
 * @param currentEntry - The entry that we want to update the data for.
 * @param newEntry - How the entry should look after it is updated.
 *
 * @return true if the key entry was updated.  false on error.
 */
bool KeyStorage::updateKey(const KeyEntry &currentEntry, const KeyEntry &newEntry, int keyStorageMethod)
{
    if ((!currentEntry.valid()) || (!newEntry.valid())) {
        LOG_ERROR("Refusing to update in invalid key entry in the key storage!");
        return false;
    }

    if (keyStorageMethod == KEYSTORAGE_METHOD_DEFAULT) {
        // Just use the first one in the list.
        return mKeyStorageDrivers.at(0)->updateKey(currentEntry, newEntry);
    }

    // Otherwise, search for the driver we want to use.
    for (size_t i = 0; i < mKeyStorageDrivers.size(); i++) {
        if (mKeyStorageDrivers.at(i)->storageId() == keyStorageMethod) {
            // Found it.  Update it in this storage method.
            return mKeyStorageDrivers.at(i)->updateKey(currentEntry, newEntry);
        }
    }

    return true;
}

/**
 * @brief KeyStorage::deleteKeyByIdentifier - Delete a key from the key store(s) based on the identifier
 *      provided.
 *
 * @param identifier - The identifier of the key that we want to delete.
 *
 * @return true if the identifier was deleted in at least one key store.  false on error.
 */
bool KeyStorage::deleteKeyByIdentifier(const QString &identifier)
{
    bool deleted = false;

    for (size_t i = 0; i < mKeyStorageDrivers.size(); i++) {
        if (mKeyStorageDrivers.at(i)->deleteKeyByIdentifier(identifier)) {
            // It was deleted, so set the deleted flag to true.
            deleted = true;
        }
    }

    return deleted;
}

/**
 * @brief KeyStorage::freeStorage - Iterate through the key storage methods and free
 *      any resources they may have used.
 *
 * @return true if all of the key storage methods were freed.  false otherwise.
 */
bool KeyStorage::freeStorage()
{
    for (size_t i = 0; i < mKeyStorageDrivers.size(); i++) {
        if (!mKeyStorageDrivers.at(i)->freeKeyStorage()) {
            LOG_ERROR("Failed to clean up the key storage driver with an id of " + QString::number(mKeyStorageDrivers.at(i)->storageId()) + ".");
            return false;
        }
    }

    return true;
}

KeyStorage &KeyStorage::operator=(const KeyStorage &toCopy)
{
    if (this == &toCopy) {
        return (*this);
    }

    copy(toCopy);

    return (*this);
}

/**
 * @brief KeyStorage::findKeyByIdentifier - Search all providers and return the key entry for the identifier along with
 *      the storage id for the method that stored it.
 *
 * @param identifier - The identifier to look for in all of the storage providers.
 * @param result[OUT] - If this method returns true, this variable will contain the key entry that was found.
 * @param storageDriverId[OUT] - If this method returns true, this variable will contain the storage ID for the
 *      storage driver that has stored the found key.
 *
 * @return true if the key was found in a storage provider.  false otherwise.
 */
bool KeyStorage::findKeyByIdentifier(const QString &identifier, KeyEntry &result, int &storageDriverId)
{
    result.clear();
    storageDriverId = -1;

    for (size_t i = 0; i < mKeyStorageDrivers.size(); i++) {
        if (mKeyStorageDrivers.at(i)->keyByIdentifier(identifier, result)) {
            storageDriverId = static_cast<int>(i);
            return true;        // We found it.
        }
    }

    LOG_DEBUG("Unable to locate the key with identifier '" + identifier + "'.");
    return false;
}

/**
 * @brief KeyStorage::copy - Copy the members from 'toCopy' in to the current
 *  object.
 *
 * @param toCopy - The object to copy data from.
 */
void KeyStorage::copy(const KeyStorage &toCopy)
{
    mKeyStorageDrivers = toCopy.mKeyStorageDrivers;
    mAvailable = toCopy.mAvailable;
}
