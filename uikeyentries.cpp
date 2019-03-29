#include "uikeyentries.h"

#include <QTimer>
#include "logger.h"
#include "otp/otphandler.h"

UiKeyEntries::UiKeyEntries(QObject *parent) :
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
}

UiKeyEntries::~UiKeyEntries()
{
    // Disconnect the QTimer slots and signals.
    disconnect(&mUpdateTimer, SIGNAL(timeout()), this, SLOT(slotUpdateOtpValues()));

    clear();

    if (!mKeyStorage.freeStorage()) {
        LOG_ERROR("Unable to free the key storage!");
    }
}

/**
 * @brief UiKeyEntries::clear - Erase all of the KeyEntries in this object, and free
 *      the associated memory.
 */
void UiKeyEntries::clear()
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
 * @brief UiKeyEntries::populateEntries - Read all of the known KeyEntry objects from
 *      key storage, and store them in memory in this object.
 *
 * @return true if the entries were populated as expected.  false on error.
 */
bool UiKeyEntries::populateEntries()
{
    QList<KeyEntry> allKeys;
    bool result;

    allKeys.clear();

    if (!mKeyStorage.getAllKeys(allKeys)) {
        LOG_ERROR("Unable to get all of the keys stored in key storage!");
        return false;
    }

    // We need to convert all of the KeyEntries in to dynamic allocations.
    for (int i = 0; i < allKeys.size(); i++) {
        mEntryList.push_back(new KeyEntry(allKeys.at(i)));
    }

    // Calculate the codes to show.
    result = calculateEntries();
    if (result) {
        // Start the update timer.
        if (!updateTimer()) {
            LOG_ERROR("Failed to start the OTP update timer!");
            return false;
        }
    }

    return result;
}

/**
 * @brief UiKeyEntries::calculateEntries - Iterate the list of KeyEntries, and calculate
 *      the OTP value for each one.
 *
 * @return true if the entries were calculated.  false on a horrible, unrecoverable error.
 */
bool UiKeyEntries::calculateEntries()
{
    for (int i = 0; i < mEntryList.size(); i++) {
        OtpHandler::calculateOtpForKeyEntry(mEntryList.at(i));
    }

    return true;
}

/**
 * @brief UiKeyEntries::addKeyEntry - Add a key entry to the KeyStorage object, and then
 *      add it to the in-memory list, followed by emitting the fullUpdate() signal to let
 *      the UI know it needs to updated everything.
 *
 * @param toAdd - The KeyEntry object that we want to add to both the KeyStorage object, and
 *      the in memory list.
 *
 * @return true if the KeyEntry object was added to both the KeyStorage object and the
 *      in-memory list.  false on error.
 */
bool UiKeyEntries::addKeyEntry(const KeyEntry &toAdd)
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
 * @brief UiKeyEntries::updateKeyEntry - Update a key entry both in memory, and in the key
 *      storage method.
 *
 * @param original - The unmodified KeyEntry object that we want to make a change to.
 * @param updated - How the KeyEntry should look after being updated.
 *
 * @return true if the KeyEntry was updated both in memory, and in the KeyStorage object.
 *      false on error.
 */
bool UiKeyEntries::updateKeyEntry(const KeyEntry &original, const KeyEntry &updated)
{
    if (!updateKeyEntryInMemory(original, updated)) {
        LOG_DEBUG("The key entry for the original identifier '" + original.identifier() + "' is not in memory.  Will attempt to update in the key storage.");
    }

    return mKeyStorage.updateKey(original, updated);
}

/**
 * @brief UiKeyEntries::deleteKeyEntry - Delete a key entry from memory, and from the key
 *      storage object.
 *
 * @param toDelete - The KeyEntry that we want to delete from memory, and from the key
 *      storage object.
 *
 * @return true if the KeyEntry was deleted from memory, and from the key storage
 *      object.
 */
bool UiKeyEntries::deleteKeyEntry(const QString &toDelete)
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
 * @brief UiKeyEntries::count - Return the number of key entries that are stored in this
 *      object.
 *
 * @return int containing the number of key entries stored in this object.
 */
int UiKeyEntries::count()
{
    return mEntryList.size();
}

/**
 * @brief UiKeyEntries::at - Return a specific KeyEntry object at the specified index.
 *
 * @param i - The index of the KeyEntry to retrieve.
 *
 * @return KeyEntry pointer for the requested object.  If the object doesn't exist, or
 *      if the index is out of range, nullptr will be returned.
 */
KeyEntry *UiKeyEntries::at(int i)
{
    if (i >= mEntryList.count()) {
        return nullptr;
    }

    return mEntryList.at(i);
}

