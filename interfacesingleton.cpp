#include "interfacesingleton.h"

#include "logger.h"
#include "otpimpl/hexdecoder.h"
#include "otpimpl/base32coder.h"

InterfaceSingleton::InterfaceSingleton() :
    QObject(nullptr)
{
    if (!mKeyStorage.initStorage()) {
        LOG_ERROR("Unable to initialize the key storage!");
    }
}

/**
 * @brief InterfaceSingleton::getInstance - Get the singleton instance for this class.
 *
 * @return InterfaceSingleton pointer.
 */
InterfaceSingleton::~InterfaceSingleton()
{
    if (!mKeyStorage.freeStorage()) {
        LOG_ERROR("Unable to free the key storage!");
    }
}

InterfaceSingleton *InterfaceSingleton::getInstance()
{
    static InterfaceSingleton singleton;

    return &singleton;
}

/**
 * @brief InterfaceSingleton::getQmlSingleton - Return the singleton that will be used by
 *      the QML engine.
 *
 * @param engine - A pointer to the active QML engine.
 * @param scriptEngine - A pointer to the active QML script engine.
 *
 * @return QObject pointer to the singleton.
 */
QObject *InterfaceSingleton::getQmlSingleton(QQmlEngine *engine, QJSEngine *)
{
    InterfaceSingleton *cSingleton;

    cSingleton = getInstance();

    cSingleton->updateEngine(engine);

    QQmlEngine::setObjectOwnership(cSingleton, QQmlEngine::CppOwnership);

    return static_cast<QObject *>(cSingleton);
}

/**
 * @brief InterfaceSingleton::updateEngine - Save a pointer to the QML engine that was
 *      passed to us when we were inited.
 *
 * @param engine - A pointer to the engine provided by the QML engine.
 */
void InterfaceSingleton::updateEngine(QQmlEngine *engine)
{
    mQmlEngine = engine;
}

/**
 * @brief InterfaceSingleton::logDebug - Write a debug level log line from QML to our logger.
 *
 * @param logline - The log line to write.
 */
void InterfaceSingleton::logDebug(QString logline)
{
    LOG_DEBUG("[UI] " + logline);
}

/**
 * @brief InterfaceSingleton::logError - Write an error level log line from QML to our logger.
 *
 * @param logline - The log line to write.
 */
void InterfaceSingleton::logError(QString logline)
{
    LOG_ERROR("[UI] " + logline);
}

/**
 * @brief InterfaceSingleton::version - Return the version string for this version of
 *      the app.
 *
 * @return QString containing the version information for this app.
 */
QString InterfaceSingleton::version()
{
    return APP_VERSION;
}

/**
 * @brief InterfaceSingleton::calculateKeyEntries - Calculate the OTP values for all currently
 *      known key entry objects, and return them.
 *
 * @return UiKeyEntries pointer on success.  nullptr on failure.
 */
UiKeyEntries *InterfaceSingleton::calculateKeyEntries()
{
    QList<KeyEntry> allKeys;
    UiKeyEntries *result = nullptr;

    allKeys.clear();

    if (!mKeyStorage.getAllKeys(allKeys)) {
        LOG_ERROR("Unable to get all of the keys stored in key storage!");
    } else {
        // We need to convert all of the KeyEntries in to dynamic allocations.
        result = new UiKeyEntries();

        if (!result->populateEntries(allKeys)) {
            LOG_ERROR("Unable to convert the key entries to a format suitable for the UI! (1)");

            // Clean up.
            delete result;
            result = nullptr;
        }

        // Then, calculate all of the key entrie values.
        if (!result->calculateEntries()) {
            LOG_ERROR("Unrecoverable failure calculating key entries!");

            // Clean up.
            delete result;
            result = nullptr;
        }
    }

    return result;
}

/**
 * @brief InterfaceSingleton::keyEntries - Get the list of all key entries.
 *
 * @return UiKeyEntries object containing all of the secret key entries store in the
 *      data store.
 */
