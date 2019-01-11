#include "databasekeystorage.h"

#include <QDir>
#include <logger.h>

#define DOT_DIRECTORY       ".2FA"          // The name of the dot directory we will use in the user home directory.

DatabaseKeyStorage::DatabaseKeyStorage()
{
    // Make sure the .2FA directory exists.
    if (!dotDirectoryExistsOrIsCreated()) {
        LOG_ERROR("Unable to create the directory to store the key entry database!");
        return;
    }

    // Open/create the SecretDatabase file.

}

DatabaseKeyStorage::~DatabaseKeyStorage()
{
    // Close the SecretDatabase file.
    mSecretDatabase.close();
}

bool DatabaseKeyStorage::initKeyStorage()
{

}

bool DatabaseKeyStorage::keyByIdentifier(const QString &identifier, KeyEntry &result)
{

}

bool DatabaseKeyStorage::getAllKeys(std::vector<KeyEntry> &result)
{

}

bool DatabaseKeyStorage::addKey(const KeyEntry &entry)
{

}

bool DatabaseKeyStorage::updateKey(const KeyEntry &currentEntry, const KeyEntry &newEntry)
{

}

bool DatabaseKeyStorage::freeKeyStorage()
{

}

/**
 * @brief DatabaseKeyStorage::getDotDirectory - Return the path to the user's .2FA directory.
 *
 * @return QString containing the path to the users .2FA directory.
 */
QString DatabaseKeyStorage::getDotDirectory()
{

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

}

