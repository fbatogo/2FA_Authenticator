#ifndef KEYSTORAGE_H
#define KEYSTORAGE_H

#include <vector>
#include "keystoragebase.h"
#include "keyentry.h"

#define KEYSTORAGE_METHOD_DEFAULT       0       // Use whatever storage method is available.

class KeyStorage
{
public:
    KeyStorage();

    bool keyByIdentifier(const QString &identifier, KeyEntry &result);
    bool getAllKeys(std::vector<KeyEntry> &result);
    bool addKey(const KeyEntry &entry);
    bool updateKey(const KeyEntry &currentEntry, const KeyEntry &newEntry);

private:
    std::vector<KeyStorageBase *> mKeyStorageDrivers;
};

#endif // KEYSTORAGE_H
