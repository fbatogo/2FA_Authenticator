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

private:
    QString mIdentifier;
    QString mCurrentCode;
    int mTimeRemaining;
};

#endif // OTPENTRY_H
