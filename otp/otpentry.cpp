#include "otpentry.h"

OtpEntry::OtpEntry(QObject *parent) :
    QObject(parent)
{
    mIdentifier.clear();
    mCurrentCode.clear();
    mStartTime = -1;
    mTimeStep = -1;
}

OtpEntry::OtpEntry(const OtpEntry &toCopy) :
    QObject(nullptr)
{
    setIdentifier(toCopy.identifier());
    setCurrentCode(toCopy.currentCode());
    setStartTime(toCopy.startTime());
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
