#include "secretdatabase.h"

#include <logger.h>

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
        LOG_ERROR("The database path to open was empty!");
        return false;
    }

    // Set the database path name.
    mDatabase.setDatabaseName(path);

    if (!mDatabase.open()) {
        LOG_ERROR("Failed to open or create the database file : " + path);
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
        LOG_ERROR("Unable to close a database that isn't open!");
        return false;
    }

    mDatabase.close();

    return true;            // I guess a close can't fail?
}

/**
 * @brief SecretDatabase::add - Add a new KeyEntry to the database.
 *
 * @param entry - The KeyEntry to write to the database.
 *
 * @return true if the entry was written to the database.  false on error.
 */
bool SecretDatabase::add(const KeyEntry &entry)
{
    QSqlQuery query;
    KeyEntry foundEntry;

    // Make sure the entry we are going to write is valid.
    if (!entry.valid()) {
        LOG_ERROR("The KeyEntry provided is invalid!");
        return false;
    }

    // Make sure it doesn't already exist.
    if (getByIdentifier(entry.identifier(), foundEntry)) {
        // Double check it is what we expect.
        if (foundEntry.valid()) {
            if (foundEntry.identifier() == entry.identifier()) {
                LOG_ERROR("The entry for identifier '" + entry.identifier() + "' already exists!  Please us update()!");
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
        LOG_ERROR("Failed to write the data to the database!");
        return false;
    }

    return true;
}

/**
 * @brief SecretDatabase::update - Update the database entry with the new KeyEntry data.
 *
 * @param currentEntry - The current entry in the database that we will look for to update.
 * @param newEntry - How the entry should look after being updated in the database.
 *
 * @return true if the entry was updated.  false on error.
 */
bool SecretDatabase::update(const KeyEntry &currentEntry, const KeyEntry &newEntry)
{
    QSqlQuery query;
    KeyEntry foundEntry;

    // Make sure that the data provided is valid.
    if ((!currentEntry.valid()) || (!newEntry.valid())) {
        LOG_ERROR("One of the KeyEntry values provided was invalid while trying to update the database!");
        return false;
    }

    // Make sure the currentEntry *DOES* exist in the database.
    if (!getByIdentifier(currentEntry.identifier(), foundEntry)) {
        LOG_WARNING("The current entry doesn't exist.  Did you mean to use add()?");
        return false;
    }

    if (!createBoundQuery("UPDATE secretData set identifier=:identifier, secret=:secret, keyType=:keyType, otpType=:otpType, outNumberCount=:outNumberCount", newEntry, query)) {
        // Already logged an error.  Just return.
        return false;
    }

    // Execute the query.
    if (!query.exec()) {
        LOG_ERROR("Failed to update the key entry data in the database!");
        return false;
    }

    return true;
}

/**
 * @brief SecretDatabase::getByIdentifier - Query the database for a specific identifier.
 *
 * @param identifier - The identifier to look for.
 * @param result[OUT] - If this function returns true, this KeyEntry will contain the data
 *      for the identifier.
 *
 * @return true if the identifier was found and returned.  false on error.
 */
bool SecretDatabase::getByIdentifier(const QString &identifier, KeyEntry &result)
{
    QSqlQuery query;

    if (identifier.isEmpty()) {
        LOG_ERROR("Cannot search with an empty identifier!");
        return false;
    }

    // Make the query.
    if (!query.exec("SELECT * from secretData where identifier='" + identifier + "'")) {
        LOG_ERROR("Unable to query the database by identifier!");
        return false;
    }

    if (!query.next()) {
        LOG_DEBUG("No entries returned when searching for the identifier '" + identifier + "'.");
        return false;
    }

    // Convert the query data to the resulting KeyEntry object.
    return queryToKeyEntry(query, result);
}

/**
 * @brief SecretDatabase::getAll - Read all of the entries in the database and return
 *      them.
 *
 * @param result[OUT] - If this method returns true, this vector will contain all of the
 *      KeyEntry rows from the database.
 *
 * @return true if the rows were read.  false on error.
 */
bool SecretDatabase::getAll(std::vector<KeyEntry> &result)
{
    QSqlQuery query;
    KeyEntry entry;

    // Clear out the result vector.
    result.clear();

    if (!query.exec("SELECT * from secretData")) {
        LOG_ERROR("Unable to query the database for all of the secret entries!");
        return false;
    }

    // Iterate each row, convert it to a KeyEntry, and stuff it in the result vector.
    while (query.next()) {
        if (!queryToKeyEntry(query, entry)) {
            LOG_ERROR("Unable to convert a database result to a KeyEntry.");
            return false;
        }

        // Add it to the result list.
        result.push_back(entry);
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
        LOG_CONDITIONAL_ERROR(logError, "Failed to get the schema version!  Error (" + QString::number(query.lastError().type()) + ") : " + query.lastError().text());
        return -1;
    }

    // We got a result.  Check it.
    if (query.size() == 0) {
        // The database is technically invalid, but attempt to save it anyway.
        return 0;
    }

    if (query.size() > 1) {
        // There are too many schema entries in the database.  Attempt to fix it.
        LOG_ERROR("Too many schema table entries!");
        return -1;
    }

    // Get the index for the version value.
    verIdx = query.record().indexOf("version");

    if (verIdx < 0) {
        LOG_CONDITIONAL_ERROR(logError, "Unable to determine the column index for the 'version' column!");
        return -1;
    }

    if (!query.next()) {
        LOG_CONDITIONAL_ERROR(logError, "SecretDatabase didn't return any records!?");       // This should be impossible.
        return -1;
    }

    // Return the version number.
    result = query.value(verIdx).toInt(&ok);
    if (!ok) {
        LOG_CONDITIONAL_ERROR(logError, "Unable to convert result value to an integer!");
        return -1;
    }

    LOG_DEBUG("Schema version is " + QString::number(result) + ".");
    return result;
}

/**
 * @brief SecretDatabase::createBoundQuery - Update a QSqlQuery object using the provided
 *      query string, and bound with the values from the provided KeyEntry.
 *
 * @param query - The SQL query to execute using the data in the 'toBind' variable.
 * @param toBind - A KeyEntry that contains the data we want to bind using the
 *      query defined in 'query'.
 * @param sqlQuery[OUT] - If this method returns true, this variable will be updated with
 *      the query and binding data provided.
 *
 * @return true if the QSqlQuery was updated with the query and binding data.  false on
 *      error.
 */
bool SecretDatabase::createBoundQuery(const QString &query, const KeyEntry &toBind, QSqlQuery &sqlQuery)
{
    if (!sqlQuery.prepare(query)) {
        LOG_ERROR("Unable to prepare the query : " + query);
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
 * @brief SecretDatabase::queryToKeyEntry - Read the KeyEntry values from the provided
 *      QSqlQuery object, and store them in to a KeyEntry object.
 *
 * @param query - The QSqlQuery object to read the data from.
 * @param result[OUT] - If this method returns true, this variable will contain the values
 *      read from the QSqlQuery.
 *
 * @return true if the values were read from the QSqlQuery and stored in the KeyEntry.
 *      false on error.
 */
bool SecretDatabase::queryToKeyEntry(const QSqlQuery &query, KeyEntry &result)
{
    QString tempStr;
    int tempInt;

    if (!queryEntryToString(query, "identifier", tempStr)) {
        LOG_ERROR("Failed to get the identifier from the database query row!");
        return false;
    }

    result.setIdentifier(tempStr);

    if (!queryEntryToString(query, "secret", tempStr)) {
        LOG_ERROR("Failed to get the secret from the database query row!");
        return false;
    }

    result.setSecert(tempStr);

    if (!queryEntryToInt(query, "keyType", tempInt)) {
        LOG_ERROR("Failed to get the key type from the database query row!");
        return false;
    }

    result.setKeyType(tempInt);

    if (!queryEntryToInt(query, "otpType", tempInt)) {
        LOG_ERROR("Failed to get the OTP type from the database query row!");
        return false;
    }

    result.setOtpType(tempInt);

    if (!queryEntryToInt(query, "outNumberCount", tempInt)) {
        LOG_ERROR("Failed to get the number of numbers to return from the database query row!");
        return false;
    }

    result.setOutNumberCount(tempInt);

    return true;
}

/**
 * @brief SecretDatabase::queryEntryToString - Read a string entry from the specified column.
 *
 * @param query - The QSqlQuery that we want to read a string from.
 * @param column - The name of the column that we want to read.
 * @param result[OUT] - If this function returns true, this variable will contain the string that
 *      was read.
 *
 * @return true if the column data was read.  false on error.
 */
bool SecretDatabase::queryEntryToString(const QSqlQuery &query, const QString &column, QString &result)
{
    int idx;

    idx = query.record().indexOf(column);
    if (idx < 0) {
        LOG_ERROR("Failed to get the index for the " + column + " field of a query!");
        return false;
    }

    result = query.value(idx).toString();

    return true;
}

/**
 * @brief SecretDatabase::queryEntryToInt - Read an integer entry from the specified column.
 *
 * @param query - The QSqlQuery that we want to read a string from.
 * @param column - The name of the column that we want to read.
 * @param result[OUT] - If this function returns true, this variable will contain the string that
 *      was read.
 *
 * @return true if the column data was read.  false on error.
 */
bool SecretDatabase::queryEntryToInt(const QSqlQuery &query, const QString &column, int &result)
{
    int idx;
    bool ok;

    idx = query.record().indexOf(column);
    if (idx < 0) {
        LOG_ERROR("Failed to get the index for the " + column + " field of a query!");
        return false;
    }

    result = query.value(idx).toInt(&ok);
    if (!ok) {
        LOG_ERROR("Unable to determine the integer value of the " + column + " field.");
        return false;
    }

    return true;
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
            LOG_ERROR("Failed to upgrade the schema to version 1!");
            return false;
        }
    }

    if (from > SECRETDATABASE_SCHEMA_VERSION) {
        LOG_ERROR("Attempting to upgrade from an invalid schema version! (" + QString::number(from) + ")");
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
        LOG_ERROR("Failed to create the secret key table in the database!  Error (" + QString::number(query.lastError().type()) + ") : " + query.lastError().text());
        return false;
    }

    if (!query.exec("CREATE TABLE schemaVersion(version int)")) {
        LOG_ERROR("Failed to create the schema table in the database!  Error (" + QString::number(query.lastError().type()) + ") : " + query.lastError().text());
        return false;
    }

    // Finally, set the schema version to be 1.
    if (!query.exec("INSERT into schemaVersion (version) values (" + QString::number(1) + ")")) {
        LOG_ERROR("Failed to set the schema version in the database!  Error (" + QString::number(query.lastError().type()) + ") : " + query.lastError().text());
        return false;
    }

    return true;
}

