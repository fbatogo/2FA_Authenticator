#include "secretentry.h"

SecretEntry::SecretEntry()
{
    // Set default values.
    mIdentifier.clear();
    mSecret.clear();
    mKeyType = -1;
    mOtpType = -1;
    mOutNumberCount = -1;
}

/**
 * @brief SecretEntry::valid - Check to see that all of the values in this object appear
 *      to be legal.
 *
 * @return false if the values aren't legal.  true otherwise.
 */
bool SecretEntry::valid() const
{
    // The identifier and secret can't be empty.
    if (mIdentifier.isEmpty() || mSecret.isEmpty()) {
        qDebug("Either the identifier or secret is empty.");
        return false;
    }

    // The key type needs to be 0 or 1.
    if ((mKeyType < 0) || (mKeyType > 1)) {
        qDebug("Invalid key type! (%d)", mKeyType);
        return false;
    }

    // The OTP type needs to be 0 or 1.
    if ((mOtpType < 0) || (mOtpType > 1)) {
        qDebug("Invalid OTP type! (%d)", mOtpType);
        return false;
    }

    // The out number count needs to be between 6 and 8.
    if ((mOutNumberCount < 6) || (mOutNumberCount > 8)) {
        qDebug("Out number count is invalid! (%d)", mOutNumberCount);
        return false;
    }

    // Everything appears to be valid!
    return true;
}

QString SecretEntry::identifier() const
{
    return mIdentifier;
}

void SecretEntry::setIdentifier(const QString &newvalue)
{
    mIdentifier = newvalue;
}

QString SecretEntry::secret() const
{
    return mSecret;
}

void SecretEntry::setSecert(const QString &newvalue)
{
    mSecret = newvalue;
}

int SecretEntry::keyType() const
{
    return mKeyType;
}

void SecretEntry::setKeyType(int newvalue)
{
    mKeyType = newvalue;
}

int SecretEntry::otpType() const
{
    return mOtpType;
}

void SecretEntry::setOtpType(int newvalue)
{
    mOtpType = newvalue;
}

int SecretEntry::outNumberCount() const
{
    return mOutNumberCount;
}

void SecretEntry::setOutNumberCount(int newvalue)
{
    mOutNumberCount = newvalue;
}
