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

private:
    QString mIdentifier;
    QString mCurrentCode;
    int mStartTime;
    int mTimeStep;
    bool mValid;
    QString mInvalidReason;
};

#endif // OTPENTRY_H
