#include "keyentry.h"

#include <logger.h>
#include <sstream>

KeyEntry::KeyEntry() :
    QObject(nullptr)
{
    clear();
}

KeyEntry::KeyEntry(const KeyEntry &toCopy) :
    QObject(nullptr)
{
    copyFromObject(toCopy);
}

KeyEntry::~KeyEntry()
{

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
    mHotpCounter = -1;           // HOTP isn't used by default.
    mIssuer.clear();
    mInvalidReason.clear();    
    mCurrentCode.clear();
    mPrintableCurrentCode.clear();
    mStartTime = -1;
    mCodeValid = false;
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
    if (mIdentifier.isEmpty() || mSecret.empty()) {
        LOG_DEBUG("Either the identifier or secret is empty.");
        return false;
    }

    // The key type needs to be 0 or 1.
    if (mKeyType > 1) {
        LOG_DEBUG("Invalid key type! (" + QString::number(mKeyType) + ")");
        return false;
    }

    // The OTP type needs to be 0 or 1.
    if (mOtpType > 1) {
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
    emit identifierChanged();
}

const ByteArray &KeyEntry::secret() const
{
    return mSecret;
}

void KeyEntry::setSecret(const ByteArray &newvalue)
{
    mSecret = newvalue;
    emit secretChanged();
}

size_t KeyEntry::keyType() const
{
    return mKeyType;
}

void KeyEntry::keyType(size_t &value)
{
    value = mKeyType;
}

void KeyEntry::setKeyType(size_t newvalue)
{
    mKeyType = newvalue;
    emit keyTypeChanged();
}

size_t KeyEntry::otpType() const
{
    return mOtpType;
}

void KeyEntry::otpType(size_t &value)
{
    value = mOtpType;
}

void KeyEntry::setOtpType(size_t newvalue)
{
    mOtpType = newvalue;
    emit otpTypeChanged();
}

size_t KeyEntry::outNumberCount() const
{
    return mOutNumberCount;
}

void KeyEntry::outNumberCount(size_t &value)
{
    value = mOutNumberCount;
}

void KeyEntry::setOutNumberCount(size_t newvalue)
{
    mOutNumberCount = newvalue;
    emit outNumberCountChanged();
}

size_t KeyEntry::timeStep() const
{
    return mTimeStep;
}

void KeyEntry::timeStep(size_t &value)
{
    value = mTimeStep;
}

void KeyEntry::setTimeStep(size_t newvalue)
{
    mTimeStep = newvalue;
    emit timeStepChanged();
}

size_t KeyEntry::timeOffset() const
{
    return mTimeOffset;
}

void KeyEntry::timeOffset(size_t &value)
{
    value = mTimeOffset;
}

void KeyEntry::setTimeOffset(size_t newvalue)
{
    mTimeOffset = newvalue;
    emit timeOffsetChanged();
}

size_t KeyEntry::algorithm() const
{
    return mAlgorithm;
}

void KeyEntry::setAlgorithm(size_t newvalue)
{
    mAlgorithm = newvalue;
    emit algorithmChanged();
}

size_t KeyEntry::hotpCounter() const
{
    return mHotpCounter;
}

void KeyEntry::setHotpCounter(size_t newvalue)
{
    mHotpCounter = newvalue;
    emit hotpCounterChanged();
}

QString KeyEntry::issuer() const
{
    return mIssuer;
}

void KeyEntry::setIssuer(const QString &newvalue)
{
    mIssuer = newvalue;
    emit issuerChanged();
}

QString KeyEntry::invalidReason() const
{
    return mInvalidReason;
}

void KeyEntry::setInvalidReason(const QString &newvalue)
{
    mInvalidReason = newvalue;
    emit invalidReasonChanged();
}

QString KeyEntry::currentCode() const
{
    return mCurrentCode;
}

void KeyEntry::setCurrentCode(const QString &newvalue)
{
    mCurrentCode = newvalue;
    emit currentCodeChanged();

    // Update the printable version of the code as well.
    setPrintableCurrentCode(mCurrentCode);
}

/**
 * @brief KeyEntry::printableCurrentCode - Return the current value of the
 *      printable current code member variable.
 *
 * @return QString containing the printable current code.  If no code is set,
 *      the string will be empty.
 */
QString KeyEntry::printableCurrentCode() const
{
    return mPrintableCurrentCode;
}

/**
 * @brief KeyEntry::setPrintableCurrentCode - Given the current code, without any special
 *      formatting, format it according to how we are configured.
 *
 * @param newvalue - The *UNFORMATTED* current code string.
 */
void KeyEntry::setPrintableCurrentCode(const QString &newvalue)
{
    mPrintableCurrentCode = newvalue;

    // Figure out how we want to break up the code.
    switch (newvalue.length()) {
    case 6:
        // Add a space between the 1st and 2nd 3 characters.
        mPrintableCurrentCode.insert(3, " ");
        break;

    case 7:
        // Add a space between the 1st 4 and the 2nd 3 characters.
        mPrintableCurrentCode.insert(4, " ");
        break;

    case 8:
        // Add a space between the 1st and 2nd 4 characters.
        mPrintableCurrentCode.insert(4, " ");
        break;

    default:
        // This shouldn't happen, but if it does, just show the code with no spaces.
        break;
    }

    emit printableCurrentCodeChanged();
}

size_t KeyEntry::startTime() const
{
    return mStartTime;
}

void KeyEntry::setStartTime(size_t newvalue)
{
    mStartTime = newvalue;
    emit startTimeChanged();
}

bool KeyEntry::codeValid() const
{
    return mCodeValid;
}

void KeyEntry::setCodeValid(bool newvalue)
{
    mCodeValid = newvalue;
    emit codeValidChanged();
}

std::string KeyEntry::toString()
{
    std::stringstream result;

    result << "[KeyEntry -- valid: ";
    result << mValid << "  code valid: " << mCodeValid << "  identifier: " << mIdentifier.toStdString() << "  secret: " << mSecret.toString() << "  key type: ";
    result << mKeyType << "  otp type: " << mOtpType << "  digits: " << mOutNumberCount << "  time step: " << mTimeStep << "  time offset: " << mTimeOffset;
    result << "  algorithm: " << mAlgorithm << "  hotp counter: " << mHotpCounter << "  issuer: " << mIssuer.toStdString() << "  invalid reason: " << mInvalidReason.toStdString();
    result << "  current code: " << mCurrentCode.toStdString() << "  start time: " << mStartTime << "]";

    return result.str();
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
    if (this != &toCopy) {
        copyFromObject(toCopy);
    }

    return *this;
}

/**
 * @brief KeyEntry::boolToString - Convert a boolean value to a string representation.
 *
 * @param value - The boolean to convert to a string.
 *
 * @return std::string containing "true" or "false".
 */
std::string KeyEntry::boolToString(bool value)
{
    if (value) {
        return "true";
    }

    return "false";
}

/**
 * @brief KeyEntry::copyFromObject - Copy all of the values from the provided object in
 *      to the current one.
 *
 * @param toCopy - The object that we want to copy the values from.
 */
void KeyEntry::copyFromObject(const KeyEntry &toCopy)
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
    setInvalidReason(toCopy.invalidReason());
    setCurrentCode(toCopy.currentCode());
    setPrintableCurrentCode(toCopy.printableCurrentCode());
    setStartTime(toCopy.startTime());
    setCodeValid(toCopy.codeValid());
}
