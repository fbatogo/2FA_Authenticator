#include "keyentriessingleton.h"

#include <QTimer>
#include "logger.h"
#include "otp/otphandler.h"

KeyEntriesSingleton::KeyEntriesSingleton(QObject *parent) :
    QObject(parent)
{
    mEntryList.clear();

    if (!mKeyStorage.initStorage()) {
        LOG_ERROR("Unable to initialize the key storage!");
    }

    // Configure our timer as a single shot timer.
    mUpdateTimer.setSingleShot(true);

    // Connect the QTimer slots and signals.
    connect(&mUpdateTimer, SIGNAL(timeout()), this, SLOT(slotUpdateOtpValues()));

    populateEntries();
}

KeyEntriesSingleton::~KeyEntriesSingleton()
{
    // Disconnect the QTimer slots and signals.
    disconnect(&mUpdateTimer, SIGNAL(timeout()), this, SLOT(slotUpdateOtpValues()));

    clear();

    if (!mKeyStorage.freeStorage()) {
        LOG_ERROR("Unable to free the key storage!");
    }
}

/**
 * @brief KeyEntriesSingleton::getInstance - Return the C++ singleton for this class.
 *
 * @return KeyEntriesSingleton pointer.
 */
KeyEntriesSingleton *KeyEntriesSingleton::getInstance()
{
    static KeyEntriesSingleton singleton;

    return &singleton;
}

/**
 * @brief KeyEntriesSingleton::getQmlSingleton - Return the QML singleton for this class.
 *
 * @param engine - Not used.
 * @param scriptEngine - Not used.
 *
 * @return QObject pointer to the singleton to use with QML.
 */
QObject *KeyEntriesSingleton::getQmlSingleton(QQmlEngine *, QJSEngine *)
{
    KeyEntriesSingleton *cSingleton;

    cSingleton = getInstance();

    QQmlEngine::setObjectOwnership(cSingleton, QQmlEngine::CppOwnership);

    return static_cast<QObject *>(cSingleton);
}

/**
 * @brief KeyEntriesSingleton::clear - Erase all of the KeyEntries in this object, and free
 *      the associated memory.
 */
void KeyEntriesSingleton::clear()
{
    // Clean up the memory used in our internal list.
    for (int i = 0; i < mEntryList.size(); i++) {
        if (mEntryList.at(i) != nullptr) {
            delete mEntryList.at(i);
        }
    }

    // Then, clear the list container.
    mEntryList.clear();
}

/**
 * @brief KeyEntriesSingleton::entryParametersAreValid - Verify that the parameters provided for use in the add and update entry calls are valid.
 *
 * @param identifier
 * @param secret
 * @param keyType
 * @param otpType
 * @param numberCount
 * @param algorithm
 * @param period
 * @param offset
 *
 * @return true if the parameters are all valid.  false if any aren't.
 */
