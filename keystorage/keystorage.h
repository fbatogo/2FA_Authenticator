#ifndef KEYSTORAGE_H
#define KEYSTORAGE_H

#include <vector>
#include <memory>
#include "keystoragebase.h"
#include "keyentry.h"

const size_t KEYSTORAGE_METHOD_DEFAULT = 0;       // Use whatever storage method is available.

class KeyStorage
{
public:
    KeyStorage();

    bool available();
    bool isOpen();

    bool initStorage();
    bool keyByIdentifier(const QString &identifier, KeyEntry &result);
    bool getAllKeys(QList<KeyEntry> &result);
    bool addKey(const KeyEntry &entry, int keyStorageMethod = KEYSTORAGE_METHOD_DEFAULT);
    bool updateKey(const KeyEntry &currentEntry, const KeyEntry &newEntry, int keyStorageMethod = KEYSTORAGE_METHOD_DEFAULT);
    bool deleteKeyByIdentifier(const QString &identifier);
    bool freeStorage();

private:
    bool findKeyByIdentifier(const QString &identifier, KeyEntry &result, int &storageDriverId);

    std::vector<std::shared_ptr<KeyStorageBase> > mKeyStorageDrivers;
    bool mAvailable;
};

#endif // KEYSTORAGE_H
