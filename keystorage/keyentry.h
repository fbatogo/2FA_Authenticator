#ifndef KEYENTRY_H
#define KEYENTRY_H

#include <QObject>
#include <QString>
#include "container/bytearray.h"

const size_t KEYENTRY_KEYTYPE_HEX=0;
const size_t KEYENTRY_KEYTYPE_BASE32=1;
const size_t KEYENTRY_KEYTYPE_MAX=1;     // The highest key type value we can encode.

const size_t KEYENTRY_OTPTYPE_TOTP=0;
const size_t KEYENTRY_OTPTYPE_HOTP=1;
const size_t KEYENTRY_OTPTYPE_MAX=1;     // The highest otp type value we can encode.

const size_t KEYENTRY_ALG_SHA1=0;
const size_t KEYENTRY_ALG_SHA256=1;
const size_t KEYENTRY_ALG_SHA512=2;
const size_t KEYENTRY_ALG_MAX=3;     // The highest algorithm value we can encode.

// This class needs to be derived from QObject so that we can easily use it in the QML code.
class KeyEntry : public QObject
{
    Q_OBJECT

    // Values used to store data about the key.
    Q_PROPERTY(bool mValid READ valid)
    Q_PROPERTY(QString mIdentifier READ identifier WRITE setIdentifier NOTIFY identifierChanged)
    Q_PROPERTY(ByteArray mSecret READ secret WRITE setSecret NOTIFY secretChanged)
    Q_PROPERTY(size_t mKeyType READ keyType WRITE setKeyType NOTIFY keyTypeChanged)
    Q_PROPERTY(size_t mOtpType READ otpType WRITE otpType NOTIFY otpTypeChanged)
    Q_PROPERTY(size_t mOutNumberCount READ outNumberCount WRITE setOutNumberCount NOTIFY outNumberCountChanged)
    Q_PROPERTY(size_t mTimeStep READ timeStep WRITE setTimeStep NOTIFY timeStepChanged)
    Q_PROPERTY(size_t mTimeOffset READ timeOffset WRITE setTimeOffset NOTIFY timeOffsetChanged)
    Q_PROPERTY(size_t mAlgorithm READ algorithm WRITE setAlgorithm NOTIFY algorithmChanged)
    Q_PROPERTY(size_t mHotpCounter READ hotpCounter WRITE setHotpCounter NOTIFY hotpCounterChanged)
    Q_PROPERTY(QString mIssuer READ issuer WRITE setIssuer NOTIFY issuerChanged)

    // Values used to store the current calculation for the OTP.
    Q_PROPERTY(QString mCurrentCode READ currentCode NOTIFY currentCodeChanged)
    Q_PROPERTY(QString mPrintableCurrentCode READ printableCurrentCode NOTIFY printableCurrentCodeChanged)
    Q_PROPERTY(size_t mStartTime READ startTime NOTIFY startTimeChanged)
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

    size_t keyType() const;
    void keyType(size_t &value);
    void setKeyType(size_t newvalue);

    size_t otpType() const;
    void otpType(size_t &value);
    void setOtpType(size_t newvalue);

    size_t outNumberCount() const;
    void outNumberCount(size_t &value);
    void setOutNumberCount(size_t newvalue);

    size_t timeStep() const;
    void timeStep(size_t &value);
    void setTimeStep(size_t newvalue);

    size_t timeOffset() const;
    void timeOffset(size_t &value);
    void setTimeOffset(size_t newvalue);

    size_t algorithm() const;
    void setAlgorithm(size_t newvalue);

    size_t hotpCounter() const;
    void setHotpCounter(size_t newvalue);

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

    size_t startTime() const;
    void setStartTime(size_t newvalue);

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
    size_t mKeyType;
    size_t mOtpType;
    size_t mOutNumberCount;
    size_t mTimeStep;
    size_t mTimeOffset;
    size_t mAlgorithm;
    size_t mHotpCounter;
    QString mIssuer;
    QString mInvalidReason;
    QString mCurrentCode;
    QString mPrintableCurrentCode;
    size_t mStartTime;
    bool mCodeValid;
};

#endif // KEYENTRY_H
