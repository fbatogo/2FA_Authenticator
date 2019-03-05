#include "otpentry.h"

OtpEntry::OtpEntry(QObject *parent) :
    QObject(parent)
{
    mIdentifier.clear();
    mCurrentCode.clear();
    mStartTime = -1;
    mTimeStep = -1;
    mInvalidReason.clear();
    mIssuer.clear();
    mOtpType = -1;
    mHotpCounter = -1;
}

OtpEntry::OtpEntry(const OtpEntry &toCopy) :
    QObject(nullptr)
{
    setIdentifier(toCopy.identifier());
    setCurrentCode(toCopy.currentCode());
    setStartTime(toCopy.startTime());
}

bool OtpEntry::valid() const
{
    return mValid;
}

void OtpEntry::setValid(bool newval)
{
    mValid = newval;
}

QString OtpEntry::identifier() const
{
    return mIdentifier;
}

void OtpEntry::setIdentifier(const QString &newvalue)
{
    mIdentifier = newvalue;
}

QString OtpEntry::currentCode() const
{
    return mCurrentCode;
}

void OtpEntry::setCurrentCode(const QString &newvalue)
{
    mCurrentCode = newvalue;
}

int OtpEntry::startTime() const
{
    return mStartTime;
}

void OtpEntry::setStartTime(int newvalue)
{
    mStartTime = newvalue;
}

int OtpEntry::timeStep() const
{
    return mTimeStep;
}

void OtpEntry::setTimeStep(int newvalue)
{
    mTimeStep = newvalue;
}

QString OtpEntry::invalidReason()
{
    return mInvalidReason;
}

void OtpEntry::setInvalidReason(const QString &newvalue)
{
    mInvalidReason = newvalue;
}

QString OtpEntry::issuer()
{
    return mIssuer;
}

void OtpEntry::setIssuer(const QString &newvalue)
{
    mIssuer = newvalue;
}

int OtpEntry::otpType() const
{
    return mOtpType;
}

void OtpEntry::setOtpType(int newvalue)
{
    mOtpType = newvalue;
}

int OtpEntry::hotpCounter() const
{
    return mHotpCounter;
}

void OtpEntry::setHotpCounter(int newvalue)
{
    mHotpCounter = newvalue;
}
