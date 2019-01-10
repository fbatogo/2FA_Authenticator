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
