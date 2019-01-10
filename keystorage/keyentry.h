#ifndef KEYENTRY_H
#define KEYENTRY_H

#include <QString>

#define KEYENTRY_KEYTYPE_HEX      0
#define KEYENTRY_KEYTYPE_BASE32   1

#define KEYENTRY_OTPTYPE_TOTP     0
#define KEYENTRY_OTPTYPE_HOTP     1


class KeyEntry
{
public:
    KeyEntry();

    void clear();

    bool valid() const;

    QString identifier() const;
    void setIdentifier(const QString &newvalue);

    QString secret() const;
    void setSecert(const QString &newvalue);

    int keyType() const;
    void setKeyType(int newvalue);

    int otpType() const;
    void setOtpType(int newvalue);

    int outNumberCount() const;
    void setOutNumberCount(int newvalue);

private:
    bool mValid;
    QString mIdentifier;
    QString mSecret;
    int mKeyType;
    int mOtpType;
    int mOutNumberCount;
};

#endif // KEYENTRY_H
