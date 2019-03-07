#include "uikeyentries.h"

#include "logger.h"
#include "otp/otphandler.h"

UiKeyEntries::UiKeyEntries(QObject *parent) :
    QObject(parent)
{

}

UiKeyEntries::~UiKeyEntries()
{
    // Clean up the memory used in our internal list.
    for (int i = 0; i < mEntryList.size(); i++) {
        if (mEntryList.at(i) != nullptr) {
            delete mEntryList.at(i);
        }
    }
}

bool UiKeyEntries::populateEntries(QList<KeyEntry> &toCopy)
{
    // We need to convert all of the KeyEntries in to dynamic allocations.
    for (int i = 0; i < toCopy.size(); i++) {
        mEntryList.push_back(new KeyEntry(toCopy.at(i)));
    }

    return true;
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

int UiKeyEntries::count()
{
    return mEntryList.size();
}

KeyEntry *UiKeyEntries::at(int i)
{
    if (i >= mEntryList.count()) {
        return nullptr;
    }

    return mEntryList.at(i);
}
