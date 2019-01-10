#ifndef SECRETDATABASE_H
#define SECRETDATABASE_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "secretentry.h"

class SecretDatabase
{
public:
    SecretDatabase();

    bool open(const QString &path);
    bool close();

    bool add(const QString &identifier, const QString &secret, int keyType, int otpType, int outNumberCount);
    bool add(const SecretEntry &entry);

    bool getByIdentifier(const QString &identifier, SecretEntry &result);

private:
    int schemaVersion();
    bool upgradeSchema(int from);
    int fixSchemaTable(const QSqlQuery &queryData);

    // Functions that are used to upgrade the schema to a newer version.  If the new version is more than
    // one step up from the current, we will iterate through all of the different version upgrades.
    bool upgradeToVersion1();

    QSqlDatabase mDatabase;
};

#endif // SECRETDATABASE_H
