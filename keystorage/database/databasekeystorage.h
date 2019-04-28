#ifndef DATABASEKEYSTORAGE_H
#define DATABASEKEYSTORAGE_H

#include "../keystoragebase.h"
#include "secretdatabase.h"

#define KEYSTORAGE_METHOD_DATABASE      1       // Store key entries in a local database.

class DatabaseKeyStorage : public KeyStorageBase
{
public:
    DatabaseKeyStorage();
    DatabaseKeyStorage(DatabaseKeyStorage &toCopy);
    ~DatabaseKeyStorage();

    int storageId();

    bool initKeyStorage();
    bool keyByIdentifier(const QString &identifier, KeyEntry &result);
    bool getAllKeys(std::vector<KeyEntry> &result);
    bool addKey(const KeyEntry &entry);
    bool updateKey(const KeyEntry &currentEntry, const KeyEntry &newEntry);
    bool deleteKeyByIdentifier(const QString &identifier);
    bool freeKeyStorage();

    DatabaseKeyStorage& operator=(const DatabaseKeyStorage& toCopy);

private:
    void copy(const DatabaseKeyStorage &toCopy);

    SecretDatabase mSecretDatabase;
};

#endif // DATABASEKEYSTORAGE_H
