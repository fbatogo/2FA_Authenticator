#ifndef DATABASEKEYSTORAGE_H
#define DATABASEKEYSTORAGE_H

#include "../keystoragebase.h"
#include "secretdatabase.h"

const unsigned int KEYSTORAGE_METHOD_DATABASE=1;       // Store key entries in a local database.

class DatabaseKeyStorage : public KeyStorageBase
{
public:
    DatabaseKeyStorage();
    ~DatabaseKeyStorage();

    int storageId();
    bool isOpen();

    bool initKeyStorage();
    bool keyByIdentifier(const QString &identifier, KeyEntry &result);
    bool getAllKeys(std::vector<KeyEntry> &result);
    bool addKey(const KeyEntry &entry);
    bool updateKey(const KeyEntry &currentEntry, const KeyEntry &newEntry);
    bool deleteKeyByIdentifier(const QString &identifier);
    bool freeKeyStorage();

private:
    SecretDatabase *mSecretDatabase;
};

#endif // DATABASEKEYSTORAGE_H
