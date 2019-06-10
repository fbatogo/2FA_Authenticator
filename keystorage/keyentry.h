#ifndef KEYENTRY_H
#define KEYENTRY_H

#include <QObject>
#include <QString>
#include "container/bytearray.h"

const unsigned int KEYENTRY_KEYTYPE_HEX=0;
const unsigned int KEYENTRY_KEYTYPE_BASE32=1;
const unsigned int KEYENTRY_KEYTYPE_MAX=1;     // The highest key type value we can encode.

const unsigned int KEYENTRY_OTPTYPE_TOTP=0;
const unsigned int KEYENTRY_OTPTYPE_HOTP=1;
const unsigned int KEYENTRY_OTPTYPE_MAX=1;     // The highest otp type value we can encode.

const unsigned int KEYENTRY_ALG_SHA1=0;
const unsigned int KEYENTRY_ALG_SHA256=1;
const unsigned int KEYENTRY_ALG_SHA512=2;
const unsigned int KEYENTRY_ALG_MAX=3;     // The highest algorithm value we can encode.

// This class needs to be derived from QObject so that we can easily use it in the QML code.
class KeyEntry : public QObject
{
    Q_OBJECT

    // Values used to store data about the key.
    Q_PROPERTY(bool mValid READ valid)
    Q_PROPERTY(QString mIdentifier READ identifier WRITE setIdentifier NOTIFY identifierChanged)
    Q_PROPERTY(ByteArray mSecret READ secret WRITE setSecret NOTIFY secretChanged)
    Q_PROPERTY(ByteArray mDecodedSecret READ decodedSecret WRITE setDecodedSecret NOTIFY decodedSecretChanged)
    Q_PROPERTY(unsigned int mKeyType READ keyType WRITE setKeyType NOTIFY keyTypeChanged)
    Q_PROPERTY(unsigned int mOtpType READ otpType WRITE otpType NOTIFY otpTypeChanged)
    Q_PROPERTY(unsigned int mOutNumberCount READ outNumberCount WRITE setOutNumberCount NOTIFY outNumberCountChanged)
    Q_PROPERTY(unsigned int mTimeStep READ timeStep WRITE setTimeStep NOTIFY timeStepChanged)
    Q_PROPERTY(unsigned int mTimeOffset READ timeOffset WRITE setTimeOffset NOTIFY timeOffsetChanged)
    Q_PROPERTY(unsigned int mAlgorithm READ algorithm WRITE setAlgorithm NOTIFY algorithmChanged)
    Q_PROPERTY(unsigned int mHotpCounter READ hotpCounter WRITE setHotpCounter NOTIFY hotpCounterChanged)
    Q_PROPERTY(QString mIssuer READ issuer WRITE setIssuer NOTIFY issuerChanged)

    // Values used to store the current calculation for the OTP.
    Q_PROPERTY(QString mCurrentCode READ currentCode NOTIFY currentCodeChanged)
    Q_PROPERTY(QString mPrintableCurrentCode READ printableCurrentCode NOTIFY printableCurrentCodeChanged)
    Q_PROPERTY(unsigned int mStartTime READ startTime NOTIFY startTimeChanged)
    Q_PROPERTY(bool mCodeValid READ codeValid NOTIFY codeValidChanged)

    // Get the reason this object appears to be invalid.
    Q_PROPERTY(QString mInvalidReason READ invalidReason NOTIFY invalidReasonChanged)


public:
    KeyEntry();
    KeyEntry(const KeyEntry &toCopy);
    ~KeyEntry();

    void clear();

    bool valid() const;

    // Get/set the key data that is stored.
    QString identifier() const;
    void setIdentifier(const QString &newvalue);

    const ByteArray &secret() const;
    void setSecret(const ByteArray &newvalue);

    const ByteArray &decodedSecret() const;
    void setDecodedSecret(const ByteArray &newvalue);

    unsigned int keyType() const;
    void keyType(unsigned int &value);
    void setKeyType(unsigned int newvalue);

    unsigned int otpType() const;
    void otpType(unsigned int &value);
    void setOtpType(unsigned int newvalue);

    unsigned int outNumberCount() const;
    void outNumberCount(unsigned int &value);
    void setOutNumberCount(unsigned int newvalue);

    unsigned int timeStep() const;
    void timeStep(unsigned int &value);
    void setTimeStep(unsigned int newvalue);

    unsigned int timeOffset() const;
    void timeOffset(unsigned int &value);
    void setTimeOffset(unsigned int newvalue);

    unsigned int algorithm() const;
    void setAlgorithm(unsigned int newvalue);

    unsigned int hotpCounter() const;
    void setHotpCounter(unsigned int newvalue);

    QString issuer() const;
    void setIssuer(const QString &newvalue);

    // Get/set the reason this object is invalid.
    QString invalidReason() const;
    void setInvalidReason(const QString &newvalue);

    // Get/set the values used for calculating and displaying data in the UI.
    QString currentCode() const;
    void setCurrentCode(const QString &newvalue);

    QString printableCurrentCode() const;
    void setPrintableCurrentCode(const QString &newvalue);

    unsigned int startTime() const;
    void setStartTime(unsigned int newvalue);

    bool codeValid() const;
    void setCodeValid(bool newvalue);

    // Utility calls.
    std::string toString();

    KeyEntry& operator=(const KeyEntry& toCopy);

    // Copy all of the values from another KeyEntry object.
    void copyFromObject(const KeyEntry &toCopy);

signals:                    //NOSONAR
    void identifierChanged();
    void secretChanged();
    void decodedSecretChanged();
    void keyTypeChanged();
    void otpTypeChanged();
    void outNumberCountChanged();
    void timeStepChanged();
    void timeOffsetChanged();
    void algorithmChanged();
    void hotpCounterChanged();
    void issuerChanged();
    void currentCodeChanged();
    void printableCurrentCodeChanged();
    void startTimeChanged();
    void codeValidChanged();
    void invalidReasonChanged();

private:
    std::string boolToString(bool value);

    bool mValid;
    QString mIdentifier;
    ByteArray mSecret;
    ByteArray mDecodedSecret;
    unsigned int mKeyType;
    unsigned int mOtpType;
    unsigned int mOutNumberCount;
    unsigned int mTimeStep;
    unsigned int mTimeOffset;
    unsigned int mAlgorithm;
    unsigned int mHotpCounter;
    QString mIssuer;
    QString mInvalidReason;
    QString mCurrentCode;
    QString mPrintableCurrentCode;
    unsigned int mStartTime;
    bool mCodeValid;
};

#endif // KEYENTRY_H
