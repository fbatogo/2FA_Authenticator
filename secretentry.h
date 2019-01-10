#ifndef SECRETENTRY_H
#define SECRETENTRY_H

#include <QString>

class SecretEntry
{
public:
    SecretEntry();

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
    QString mIdentifier;
    QString mSecret;
    int mKeyType;
    int mOtpType;
    int mOutNumberCount;
};

#endif // SECRETENTRY_H
