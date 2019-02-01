#ifndef SECRETDATABASE_H
#define SECRETDATABASE_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <vector>

#include "../keyentry.h"

class SecretDatabase
{
public:
    SecretDatabase();
    ~SecretDatabase();

    bool open(const QString &path);
    bool close();
    bool isOpen();

    bool add(const KeyEntry &entry);

    bool update(const KeyEntry &currentEntry, const KeyEntry &newEntry);

    bool getByIdentifier(const QString &identifier, KeyEntry &result);

    bool getAll(std::vector<KeyEntry> &result);

    bool deleteByIdentifier(const QString &identifier);

    int schemaVersion(bool logError = true);

private:
    bool createBoundQuery(const QString &query, const KeyEntry &toBind, QSqlQuery &sqlQuery);
    bool queryToKeyEntry(const QSqlQuery &query, KeyEntry &result);
    bool queryEntryToString(const QSqlQuery &query, const QString &column, QString &result);
    bool queryEntryToInt(const QSqlQuery &query, const QString &column, int &result);

    bool upgradeSchema(int from);

    // Functions that are used to upgrade the schema to a newer version.  If the new version is more than
    // one step up from the current, we will iterate through all of the different version upgrades.
    bool upgradeToVersion1();

    QSqlDatabase mDatabase;
};

#endif // SECRETDATABASE_H
