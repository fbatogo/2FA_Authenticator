#include "keyentry.h"

#include <logger.h>

KeyEntry::KeyEntry()
{
    clear();
}

void KeyEntry::clear()
{
    // Set default values.
    mIdentifier.clear();
    mSecret.clear();
    mKeyType = -1;
    mOtpType = -1;
    mOutNumberCount = -1;
}

/**
 * @brief KeyEntry::valid - Check to see that all of the values in this object appear
 *      to be legal.
 *
 * @return false if the values aren't legal.  true otherwise.
 */
bool KeyEntry::valid() const
{
    // The identifier and secret can't be empty.
    if (mIdentifier.isEmpty() || mSecret.isEmpty()) {
        LOG_DEBUG("Either the identifier or secret is empty.");
        return false;
    }

    // The key type needs to be 0 or 1.
    if ((mKeyType < 0) || (mKeyType > 1)) {
        LOG_DEBUG("Invalid key type! (" + QString::number(mKeyType) + ")");
        return false;
    }

    // The OTP type needs to be 0 or 1.
    if ((mOtpType < 0) || (mOtpType > 1)) {
        LOG_DEBUG("Invalid OTP type! (" + QString::number(mOtpType) + ")");
        return false;
    }

    // The out number count needs to be between 6 and 8.
    if ((mOutNumberCount < 6) || (mOutNumberCount > 8)) {
        LOG_DEBUG("Out number count is invalid! (" + QString::number(mOutNumberCount) + ")");
        return false;
    }

    // Everything appears to be valid!
    return true;
}

QString KeyEntry::identifier() const
{
    return mIdentifier;
}

void KeyEntry::setIdentifier(const QString &newvalue)
{
    mIdentifier = newvalue;
}

QString KeyEntry::secret() const
{
    return mSecret;
}

void KeyEntry::setSecert(const QString &newvalue)
{
    mSecret = newvalue;
}

int KeyEntry::keyType() const
{
    return mKeyType;
}

void KeyEntry::setKeyType(int newvalue)
{
    mKeyType = newvalue;
}

int KeyEntry::otpType() const
{
    return mOtpType;
}

void KeyEntry::setOtpType(int newvalue)
{
    mOtpType = newvalue;
}

int KeyEntry::outNumberCount() const
{
    return mOutNumberCount;
}

void KeyEntry::setOutNumberCount(int newvalue)
{
    mOutNumberCount = newvalue;
}