UiKeyEntries *InterfaceSingleton::keyEntries()
{
    QList<KeyEntry> allKeys;
    UiKeyEntries *result = nullptr;

    allKeys.clear();

    if (!mKeyStorage.getAllKeys(allKeys)) {
        LOG_ERROR("Unable to get all of the keys stored in key storage!");
    } else {
        // We need to convert all of the KeyEntries in to dynamic allocations.
        result = new UiKeyEntries();

        if (!result->populateEntries(allKeys)) {
            LOG_ERROR("Unable to convert the key entries to a format suitable for the UI! (1)");

            // Clean up.
            delete result;
            result = nullptr;
        }
    }

    return result;
}

/**
 * @brief InterfaceSingleton::keyEntryFromIdentifier - Locate, and return, a KeyEntry object based
 *      on the identifier value provided.
 *
 * @param identifier - The identifier that we want to look for.
 *
 * @return KeyEntry pointer to the requested identifier on success.  On failure, nullptr will be
 *      returned.
 */
KeyEntry *InterfaceSingleton::keyEntryFromIdentifier(const QString &identifier)
{
    KeyEntry result;

    if (identifier.isEmpty()) {
        LOG_ERROR("Unable to locate a key with an empty identifier!");
        return nullptr;
    }

    if (!mKeyStorage.keyByIdentifier(identifier, result)) {
        LOG_ERROR("Failed to locate the key entry for identifier : " + identifier);
        return nullptr;
    }

    if (!result.valid()) {
        LOG_ERROR("KeyEntry returned wasn't valid!");
        return nullptr;
    }

    // Return a pointerized copy of the KeyEntry.
    return new KeyEntry(result);
}

/**
 * @brief InterfaceSingleton::addKeyEntry - Add a new key entry to the key storage.
 *
 * @param identifier - The identifier (name of the site) that this key is for.
 * @param secret - The secret code that is used for the site named by the 'identifier'.
 * @param keyType - One of the KEYENTRY_KEYTYPE_* values from keyentry.h
 * @param otpType - One of the KEYENTRY_OTPTYPE_* values from keyentry.h
 * @param numberCount - The number of digits of the secret to show 6, 7, or 8.
 * @param algorithm - One of the KEYENTRY_ALG_* values from keyentry.h
 * @param period - The number of seconds a key should be valid for.
 * @param offset - The offset (in seconds) that should be used for the OTP.
 *
 * @return true if the key entry was stored to the data store.  false on error.
 */
bool InterfaceSingleton::addKeyEntry(QString identifier, QString secret, int keyType, int otpType, int numberCount, int algorithm, int period, int offset)
{
    KeyEntry toAdd;

    // Validate inputs.
    if (identifier.isEmpty()) {
        LOG_ERROR("Unable to add a key with an empty identifier!");
        return false;
    }

    if (secret.isEmpty()) {
        LOG_ERROR("Unable to add a key with an empty secret value!");
        return false;
    }

    if ((keyType < 0) || (keyType > KEYENTRY_KEYTYPE_MAX)) {
        LOG_ERROR("Unable to add a key with an invalid key encoding!");
        return false;
    }

    if ((otpType < 0) || (otpType > KEYENTRY_OTPTYPE_MAX)) {
        LOG_ERROR("Unable to add a key with an invalid OTP type!");
        return false;
    }

    if ((numberCount < 6) || (numberCount > 8)) {
        LOG_ERROR("Unable to add a key with a number count that isn't 6 through 8!");
        return false;
    }

    if ((algorithm < 0) || (algorithm > KEYENTRY_ALG_MAX)) {
        LOG_ERROR("Unable to use an algorithm with an invalid type!");
        return false;
    }

    if (period < 0) {
        LOG_ERROR("Unable to use a period length less than 0!");
        return false;
    }

    if (offset < 0) {
        LOG_ERROR("Unable to use an offset value less than 0!");
        return false;
    }

    // Populate the KeyEntry object that we want to write to the key storage method.
    toAdd.clear();
    toAdd.setIdentifier(identifier);
    toAdd.setSecret(secret);
    toAdd.setKeyType(keyType);
    toAdd.setOtpType(otpType);
    toAdd.setOutNumberCount(numberCount);
    toAdd.setAlgorithm(algorithm);
    toAdd.setTimeStep(period);
    toAdd.setTimeOffset(offset);

    if (!toAdd.valid()) {
        LOG_ERROR("Failed to create a valid KeyEntry object from the provided data!");
        return false;
    }

    // Pass it in to be handled.
    if (!mKeyStorage.addKey(toAdd)) {
        LOG_ERROR("Unable to add the key data to the key storage method!");
        return false;
    }
    LOG_DEBUG("Key was written to the key storage!");

    return true;
}

