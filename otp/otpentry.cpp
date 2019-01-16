#include "otpentry.h"

OtpEntry::OtpEntry(QObject *parent) :
    QObject(parent)
{
    mIdentifier.clear();
    mCurrentCode.clear();
    mTimeRemaining = -1;
}

OtpEntry::OtpEntry(const OtpEntry &toCopy)
{
    setIdentifier(toCopy.identifier());
    setCurrentCode(toCopy.currentCode());
    setTimeRemaining(toCopy.timeRemaining());
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

int OtpEntry::timeRemaining() const
{
    return mTimeRemaining;
}

void OtpEntry::setTimeRemaining(int newvalue)
{
    mTimeRemaining = newvalue;
}

int OtpEntry::timeStep() const
{
    return mTimeStep;
}

void OtpEntry::setTimeStep(int newvalue)
{
    mTimeStep = newvalue;
}
