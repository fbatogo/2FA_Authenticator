#ifndef KEYENTRY_H
#define KEYENTRY_H

#include <QObject>
#include <QString>

#define KEYENTRY_KEYTYPE_HEX      0
#define KEYENTRY_KEYTYPE_BASE32   1

#define KEYENTRY_OTPTYPE_TOTP     0
#define KEYENTRY_OTPTYPE_HOTP     1


// This class needs to be derived from QObject so that we can easily use it in the QML code.
class KeyEntry : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool mValid READ valid)
    Q_PROPERTY(QString mIdentifier READ identifier WRITE setIdentifier)
    Q_PROPERTY(QString mSecret READ secret WRITE setSecret)
    Q_PROPERTY(int mKeyType READ keyType WRITE setKeyType)
    Q_PROPERTY(int mOtpType READ otpType WRITE otpType)
    Q_PROPERTY(int mOutNumberCount READ outNumberCount WRITE setOutNumberCount)
    Q_PROPERTY(int mTimeStep READ timeStep WRITE setTimeStep)
    Q_PROPERTY(int mTimeOffset READ timeOffset WRITE setTimeOffset)


public:
    KeyEntry();
    KeyEntry(const KeyEntry &toCopy);

    void clear();

    bool valid() const;

    QString identifier() const;
    void setIdentifier(const QString &newvalue);

    QString secret() const;
    void setSecret(const QString &newvalue);

    int keyType() const;
    void keyType(int &value);
    void setKeyType(int newvalue);

    int otpType() const;
    void otpType(int &value);
    void setOtpType(int newvalue);

    int outNumberCount() const;
    void outNumberCount(int &value);
    void setOutNumberCount(int newvalue);

    int timeStep() const;
    void timeStep(int &value);
    void setTimeStep(int newvalue);

    int timeOffset() const;
    void timeOffset(int &value);
    void setTimeOffset(int newvalue);

    KeyEntry& operator=(const KeyEntry& toCopy);

private:
    bool mValid;
    QString mIdentifier;
    QString mSecret;
    int mKeyType;
    int mOtpType;
    int mOutNumberCount;
    int mTimeStep;
    int mTimeOffset;
};

#endif // KEYENTRY_H