/**
 * @brief InterfaceSingleton::updateKeyEntry - Update an existing key entry.
 *
 * @param identifier - The identifier for the key to be updated.
 * @param secret - The secret value that should be associated with the provided identifier.
 * @param keyType - The way the key is encoded for the provided identifier.
 * @param otpType - The type of OTP value being used with the provided identifier.
 * @param numberCount - The number of digits shown for the OTP.
 * @param algorithm - One of the KEYENTRY_ALG_* values from keyentry.h
 * @param period - The number of seconds a key should be valid for.
 * @param offset - The offset (in seconds) that should be used for the OTP.
 *
 * @return true if the key entry was updated.  false on error.
 */
bool InterfaceSingleton::updateKeyEntry(QString identifier, QString secret, int keyType, int otpType, int numberCount, int algorithm, int period, int offset)
{
    KeyEntry toUpdate;

    // Validate inputs.
    if (identifier.isEmpty()) {
        LOG_ERROR("Unable to update a key with an empty identifier!");
        return false;
    }

    if (secret.isEmpty()) {
        LOG_ERROR("Unable to update a key with an empty secret value!");
        return false;
    }

    if ((keyType < 0) || (keyType > KEYENTRY_KEYTYPE_MAX)) {
        LOG_ERROR("Unable to update a key with an invalid key encoding!");
        return false;
    }

    if ((otpType < 0) || (otpType > KEYENTRY_OTPTYPE_MAX)) {
        LOG_ERROR("Unable to update a key with an invalid OTP type!");
        return false;
    }

    if ((numberCount < 6) || (numberCount > 8)) {
        LOG_ERROR("Unable to update a key with a number count that isn't 6 through 8!");
        return false;
    }

    if ((algorithm < 0) || (algorithm > KEYENTRY_ALG_MAX)) {
        LOG_ERROR("Unable to use an algorithm with an invalid type!");
        return false;
    }

    if (period < 0) {
        LOG_ERROR("Unable to use a period length less than 0!");
        return false;
    }

    if (offset < 0) {
        LOG_ERROR("Unable to use an offset value less than 0!");
        return false;
    }

    // Populate the KeyEntry object that we want to write to the key storage method.
    toUpdate.clear();
    toUpdate.setIdentifier(identifier);
    toUpdate.setSecret(secret);
    toUpdate.setKeyType(keyType);
    toUpdate.setOtpType(otpType);
    toUpdate.setOutNumberCount(numberCount);
    toUpdate.setAlgorithm(algorithm);
    toUpdate.setTimeStep(period);
    toUpdate.setTimeOffset(offset);

    return updateKeyEntry(&toUpdate);
}

/**
 * @brief InterfaceSingleton::updateKeyEntry - Update a KeyEntry in the database using
 *      a pointer to a KeyEntry object.
 *
 * @param toUpdate - A pointer to the KeyEntry object to update in the database.
 *
 * @return true if the database was updated.  false on error.
 */
