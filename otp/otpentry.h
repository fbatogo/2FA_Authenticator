#ifndef OTPENTRY_H
#define OTPENTRY_H

#include <QObject>
#include <QString>

class OtpEntry : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString mIdentifier READ identifier)
    Q_PROPERTY(QString mCurrentCode READ currentCode)
    Q_PROPERTY(int mStartTime READ startTime)
    Q_PROPERTY(int mTimeStep READ timeStep)
    Q_PROPERTY(bool mValid READ valid)
    Q_PROPERTY(QString mInvalidReason READ invalidReason)
    Q_PROPERTY(QString mIssuer READ issuer)
    Q_PROPERTY(int mOtpType READ otpType)
    Q_PROPERTY(int mHotpCounter READ hotpCounter)

public:
    OtpEntry(QObject *parent = nullptr);
    OtpEntry(const OtpEntry &toCopy);

    bool valid() const;
    void setValid(bool newval);

    QString identifier() const;
    void setIdentifier(const QString &newvalue);

    QString currentCode() const;
    void setCurrentCode(const QString &newvalue);

    int startTime() const;
    void setStartTime(int newvalue);

    int timeStep() const;
    void setTimeStep(int newvalue);

    QString invalidReason();
    void setInvalidReason(const QString &newvalue);

    QString issuer();
    void setIssuer(const QString &newvalue);

    int otpType() const;
    void setOtpType(int newvalue);

    int hotpCounter() const;
    void setHotpCounter(int newvalue);

private:
    QString mIdentifier;
    QString mCurrentCode;
    int mStartTime;
    int mTimeStep;
    bool mValid;
    QString mInvalidReason;
    QString mIssuer;
    int mOtpType;
    int mHotpCounter;
};

#endif // OTPENTRY_H
