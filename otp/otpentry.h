#ifndef OTPENTRY_H
#define OTPENTRY_H

#include <QObject>
#include <QString>

class OtpEntry : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString mIdentifier READ identifier)
    Q_PROPERTY(QString mCurrentCode READ currentCode)
    Q_PROPERTY(int mTimeRemaining READ timeRemaining)

public:
    OtpEntry(QObject *parent = 0);
    OtpEntry(const OtpEntry &toCopy);

    QString identifier() const;
    void setIdentifier(const QString &newvalue);

    QString currentCode() const;
    void setCurrentCode(const QString &newvalue);

    int timeRemaining() const;
    void setTimeRemaining(int newvalue);

    int timeStep() const;
    void setTimeStep(int newvalue);

private:
    QString mIdentifier;
    QString mCurrentCode;
    int mTimeRemaining;
    int mTimeStep;
};

#endif // OTPENTRY_H