/**
 * @brief UiKeyEntries::fromIdentifier - Attempt to locate the KeyEntry object for the
 *      specified identifier.  If it isn't loaded in memory, we will look in the database
 *      just in case it didn't get loaded for some reason.  If it wasn't loaded, we will
 *      add it to our list, and emit a fullRefresh() signal.
 *
 * @param identifier - The key identifier that we are looking for.
 *
 * @return KeyEntry pointer to the requested KeyEntry.  If the KeyEntry isn't found,
 *      a nullptr will be returned.
 */
KeyEntry *UiKeyEntries::fromIdentifier(const QString &identifier)
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
 * @brief UiKeyEntries::slotUpdateOtpValues - Called by a QTimer, we should update the
 *      values of the OTPs that are being shown, calculate the next time that we should
 *      update, and reset the timer.
 */
void UiKeyEntries::slotUpdateOtpValues()
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
 * @brief UiKeyEntries::indexFromIdentifierInMemory - Find the index for the KeyEntry that
 *      matches the provided identifier.
 *
 * @param identifier - The identifier to search in memory for.
 *
 * @return int containing the index to the KeyEntry that matches the provided identifier.
 *      If the KeyEntry couldn't be found, -1 will be returned.
 */
int UiKeyEntries::indexFromIdentifierInMemory(const QString &identifier)
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
 * @brief UiKeyEntries::fromIdentifierInMemory - Search for the key identifier in memory.
 *
 * @param identifier - The identifier for the key we want to locate.
 *
 * @return KeyEntry pointer to the KeyEntry if it was found.  If it wasn't found, nullptr
 *      will be returned.
 */
KeyEntry *UiKeyEntries::fromIdentifierInMemory(const QString &identifier)
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
 * @brief UiKeyEntries::fromIdentifierInKeyStorage - See if the specified key identifier can
 *      be found in the KeyStorage object.  If it is, then we want to add it to our list,
 *      and emit the fullUpdate() signal to let the UI know that there is new data.
 *
 * @param identifier - The identifier to search the KeyStorage object for.
 *
 * @return KeyEntry pointer to the KeyEntry if it was found.  If it wasn't found, nullptr
 *      will be returned.
 */
KeyEntry *UiKeyEntries::fromIdentifierInKeyStorage(const QString &identifier)
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
 * @brief UiKeyEntries::deleteKeyEntryFromMemory - Look in the list of known key entries in
 *      memory, and delete the one that matches the value provided.
 *
 * @param toDelete - The identifier of the KeyEntry that we want to delete.
 *
 * @return true if the KeyEntry was removed from memory.  false if it wasn't found in memory.
 */
bool UiKeyEntries::deleteKeyEntryFromMemory(const QString &toDelete)
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
 * @brief UiKeyEntries::updateKeyEntryInMemory - Update the KeyEntry object that is in memory
 *      with the new values.
 *
 * @param original - The original object that we want to update.
 * @param updated - The way we want the object to look after being updated.
 *
 * @return true if the in memory object was updated.  false if it couldn't be found.
 */
bool UiKeyEntries::updateKeyEntryInMemory(const KeyEntry &original, const KeyEntry &updated)
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
 * @brief UiKeyEntries::addKeyEntryInMemory - Add a key entry to the in-memory list.
 *
 * @param toAdd - The KeyEntry object to add to the in-memory list.
 *
 * @return true if the entry was added to the in-memory list.  false on error.
 */
bool UiKeyEntries::addKeyEntryInMemory(const KeyEntry &toAdd)
{
    // Add the entry to our list.
    mEntryList.push_back(new KeyEntry(toAdd));

    // Then, calculate the current code to show.
    OtpHandler::calculateOtpForKeyEntry(mEntryList.at(mEntryList.size() - 1));

    return true;
}

/**
 * @brief UiKeyEntries::updateTimer - Figure out how long we need to wait before calculating the
 *      new value for an OTP, and set a timer to do the update at that time.
 *
 * @return true if the timer was calculated and started.  false on error.
 */
bool UiKeyEntries::updateTimer()
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
        // slotUpdateOtpValues will call back in to this function.
//        slotUpdateOtpValues();
        return true;
    }

    // Then, set our timer to call our slot when the next update period expires.
    mUpdateTimer.start((nextUpdate * 1000));

    return true;
}

/**
 * @brief UiKeyEntries::shortestUpdatePeriod - Iterate the list of known OTPs, and locate the
 *      shortest time until we need to update one.
 *
 * @return int containing the shortest time until we need to update at least one of the OTP
 *      values.  On error, -1 will be returned.
 */
int UiKeyEntries::shortestUpdatePeriod()
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
