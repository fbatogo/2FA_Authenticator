#include "secretdatabase.h"

#include <QFileInfo>
#include <QSqlQuery>
#include <QSqlError>

// The schema version expected/used by this implementation of the app.
#define SECRETDATABASE_SCHEMA_VERSION       1

SecretDatabase::SecretDatabase()
{
    // Set up the database object to use SQLITE.
    mDatabase = QSqlDatabase::addDatabase("QSQLITE");
}

/**
 * @brief SecretDatabase::open - Open the database so that we can work with it.
 *
 * @return true if the database was opened.  false on error.
 */
bool SecretDatabase::open(const QString &path)
{
    bool createSchema = false;
    int sVer;

    if (path.isEmpty()) {
        qDebug("The database path to open was empty!");
        return false;
    }

    // Set the database path name.
    mDatabase.setDatabaseName(path);

    if (!mDatabase.open()) {
        qDebug("Failed to open or create the database file : %s", path.toStdString().c_str());
        return false;
    }

    // See if we need to create or update the schema.
    sVer = schemaVersion();
    if (sVer != SECRETDATABASE_SCHEMA_VERSION) {
        // Attempt to create the schema for the database.
        return upgradeSchema(sVer);
    }

    // Otherwise, things are good.
    return true;
}

/**
 * @brief SecretDatabase::close - Close the database.
 *
 * @return true if the database was closed.  false on error.
 */
bool SecretDatabase::close()
{
    if (!mDatabase.isOpen()) {
        qDebug("Unable to close a database that isn't open!");
        return false;
    }

    mDatabase.close();

    return true;            // I guess a close can't fail?
}

/**
 * @brief SecretDatabase::add - Add a secret key entry to the database.
 *
 * @param identifier - The 'identifier' (usually the name of the service for this secret) for the key.
 * @param secret - The secret value that is used to generate OTP values.
 * @param keyType - One of the DATABASE_KEYTYPE_* values which indicates how the secret is encoded.
 * @param otpType - One of the DATABASE_OTPTPYE_* values which indicates the algorithm that should be used
 *      to generate the OTP value.
 * @param outNumberCount - The number of digits that should be shown using this secret value.
 *
 * @return true if the key entry was added.  false on error.
 */
bool SecretDatabase::add(const QString &identifier, const QString &secret, int keyType, int otpType, int outNumberCount)
{

}

/**
 * @brief SecretDatabase::add - Convenience method for writing a SecretEntry to the database.
 *
 * @param entry - The SecretEntry to write to the database.
 *
 * @return true if the entry was written to the database.  false on error.
 */
bool SecretDatabase::add(const SecretEntry &entry)
{
    return add(entry.identifier(), entry.secret(), entry.keyType(), entry.otpType(), entry.outNumberCount());
}

/**
 * @brief SecretDatabase::getByIdentifier - Query the database for a specific identifier.
 *
 * @param identifier - The identifier to look for.
 * @param result[OUT] - If this function returns true, this SecretEntry will contain the data
 *      for the identifier.
 *
 * @return true if the identifier was found and returned.  false on error.
 */
bool SecretDatabase::getByIdentifier(const QString &identifier, SecretEntry &result)
{

}

/**
 * @brief SecretDatabase::schemaVersion - Get the schema version that the database is currently using.
 *
 * @return int containing the schema version being used.  If the schema version can't be identified, -1 will
 *      be returned.
 */
int SecretDatabase::schemaVersion()
{
    QSqlQuery query("SELECT * from schemaVersion");

    // Execute the query.
    if (!query.exec()) {
        qDebug("Failed to get the schema version!  Error (%d) : %s", query.lastError().type(), query.lastError().text().toStdString().c_str());
        return -1;
    }

    // We got a result.  Check it.
    if (query.size() == 0) {
        // The database is technically invalid, but attempt to save it anyway.
        return 0;
    }

    if (query.size() > 1) {
        // There are too many schema entries in the database.  Attempt to fix it.
        return fixSchemaTable(query);
    }

    // XXX Read the schema version.
}

/**
 * @brief SecretDatabase::upgradeSchema - Handle upgrading the schema for the current database to whatever the most current
 *      schema version is.
 *
 * @param from - A number that indicates the current schema version of the database.
 *
 * @return true if the database schema was upgraded to the most current version.  false on error.
 */
bool SecretDatabase::upgradeSchema(int from)
{
    qDebug("Need to upgrade the schmea from version %d to %d...", from, SECRETDATABASE_SCHEMA_VERSION);
}

/**
 * @brief SecretDatabase::fixSchemaTable - The schema table has more than one entry in it.  So, we will attempt to figure out
 *      which version is the most "correct", and clean up the schema table accordingly.
 *
 * @param queryData - The data that was returned from the query of the schema table.
 *
 * @return int containing the schema version the cleaned up table has.  On error, -1 will be returned.
 */
int SecretDatabase::fixSchemaTable(const QSqlQuery &queryData)
{

}

/**
 * @brief SecretDatabase::upgradeToVersion1 - Handle "upgrading" to a database with a version 1 schema.  This call basically
 *      creates the base tables the database uses.
 *
 * @return true if the schema was upgraded.  false on error.
 */
bool SecretDatabase::upgradeToVersion1()
{
    QSqlQuery query;

    // Start by creating the table for the data and the schema.
    if (!query.exec("CREATE TABLE secretData(identifier text primary key, secret text, keyType int, otpType int, outNumberCount int)")) {
        qDebug("Failed to create the secret key table in the database!  Error (%d) : %s", query.lastError().type(), query.lastError().text().toStdString().c_str());
        return false;
    }

    if (!query.exec("CREATE TABLE schemaVersion(version int)")) {
        qDebug("Failed to create the schema table in the database!  Error (%d) : %s", query.lastError().type(), query.lastError().text().toStdString().c_str());
        return false;
    }

    // Finally, set the schema version to be 1.
    if (!query.exec("INSERT into schemaVersion set version=" + QString::number(1))) {
        qDebug("Failed to set the schema version in the database!  Error (%d) : %s", query.lastError().type(), query.lastError().text().toStdString().c_str());
        return false;
    }

    return true;
}