bool InterfaceSingleton::updateKeyEntry(KeyEntry *toUpdate)
{
    KeyEntry *currentEntry;
    bool updated;

    // Validate inputs.
    if (toUpdate == nullptr) {
        LOG_ERROR("Unable to update a key entry.  The provided object is null!");
        return false;
    }

    if (!toUpdate->valid()) {
        LOG_ERROR("Failed to update KeyEntry object from the provided data!  The data is invalid!");
        return false;
    }

    // Locate the current entry for key specified by the identifier.
    currentEntry = keyEntryFromIdentifier(toUpdate->identifier());
    if (nullptr == currentEntry) {
        LOG_ERROR("Unable to locate the key entry for identifier '" + toUpdate->identifier() + "'!  Cannot update the key!");
        return false;
    }

    // Pass it in to be handled.
    updated = mKeyStorage.updateKey((*currentEntry), (*toUpdate));

    // Free the current entry.
    delete currentEntry;
    currentEntry = nullptr;

    if (!updated) {
        LOG_ERROR("Failed to update the key for identifier '" + toUpdate->identifier() + "'!");
        return false;
    }

    return true;
}

/**
 * @brief InterfaceSingleton::deleteKey - Given a key identifier, delete it from the
 *      key storage.
 *
 * @param identifier - The identifier of the key to delete.
 *
 * @return true if the key was deleted.  false on error.
 */
bool InterfaceSingleton::deleteKey(QString identifier)
{
    if (identifier.isEmpty()) {
        LOG_ERROR("Unable to delete a key with an empty identifier!");
        return false;
    }

    return mKeyStorage.deleteKeyByIdentifier(identifier);
}

/**
 * @brief InterfaceSingleton::incrementHotpCounter - Given the key identifier, increment
 *      the HOTP counter by 1.
 *
 * @param identifier - The identifier for the HOTP counter that we want to increment.
 */
void InterfaceSingleton::incrementHotpCounter(QString identifier)
{
    KeyEntry *toIncrement;

    if (identifier.isEmpty()) {
        LOG_ERROR("Cannot increment the HOTP counter for an empty identifier!");
        return;
    }

    // Find the KeyEntry to use.
    toIncrement = keyEntryFromIdentifier(identifier);
    if (toIncrement == nullptr) {
        LOG_ERROR("Unable to locate the key entry for identifier : " + identifier);
        return;
    }

    // Make sure the KeyEntry is an HOTP entry.
    if (toIncrement->otpType() != KEYENTRY_OTPTYPE_HOTP) {
        LOG_ERROR("Cannot increment the HOTP counter for non-HOTP key with the identifier : " + identifier);
        return;
    }

    // It is an HOTP type, so increment the counter.
    toIncrement->setHotpCounter(toIncrement->hotpCounter()+1);

    // Then, update the database entry.
    if (updateKeyEntry(toIncrement) == false) {
        LOG_ERROR("Failed to increment the HOTP counter!  The displayed value won't be updated.");
    }
}

/**
 * @brief InterfaceSingleton::isEncodedProperly - Make sure the secret is encoded
 *      properly.
 *
 * @param encodingType - One of the KEYENTRY_KEYTYPE_* values.
 * @param valueToCheck - The string that we want to check the encoding on.
 *
 * @return true if the string appears to be encoded properly.  false if it isn't.
 */
bool InterfaceSingleton::isEncodedProperly(int encodingType, const QString &valueToCheck)
{
    switch (encodingType) {
    case KEYENTRY_KEYTYPE_HEX:
        return HexDecoder::isHexEncoded(valueToCheck.toStdString());

    case KEYENTRY_KEYTYPE_BASE32:
        return Base32Coder::isBase32Encoded(valueToCheck.toStdString());
    }

    // If we get here, we don't understand the encodingType, so return false.
    LOG_ERROR("Unknown secret encoding type of '" + QString::number(encodingType) + "'!");
    return false;
}

/**
 * @brief InterfaceSingleton::haveZBar - Indicate to the UI if we were built with
 *  ZBar, or not.
 *
 * @return true if we were built with ZBar, false if not.
 */
bool InterfaceSingleton::haveZBar()
{
#ifndef NO_ZBAR
    return true;
#else
    return false;
#endif // NO_ZBAR
}

