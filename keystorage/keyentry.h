#ifndef KEYENTRY_H
#define KEYENTRY_H

#include <QObject>
#include <QString>

#define KEYENTRY_KEYTYPE_HEX      0
#define KEYENTRY_KEYTYPE_BASE32   1
#define KEYENTRY_KEYTYPE_MAX      1     // The highest key type value we can encode.

#define KEYENTRY_OTPTYPE_TOTP     0
#define KEYENTRY_OTPTYPE_HOTP     1
#define KEYENTRY_OTPTYPE_MAX      1     // The highest otp type value we can encode.

#define KEYENTRY_ALG_SHA1         0
#define KEYENTRY_ALG_SHA256       1
#define KEYENTRY_ALG_SHA512       2
#define KEYENTRY_ALG_MAX          3     // The highest algorithm value we can encode.

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
    Q_PROPERTY(int mAlgorithm READ algorithm WRITE setAlgorithm)
    Q_PROPERTY(int mHotpCounter READ hotpCounter WRITE setHotpCounter)
    Q_PROPERTY(QString mIssuer READ issuer WRITE setIssuer)

    Q_PROPERTY(QString mInvalidReason READ invalidReason)


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

    int algorithm() const;
    void setAlgorithm(int newvalue);

    int hotpCounter() const;
    void setHotpCounter(int newvalue);

    QString issuer() const;
    void setIssuer(const QString &newvalue);

    QString invalidReason() const;
    void setInvalidReason(const QString &newvalue);

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
    int mAlgorithm;
    int mHotpCounter;
    QString mIssuer;
    QString mInvalidReason;
};

#endif // KEYENTRY_H
