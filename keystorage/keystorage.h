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

    bool available();

    bool initStorage();
    bool keyByIdentifier(const QString &identifier, KeyEntry &result);
    bool getAllKeys(QList<KeyEntry> &result);
    bool addKey(const KeyEntry &entry, int keyStorageMethod = KEYSTORAGE_METHOD_DEFAULT);
    bool updateKey(const KeyEntry &currentEntry, const KeyEntry &newEntry);
    bool freeStorage();

private:
    bool findKeyByIdentifier(const QString &identifier, KeyEntry &result, int &storageDriverId);

    std::vector<KeyStorageBase *> mKeyStorageDrivers;
    bool mAvailable;
};

#endif // KEYSTORAGE_H
