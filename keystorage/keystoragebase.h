#ifndef KEYSTORAGEBASE_H
#define KEYSTORAGEBASE_H

#include <string>
#include <vector>
#include "keyentry.h"

/****
 * This base class is a template for the key storage "drivers" that can be implemented and used.  After adding a driver
 * (by overriding and implementing this class), you must add an "if" statement in keystorage.cpp to be able to make
 * use of the new driver.
 *
 * To see what is expected of each method listed below, read the method comments for the matching methods in keystorage.cpp.
 */


class KeyStorageBase
{
public:
    KeyStorageBase() = default;
    virtual ~KeyStorageBase() = default;

    virtual int storageId() = 0;

    virtual bool initKeyStorage() = 0;
    virtual bool keyByIdentifier(const QString &identifier, KeyEntry &result) = 0;
    virtual bool getAllKeys(std::vector<KeyEntry> &result) = 0;
    virtual bool addKey(const KeyEntry &entry) = 0;
    virtual bool updateKey(const KeyEntry &currentEntry, const KeyEntry &newEntry) = 0;
    virtual bool deleteKeyByIdentifier(const QString &identifier) = 0;
    virtual bool freeKeyStorage() = 0;
};

#endif // KEYSTORAGEBASE_H
