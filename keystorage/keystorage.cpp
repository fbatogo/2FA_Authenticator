#include "keystorage.h"

KeyStorage::KeyStorage()
{

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

}

/**
 * @brief KeyStorage::getAllKeys - Attempt to get a list of all key entries stored in all storage methods.
 *
 * @param result[OUT] - If this method returns true, this variable will contain a vector of all of the
 *      key entries found.  If this method returns false, any values in this variable should be ignored.
 *
 * @return true if all key entries were returned.  false if all key entries couldn't be returned.
 */
bool KeyStorage::getAllKeys(std::vector<KeyEntry> &result)
{

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
bool KeyStorage::addKey(const KeyEntry &entry)
{

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
bool KeyStorage::updateKey(const KeyEntry &currentEntry, const KeyEntry &newEntry)
{

}
