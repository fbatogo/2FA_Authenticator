#include "keyentry.h"

#include <logger.h>

KeyEntry::KeyEntry() :
    QObject(nullptr)
{
    clear();
}

KeyEntry::KeyEntry(const KeyEntry &toCopy) :
    QObject(nullptr)
{
    setIdentifier(toCopy.identifier());
    setSecret(toCopy.secret());
    setKeyType(toCopy.keyType());
    setOtpType(toCopy.otpType());
    setOutNumberCount(toCopy.outNumberCount());
    setTimeStep(toCopy.timeStep());
    setTimeOffset(toCopy.timeOffset());
}

void KeyEntry::clear()
{
    // Set default values.
    mIdentifier.clear();
    mSecret.clear();
    mKeyType = -1;
    mOtpType = -1;
    mOutNumberCount = -1;
    mTimeStep = 30;             // Recommended default.
    mTimeOffset = 0;            // Recommended default.
    mAlgorithm = KEYENTRY_ALG_SHA1;        // Recommended default.
    mHotpCounter = 0;           // HOTP isn't used by default.
    mIssuer.clear();
    mInvalidReason.clear();
}

/**
 * @brief KeyEntry::valid - Check to see that all of the values in this object appear
 *      to be legal.
 *
 * @return false if the values aren't legal.  true otherwise.
 */
bool KeyEntry::valid() const
{
    // If the invalid reason isn't an empty string, then this key entry object isn't valid.
    if (!mInvalidReason.isEmpty()) {
        LOG_DEBUG("Key entry is invalid because an 'invalid reason' was set.");
        return false;
    }

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

void KeyEntry::setSecret(const QString &newvalue)
{
    mSecret = newvalue;
}

int KeyEntry::keyType() const
{
    return mKeyType;
}

void KeyEntry::keyType(int &value)
{
    value = mKeyType;
}

void KeyEntry::setKeyType(int newvalue)
{
    mKeyType = newvalue;
}

int KeyEntry::otpType() const
{
    return mOtpType;
}

void KeyEntry::otpType(int &value)
{
    value = mOtpType;
}

void KeyEntry::setOtpType(int newvalue)
{
    mOtpType = newvalue;
}

int KeyEntry::outNumberCount() const
{
    return mOutNumberCount;
}

void KeyEntry::outNumberCount(int &value)
{
    value = mOutNumberCount;
}

void KeyEntry::setOutNumberCount(int newvalue)
{
    mOutNumberCount = newvalue;
}

int KeyEntry::timeStep() const
{
    return mTimeStep;
}

void KeyEntry::timeStep(int &value)
{
    value = mTimeStep;
}

void KeyEntry::setTimeStep(int newvalue)
{
    mTimeStep = newvalue;
}

int KeyEntry::timeOffset() const
{
    return mTimeOffset;
}

void KeyEntry::timeOffset(int &value)
{
    value = mTimeOffset;
}

void KeyEntry::setTimeOffset(int newvalue)
{
    mTimeOffset = newvalue;
}

int KeyEntry::algorithm() const
{
    return mAlgorithm;
}

void KeyEntry::setAlgorithm(int newvalue)
{
    mAlgorithm = newvalue;
}

int KeyEntry::hotpCounter() const
{
    return mHotpCounter;
}

void KeyEntry::setHotpCounter(int newvalue)
{
    mHotpCounter = newvalue;
}

QString KeyEntry::issuer() const
{
    return mIssuer;
}

void KeyEntry::setIssuer(const QString &newvalue)
{
    mIssuer = newvalue;
}

QString KeyEntry::invalidReason() const
{
    return mInvalidReason;
}

void KeyEntry::setInvalidReason(const QString &newvalue)
{
    mInvalidReason = newvalue;
}

/**
 * @brief KeyEntry::operator = - Handle copying data when using the = operator.
 *
 * @param toCopy - The object to copy from.
 *
 * @return KeyEntry & containing the data from the 'toCopy' object.
 */
KeyEntry &KeyEntry::operator=(const KeyEntry &toCopy)
{
    setIdentifier(toCopy.identifier());
    setSecret(toCopy.secret());
    setKeyType(toCopy.keyType());
    setOtpType(toCopy.otpType());
    setOutNumberCount(toCopy.outNumberCount());
    setTimeStep(toCopy.timeStep());
    setTimeOffset(toCopy.timeOffset());
    setAlgorithm(toCopy.algorithm());
    setHotpCounter(toCopy.hotpCounter());
    setIssuer(toCopy.issuer());

    return *this;
}