bool KeyEntriesSingleton::entryParametersAreValid(const QString &addUpdate, QString identifier, QString secret, size_t keyType, size_t otpType, int numberCount, size_t algorithm, int period, int offset)
{
    // Validate inputs.
    if (identifier.isEmpty()) {
        LOG_ERROR("Unable to " + addUpdate + " a key with an empty identifier!");
        return false;
    }

    if (secret.isEmpty()) {
        LOG_ERROR("Unable to " + addUpdate + " a key with an empty secret value!");
        return false;
    }

    if (keyType > KEYENTRY_KEYTYPE_MAX) {
        LOG_ERROR("Unable to " + addUpdate + " a key with an invalid key encoding!");
        return false;
    }

    if (otpType > KEYENTRY_OTPTYPE_MAX) {
        LOG_ERROR("Unable to " + addUpdate + " a key with an invalid OTP type!");
        return false;
    }

    if ((numberCount < 6) || (numberCount > 8)) {
        LOG_ERROR("Unable to " + addUpdate + " a key with a number count that isn't 6 through 8!");
        return false;
    }

    if (algorithm > KEYENTRY_ALG_MAX) {
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

    return true;
}

/**
 * @brief KeyEntriesSingleton::populateEntries - Read all of the known KeyEntry objects from
 *      key storage, and store them in memory in this object.
 *
 * @return true if the entries were populated as expected.  false on error.
 */
bool KeyEntriesSingleton::populateEntries()
{
    QList<KeyEntry> allKeys;
    bool result;
    KeyEntry *temp;

    allKeys.clear();

    if (!mKeyStorage.getAllKeys(allKeys)) {
        LOG_ERROR("Unable to get all of the keys stored in key storage!");
        return false;
    }

    // We need to convert all of the KeyEntries in to dynamic allocations.
    for (auto &keyEntry : allKeys) {
        temp = new KeyEntry(keyEntry);
        QQmlEngine::setObjectOwnership(temp, QQmlEngine::CppOwnership);

        mEntryList.push_back(temp);
    }

    // Calculate the codes to show.
    result = calculateEntries();

    // Start the update timer.
    if ((result) && (!updateTimer())) {
        LOG_ERROR("Failed to start the OTP update timer!");
        return false;
    }

    return result;
}

/**
 * @brief KeyEntriesSingleton::calculateEntries - Iterate the list of KeyEntries, and calculate
 *      the OTP value for each one.
 *
 * @return true if the entries were calculated.  false on a horrible, unrecoverable error.
 */
bool KeyEntriesSingleton::calculateEntries()
{
    for (int i = 0; i < mEntryList.size(); i++) {
        OtpHandler::calculateOtpForKeyEntry(mEntryList.at(i));
    }

    return true;
}

/**
 * @brief KeyEntriesSingleton::addKeyEntry - Add a new key entry to the key storage.
 *
 * @param identifier - The identifier (name of the site) that this key is for.
 * @param issuer - The issuer of this key.
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
bool KeyEntriesSingleton::addKeyEntry(const QString &identifier, const QString &issuer, const QString &secret, int keyType, int otpType, int numberCount, int algorithm, int period, int offset)
{
    KeyEntry toAdd;

    if (!entryParametersAreValid("add", identifier, secret, keyType, otpType, numberCount, algorithm, period, offset)) {
        return false;
    }

    // Populate the KeyEntry object that we want to write to the key storage method.
    toAdd.clear();
    toAdd.setIdentifier(identifier);
    toAdd.setIssuer(issuer);
    toAdd.setSecret(ByteArray(secret.toStdString()));
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
    if (!addKeyEntry(toAdd)) {
        LOG_ERROR("Unable to add the key data to the key storage method!");
        return false;
    }
    LOG_DEBUG("New key was added!");

    return true;
}

/**
 * @brief KeyEntriesSingleton::addKeyEntry - Add a key entry to the KeyStorage object, and then
 *      add it to the in-memory list, followed by emitting the fullUpdate() signal to let
 *      the UI know it needs to updated everything.
 *
 * @param toAdd - The KeyEntry object that we want to add to both the KeyStorage object, and
 *      the in memory list.
 *
 * @return true if the KeyEntry object was added to both the KeyStorage object and the
 *      in-memory list.  false on error.
 */
bool KeyEntriesSingleton::addKeyEntry(const KeyEntry &toAdd)
{
    if (!mKeyStorage.addKey(toAdd)) {
        LOG_ERROR("Unable to add a new KeyEntry to the key storage!");
        return false;
    }

    if (!addKeyEntryInMemory(toAdd)) {
        LOG_ERROR("Unable to add the new KeyEntry to the in-memory list.  (But, it was written to the key storage!");
        return false;
    }

    // Emit the update signal.
    emit fullRefresh();

    // Everything is good!
    return true;
}

/**
 * @brief KeyEntrySingleton::updateKeyEntry - Update an existing key entry.
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
bool KeyEntriesSingleton::updateKeyEntry(KeyEntry *currentEntry, QString identifier, QString secret, int keyType, int otpType, int numberCount, int algorithm, int period, int offset)
{
    KeyEntry toUpdate;

    if (!entryParametersAreValid("update", identifier, secret, keyType, otpType, numberCount, algorithm, period, offset)) {
        return false;
    }

    // Populate the KeyEntry object that we want to write to the key storage method.
    toUpdate.clear();
    toUpdate.setIdentifier(identifier);
    toUpdate.setSecret(ByteArray(secret.toStdString()));
    toUpdate.setKeyType(keyType);
    toUpdate.setOtpType(otpType);
    toUpdate.setOutNumberCount(numberCount);
    toUpdate.setAlgorithm(algorithm);
    toUpdate.setTimeStep(period);
    toUpdate.setTimeOffset(offset);

    return updateKeyEntry((*currentEntry), toUpdate);
}

/**
 * @brief KeyEntriesSingleton::updateKeyEntry - Update a key entry both in memory, and in the key
 *      storage method.
 *
 * @param original - The unmodified KeyEntry object that we want to make a change to.
 * @param updated - How the KeyEntry should look after being updated.
 *
 * @return true if the KeyEntry was updated both in memory, and in the KeyStorage object.
 *      false on error.
 */
bool KeyEntriesSingleton::updateKeyEntry(const KeyEntry &original, const KeyEntry &updated)
{
    if (!updateKeyEntryInMemory(original, updated)) {
        LOG_DEBUG("The key entry for the original identifier '" + original.identifier() + "' is not in memory.  Will attempt to update in the key storage.");
    }

    return mKeyStorage.updateKey(original, updated);
}

/**
 * @brief KeyEntriesSingleton::deleteKeyEntry - Delete a key entry from memory, and from the key
 *      storage object.
 *
 * @param toDelete - The KeyEntry that we want to delete from memory, and from the key
 *      storage object.
 *
 * @return true if the KeyEntry was deleted from memory, and from the key storage
 *      object.
 */
bool KeyEntriesSingleton::deleteKeyEntry(const QString &toDelete)
{
    if (toDelete.isEmpty()) {
        LOG_ERROR("Attempted to delete a KeyEntry with an empty identifier!");
        return false;
    }

    if (!deleteKeyEntryFromMemory(toDelete)) {
        LOG_DEBUG("The key entry for identifier '" + toDelete + "' is not in memory.  Will attempt to delete from key storage.");
    }

    // We should ALWAYS be able to delete from the KeyStorage object.
    if (!mKeyStorage.deleteKeyByIdentifier(toDelete)) {
        LOG_ERROR("Failed to remove the key entry with identifier '" + toDelete + "' from the key storage!");
        return false;
    }

    // Tell the UI to update itself.
    emit fullRefresh();

    // Everything is good!
    return true;
}

/**
 * @brief KeyEntriesSingleton::incrementHotpCounter - Increment the HOTP counter value, and then
 *      update the code that is being displayed.
 *
 * @param identifier - The identifier of the OTP that we want to increment the HOTP value
 *      for.
 *
 * @return true if the HOTP counter and value were incremented properly.  false on error.
 */
bool KeyEntriesSingleton::incrementHotpCounter(const QString &identifier)
{
    KeyEntry toIncrement;
    KeyEntry *original;

    if (identifier.isEmpty()) {
        LOG_ERROR("Cannot increment the HOTP counter for an empty identifier!");
        return false;
    }

    LOG_DEBUG("Incrementing HOTP counter for : " + identifier);

    // Find the KeyEntry to use.
    original = fromIdentifier(identifier);
    if (original == nullptr) {
        LOG_ERROR("Unable to locate the key entry for identifier : " + identifier);
        return false;
    }

    // Make sure the KeyEntry is an HOTP entry.
    if (original->otpType() != KEYENTRY_OTPTYPE_HOTP) {
        LOG_ERROR("Cannot increment the HOTP counter for non-HOTP key with the identifier : " + identifier);
        return false;
    }

    // Make a copy to edit.
    toIncrement = (*original);

    // It is an HOTP type, so increment the counter.
    toIncrement.setHotpCounter(toIncrement.hotpCounter()+1);

    // Then, update the database entry.
    if (updateKeyEntry((*original), toIncrement) == false) {
        LOG_ERROR("Failed to increment the HOTP counter!  The displayed value won't be updated.");
    }

    return true;
}

/**
 * @brief KeyEntriesSingleton::count - Return the number of key entries that are stored in this
 *      object.
 *
 * @return int containing the number of key entries stored in this object.
 */
int KeyEntriesSingleton::count()
{
    return mEntryList.size();
}

/**
 * @brief KeyEntriesSingleton::at - Return a specific KeyEntry object at the specified index.
 *
 * @param i - The index of the KeyEntry to retrieve.
 *
 * @return KeyEntry pointer for the requested object.  If the object doesn't exist, or
 *      if the index is out of range, nullptr will be returned.
 */
KeyEntry *KeyEntriesSingleton::at(int i)
{
    if (i >= mEntryList.count()) {
        return nullptr;
    }

    return mEntryList.at(i);
}

/**
 * @brief KeyEntriesSingleton::fromIdentifier - Attempt to locate the KeyEntry object for the
 *      specified identifier.  If it isn't loaded in memory, we will look in the database
 *      just in case it didn't get loaded for some reason.  If it wasn't loaded, we will
 *      add it to our list, and emit a fullRefresh() signal.
 *
 * @param identifier - The key identifier that we are looking for.
 *
 * @return KeyEntry pointer to the requested KeyEntry.  If the KeyEntry isn't found,
 *      a nullptr will be returned.
 */
KeyEntry *KeyEntriesSingleton::fromIdentifier(const QString &identifier)
{
    KeyEntry *result = nullptr;

    if (identifier.isEmpty()) {
        LOG_ERROR("Attempted to locate a KeyEntry object with an empty identifier!");
        return nullptr;
    }

    result = fromIdentifierInMemory(identifier);
    if (nullptr == result) {
        // Look in the key storage object.
        result = fromIdentifierInKeyStorage(identifier);
    }

    return result;
}

/**
 * @brief KeyEntriesSingleton::slotUpdateOtpValues - Called by a QTimer, we should update the
 *      values of the OTPs that are being shown, calculate the next time that we should
 *      update, and reset the timer.
 */
void KeyEntriesSingleton::slotUpdateOtpValues()
{
    LOG_DEBUG("Update timer fired!");

    if (!calculateEntries()) {
        LOG_ERROR("Unable to calculate new OTP values.  Will try again in 10 seconds!");

        // Set the timer to fire again in 10 seconds.
        mUpdateTimer.start(10000);
        return;
    }

    // Call updateTimer() to reset the timer for the next time we need to update.
    if (!updateTimer()) {
        LOG_ERROR("Unable to calculate the next time interval to use!  Will try again in 10 seconds!");

        // Set the timer to fire again in 10 seconds.
        mUpdateTimer.start(10000);
    }
}

/**
 * @brief KeyEntriesSingleton::indexFromIdentifierInMemory - Find the index for the KeyEntry that
 *      matches the provided identifier.
 *
 * @param identifier - The identifier to search in memory for.
 *
 * @return int containing the index to the KeyEntry that matches the provided identifier.
 *      If the KeyEntry couldn't be found, -1 will be returned.
 */
int KeyEntriesSingleton::indexFromIdentifierInMemory(const QString &identifier)
{
    for (int i = 0; i < mEntryList.size(); i++) {
        if (mEntryList.at(i)->identifier() == identifier) {
            // Found it.
            return i;
        }
    }

    // Didn't find it.
    return -1;
}

/**
 * @brief KeyEntriesSingleton::fromIdentifierInMemory - Search for the key identifier in memory.
 *
 * @param identifier - The identifier for the key we want to locate.
 *
 * @return KeyEntry pointer to the KeyEntry if it was found.  If it wasn't found, nullptr
 *      will be returned.
 */
KeyEntry *KeyEntriesSingleton::fromIdentifierInMemory(const QString &identifier)
{
    int index;

    index = indexFromIdentifierInMemory(identifier);
    if (index >= 0) {
        // Return that entry.
        return mEntryList.at(index);
    }

    // Otherwise, we didn't find it.
    return nullptr;
}

/**
 * @brief KeyEntriesSingleton::fromIdentifierInKeyStorage - See if the specified key identifier can
 *      be found in the KeyStorage object.  If it is, then we want to add it to our list,
 *      and emit the fullUpdate() signal to let the UI know that there is new data.
 *
 * @param identifier - The identifier to search the KeyStorage object for.
 *
 * @return KeyEntry pointer to the KeyEntry if it was found.  If it wasn't found, nullptr
 *      will be returned.
 */
KeyEntry *KeyEntriesSingleton::fromIdentifierInKeyStorage(const QString &identifier)
{
    KeyEntry result;

    if (!mKeyStorage.keyByIdentifier(identifier, result)) {
        // Didn't find it.
        return nullptr;
    }

    // Found it, add it to our in-memory list.
    mEntryList.push_back(new KeyEntry(result));

    // Then, return the newly created entry.
    return mEntryList.at(mEntryList.size() - 1);
}

/**
 * @brief KeyEntriesSingleton::deleteKeyEntryFromMemory - Look in the list of known key entries in
 *      memory, and delete the one that matches the value provided.
 *
 * @param toDelete - The identifier of the KeyEntry that we want to delete.
 *
 * @return true if the KeyEntry was removed from memory.  false if it wasn't found in memory.
 */
bool KeyEntriesSingleton::deleteKeyEntryFromMemory(const QString &toDelete)
{
    QList<KeyEntry *> newList;

    for (int i = 0; i < mEntryList.size(); i++) {
        if (mEntryList.at(i)->identifier() != toDelete) {
            // Copy it to the new list.
            newList.push_back(mEntryList.at(i));
        } else {
            // Delete the pointer.
            delete mEntryList.at(i);
        }
    }

    // Update the object member.
    mEntryList = newList;

    return true;
}

/**
 * @brief KeyEntriesSingleton::updateKeyEntryInMemory - Update the KeyEntry object that is in memory
 *      with the new values.
 *
 * @param original - The original object that we want to update.
 * @param updated - The way we want the object to look after being updated.
 *
 * @return true if the in memory object was updated.  false if it couldn't be found.
 */
bool KeyEntriesSingleton::updateKeyEntryInMemory(const KeyEntry &original, const KeyEntry &updated)
{
    int index;

    index = indexFromIdentifierInMemory(original.identifier());
    if ((index < 0) || (index >= mEntryList.size())) {
        LOG_ERROR("Unable to locate the existing object for the identifier '" + original.identifier() + "'.");
        return false;
    }

    // Copy the values from the updated object in to our in-memory object.
    mEntryList.at(index)->copyFromObject(updated);

    return true;
}

/**
 * @brief KeyEntriesSingleton::addKeyEntryInMemory - Add a key entry to the in-memory list.
 *
 * @param toAdd - The KeyEntry object to add to the in-memory list.
 *
 * @return true if the entry was added to the in-memory list.  false on error.
 */
bool KeyEntriesSingleton::addKeyEntryInMemory(const KeyEntry &toAdd)
{
    KeyEntry *temp;

    // Create a pointer to add to mEntryList.
    temp = new KeyEntry(toAdd);
    QQmlEngine::setObjectOwnership(temp, QQmlEngine::CppOwnership);

    // Add the entry to our list.
    mEntryList.push_back(temp);

    // Then, calculate the current code to show.
    OtpHandler::calculateOtpForKeyEntry(mEntryList.at(mEntryList.size() - 1));

    return true;
}

/**
 * @brief KeyEntriesSingleton::updateTimer - Figure out how long we need to wait before calculating the
 *      new value for an OTP, and set a timer to do the update at that time.
 *
 * @return true if the timer was calculated and started.  false on error.
 */
bool KeyEntriesSingleton::updateTimer()
{
    int nextUpdate;

    // Figure out the shortest period before one of the entries needs to be updated.
    nextUpdate = shortestUpdatePeriod();
    if (nextUpdate < 0) {
        LOG_ERROR("Got an invalid period of " + QString::number(nextUpdate) + " to the next code update!   Updating will stop!");
        return false;
    }

    // If the shortest time is 0 seconds, then we need to update again in a little bit.
    if (nextUpdate == 0) {
        // Set the timer for half a second, and try again.
        mUpdateTimer.start(500);
        return true;
    }

    // Then, set our timer to call our slot when the next update period expires.
    mUpdateTimer.start(nextUpdate * 1000);

    return true;
}

/**
 * @brief KeyEntriesSingleton::shortestUpdatePeriod - Iterate the list of known OTPs, and locate the
 *      shortest time until we need to update one.
 *
 * @return int containing the shortest time until we need to update at least one of the OTP
 *      values.  On error, -1 will be returned.
 */
int KeyEntriesSingleton::shortestUpdatePeriod()
{
    bool allHotp = true;
    int shortestTime = 0xffff;          // Shouldn't be possible for it to be that long.
    int interval;

    for (int i = 0; i < mEntryList.size(); i++) {
        if (mEntryList.at(i)->keyType() != KEYENTRY_OTPTYPE_HOTP) {
            // We have at least one non-HOTP key.
            allHotp = false;

            // Calculate the interval until this OTP needs to be updated again.
            interval = mEntryList.at(i)->timeStep() - mEntryList.at(i)->startTime();

            // If that value is less than the current shortestTime, then set our shortestTime
            // to that value.
            if (interval < shortestTime) {
                shortestTime = interval;
            }
        }
    }

    // If all of the entries were HOTP entries, then have us check again in 30 seconds, in case
    // the user adds a new, non-HOTP entry.
    if (allHotp) {
        return 30;
    }    

    LOG_DEBUG("Next update interval should be " + QString::number(shortestTime) + " second(s).");

    // Otherwise, return the shorttestTime that we found.
    return shortestTime;
}
