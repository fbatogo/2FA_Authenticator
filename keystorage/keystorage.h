#ifndef KEYSTORAGE_H
#define KEYSTORAGE_H

#include <vector>
#include "keystoragebase.h"
#include "keyentry.h"

const size_t KEYSTORAGE_METHOD_DEFAULT = 0;       // Use whatever storage method is available.

class KeyStorage
{
public:
    KeyStorage();
    KeyStorage(KeyStorage &toCopy);
    ~KeyStorage();

    bool available();

    bool initStorage();
    bool keyByIdentifier(const QString &identifier, KeyEntry &result);
    bool getAllKeys(QList<KeyEntry> &result);
    bool addKey(const KeyEntry &entry, int keyStorageMethod = KEYSTORAGE_METHOD_DEFAULT);
    bool updateKey(const KeyEntry &currentEntry, const KeyEntry &newEntry, int keyStorageMethod = KEYSTORAGE_METHOD_DEFAULT);
    bool deleteKeyByIdentifier(const QString &identifier);
    bool freeStorage();

    KeyStorage& operator=(const KeyStorage& toCopy);

private:
    bool findKeyByIdentifier(const QString &identifier, KeyEntry &result, int &storageDriverId);
    void copy(const KeyStorage &toCopy);

    std::vector<KeyStorageBase *> mKeyStorageDrivers;
    bool mAvailable;
};

#endif // KEYSTORAGE_H
