#include "secretdatabase.h"

#include <QFileInfo>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>

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
    sVer = schemaVersion(false);
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
 * @brief SecretDatabase::add - Convenience call for adding a secret entry to the database using
 *      discrete values.
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
    SecretEntry toWrite;

    // Store the values in to our SecretEntry.
    toWrite.setIdentifier(identifier);
    toWrite.setSecert(secret);
    toWrite.setKeyType(keyType);
    toWrite.setOtpType(otpType);
    toWrite.setOutNumberCount(outNumberCount);

    // Then, call the other add instance.
    return add(toWrite);
}

/**
 * @brief SecretDatabase::add - Add a new SecretEntry to the database.
 *
 * @param entry - The SecretEntry to write to the database.
 *
 * @return true if the entry was written to the database.  false on error.
 */
bool SecretDatabase::add(const SecretEntry &entry)
{
    QSqlQuery query;
    SecretEntry foundEntry;

    // Make sure the entry we are going to write is valid.
    if (!entry.valid()) {
        qDebug("The SecretEntry provided is invalid!");
        return false;
    }

    // Make sure it doesn't already exist.
    if (getByIdentifier(entry.identifier(), foundEntry)) {
        // Double check it is what we expect.
        if (foundEntry.valid()) {
            if (foundEntry.identifier() == entry.identifier()) {
                qDebug("The entry for identifier '%s' already exists!  Please us update()!", entry.identifier().toStdString().c_str());
                return false;
            }
        }
    }

    if (!createBoundQuery("INSERT into secretData (identifier, secret, keyType, otpType, outNumberCount) VALUES (:identifier, :secret, :keyType, :otpType, :outNumberCount)", entry, query)) {
        // Already logged an error.  Just return.
        return false;
    }

    // Execute the query.
    if (!query.exec()) {
        qDebug("Failed to write the data to the database!");
        return false;
    }

    return true;
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
    QSqlQuery query;

    if (identifier.isEmpty()) {
        qDebug("Cannot search with an empty identifier!");
        return false;
    }

    // Make the query.
    if (!query.exec("SELECT * from secretData where identifier='" + identifier + "'")) {
        qDebug("Unable to query the database by identifier!");
        return false;
    }

    // Convert the query data to the resulting SecretEntry object.
    return queryToSecretEntry(query, result);
}

/**
 * @brief SecretDatabase::getAll - Read all of the entries in the database and return
 *      them.
 *
 * @param result[OUT] - If this method returns true, this vector will contain all of the
 *      SecretEntry rows from the database.
 *
 * @return true if the rows were read.  false on error.
 */
bool SecretDatabase::getAll(std::vector<SecretEntry> &result)
{
    QSqlQuery query;
    SecretEntry entry;

    // Clear out the result vector.
    result.clear();

    if (!query.exec("SELECT * from secretData")) {
        qDebug("Unable to query the database for all of the secret entries!");
        return false;
    }

    // Iterate each row, convert it to a SecretEntry, and stuff it in the result vector.
    while (query.next()) {
        // XXX FINISH!
    }

    return true;
}

/**
 * @brief SecretDatabase::schemaVersion - Get the schema version that the database is currently using.
 *
 * @param logError - If true, then error messages will be logged when the schema can't be
 *      read.  Default is true.
 *
 * @return int containing the schema version being used.  If the schema version can't be identified, -1 will
 *      be returned.
 */
int SecretDatabase::schemaVersion(bool logError)
{
    QSqlQuery query("SELECT * from schemaVersion");
    int verIdx;
    int result;
    bool ok;

    // Execute the query.
    if (!query.exec()) {
        if (logError) {
            qDebug("Failed to get the schema version!  Error (%d) : %s", query.lastError().type(), query.lastError().text().toStdString().c_str());
        }
        return -1;
    }

    // We got a result.  Check it.
    if (query.size() == 0) {
        // The database is technically invalid, but attempt to save it anyway.
        return 0;
    }

    if (query.size() > 1) {
        // There are too many schema entries in the database.  Attempt to fix it.
        qDebug("Too many schema table entries!");
        return -1;
    }

    // Get the index for the version value.
    verIdx = query.record().indexOf("version");

    if (verIdx < 0) {
        if (logError) {
            qDebug("Unable to determine the column index for the 'version' column!");
        }
        return -1;
    }

    if (!query.next()) {
        if (logError) {
            qDebug("SecretDatabase didn't return any records!?");       // This should be impossible.
        }
        return -1;
    }

    // Return the version number.
    result = query.value(verIdx).toInt(&ok);
    if (!ok) {
        if (logError) {
            qDebug("Unable to convert result value to an integer!");
        }
        return -1;
    }

    qDebug("Schema version is %d.", result);
    return result;
}

/**
 * @brief SecretDatabase::createBoundQuery - Update a QSqlQuery object using the provided
 *      query string, and bound with the values from the provided SecretEntry.
 *
 * @param query - The SQL query to execute using the data in the 'toBind' variable.
 * @param toBind - A SecretEntry that contains the data we want to bind using the
 *      query defined in 'query'.
 * @param sqlQuery[OUT] - If this method returns true, this variable will be updated with
 *      the query and binding data provided.
 *
 * @return true if the QSqlQuery was updated with the query and binding data.  false on
 *      error.
 */
bool SecretDatabase::createBoundQuery(const QString &query, const SecretEntry &toBind, QSqlQuery &sqlQuery)
{
    if (!sqlQuery.prepare(query)) {
        qDebug("Unable to prepare the query : %s", query.toStdString().c_str());
        return false;
    }

    // Bind the values provided.
    sqlQuery.bindValue(":identifier", toBind.identifier());
    sqlQuery.bindValue(":secret", toBind.secret());
    sqlQuery.bindValue(":keyType", toBind.keyType());
    sqlQuery.bindValue(":otpType", toBind.otpType());
    sqlQuery.bindValue(":outNumberCount", toBind.outNumberCount());

    return true;
}

/**
 * @brief SecretDatabase::queryToSecretEntry - Read the SecretEntry values from the provided
 *      QSqlQuery object, and store them in to a SecretEntry object.
 *
 * @param query - The QSqlQuery object to read the data from.
 * @param result[OUT] - If this method returns true, this variable will contain the values
 *      read from the QSqlQuery.
 *
 * @return true if the values were read from the QSqlQuery and stored in the SecretEntry.
 *      false on error.
 */
bool SecretDatabase::queryToSecretEntry(const QSqlQuery &query, SecretEntry &result)
{
    int idx;

    return false;
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
    // See if we need to upgrade to version 1.
    if (from < 1) {
        if (!upgradeToVersion1()) {
            qDebug("Failed to upgrade the schema to version 1!");
            return false;
        }
    }

    if (from > SECRETDATABASE_SCHEMA_VERSION) {
        qDebug("Attempting to upgrade from an invalid schema version! (%d)", from);
        return false;
    }

    return true;
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
    if (!query.exec("INSERT into schemaVersion (version) values (" + QString::number(1) + ")")) {
        qDebug("Failed to set the schema version in the database!  Error (%d) : %s", query.lastError().type(), query.lastError().text().toStdString().c_str());
        return false;
    }

    return true;
}

