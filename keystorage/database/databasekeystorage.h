#ifndef DATABASEKEYSTORAGE_H
#define DATABASEKEYSTORAGE_H

#include "../keystoragebase.h"
#include "secretdatabase.h"

class DatabaseKeyStorage : public KeyStorageBase
{
public:
    DatabaseKeyStorage();
    ~DatabaseKeyStorage();

    bool keyByIdentifier(const QString &identifier, KeyEntry &result);
    bool getAllKeys(std::vector<KeyEntry> &result);
    bool addKey(const KeyEntry &entry);
    bool updateKey(const KeyEntry &currentEntry, const KeyEntry &newEntry);

private:
    QString getHomeDirectory();

    SecretDatabase mSecretDatabase;
};

#endif // DATABASEKEYSTORAGE_H
