#include "interfacesingleton.h"

#include "logger.h"

#define APP_VERSION "0.01"

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
            LOG_ERROR("Unable to convert the key entries to a format suitable for the UI!");

            // Clean up.
            delete result;
            result = nullptr;
        }
    }

    return result;
}

/**
 * @brief InterfaceSingleton::otpEntries - Get the list of all key entries, calculate the
 *      OTP value, and add it to the resulting list.
 *
 * @return UiOtpEntries pointer for all of the KeyEntry objects.
 */
UiOtpEntries *InterfaceSingleton::otpEntries()
{
    QList<KeyEntry> allKeys;
    UiOtpEntries *result = nullptr;

    allKeys.clear();

    if (!mKeyStorage.getAllKeys(allKeys)) {
        LOG_ERROR("Unable to get all of the keys stored in key storage!");
    } else {
        // We need to convert all of the KeyEntries in to dynamic allocations.
        result = new UiOtpEntries();

        if (!result->populateEntries(allKeys)) {
            LOG_ERROR("Unable to convert the key entries to a format suitable for the UI!");

            // Clean up.
            delete result;
            result = nullptr;
        }
    }

    return result;
}

/**
 * @brief InterfaceSingleton::addKeyEntry - Add a new key entry to the key storage.
 *
 * @param identifier - The identifier (name of the site) that this key is for.
 * @param secret - The secret code that is used for the site named by the 'identifier'.
 * @param keyType - One of the KEYENTRY_KEYTYPE_* values from keyentry.h
 * @param otpType - One of the KEYENTRY_OTPTYPE_* values from keyentry.h
 * @param numberCount - The number of digits of the secret to show 6, 7, or 8.
 *
 * @return true if the key entry was stored to the data store.  false on error.
 */
bool InterfaceSingleton::addKeyEntry(QString identifier, QString secret, int keyType, int otpType, int numberCount)
{
    KeyEntry toAdd;

    // Populate the KeyEntry object that we want to write to the key storage method.
    toAdd.clear();
    toAdd.setIdentifier(identifier);
    toAdd.setSecret(secret);
    toAdd.setKeyType(keyType);
    toAdd.setOtpType(otpType);
    toAdd.setOutNumberCount(numberCount);

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
 * @brief InterfaceSingleton::deleteKey - Given a key identifier, delete it from the
 *      key storage.
 *
 * @param identifier - The identifier of the key to delete.
 *
 * @return true if the key was deleted.  false on error.
 */
bool InterfaceSingleton::deleteKey(QString identifier)
{
    return mKeyStorage.deleteKeyByIdentifier(identifier);
}

/**
 * @brief InterfaceSingleton::haveZbar - Indicate to the UI if we were built with
 *  ZBar, or not.
 *
 * @return true if we were built with ZBar, false if not.
 */
bool InterfaceSingleton::haveZbar()
{
#ifndef NO_ZBAR
    return true;
#else
    return false;
#endif // NO_ZBAR
}

