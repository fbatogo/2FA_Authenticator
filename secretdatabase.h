#ifndef SECRETDATABASE_H
#define SECRETDATABASE_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <vector>

#include "secretentry.h"

#define SECRETDATABASE_KEYTYPE_HEX      0
#define SECRETDATABASE_KEYTYPE_BASE32   1

#define SECRETDATABASE_OTPTYPE_TOTP     0
#define SECRETDATABASE_OTPTYPE_HOTP     1

class SecretDatabase
{
public:
    SecretDatabase();

    bool open(const QString &path);
    bool close();

    bool add(const QString &identifier, const QString &secret, int keyType = SECRETDATABASE_KEYTYPE_BASE32, int otpType = SECRETDATABASE_OTPTYPE_TOTP, int outNumberCount = 6);
    bool add(const SecretEntry &entry);

    bool update(const QString &identifier, const QString &secret, int keyType = SECRETDATABASE_KEYTYPE_BASE32, int otpType = SECRETDATABASE_OTPTYPE_TOTP, int outNumberCount = 6);
    bool update(const SecretEntry &entry);

    bool getByIdentifier(const QString &identifier, SecretEntry &result);

    bool getAll(std::vector<SecretEntry> &result);

    int schemaVersion(bool logError = true);

private:
    bool createBoundQuery(const QString &query, const SecretEntry &toBind, QSqlQuery &sqlQuery);
    bool queryToSecretEntry(const QSqlQuery &query, SecretEntry &result);

    bool upgradeSchema(int from);

    // Functions that are used to upgrade the schema to a newer version.  If the new version is more than
    // one step up from the current, we will iterate through all of the different version upgrades.
    bool upgradeToVersion1();

    QSqlDatabase mDatabase;
};

#endif // SECRETDATABASE_H
