#ifndef OTPHANDLER_H
#define OTPHANDLER_H

#include "otpentry.h"
#include "keystorage/keyentry.h"

class OtpHandler
{
public:
    OtpHandler();

    OtpEntry *calculateFromKeyEntry(const KeyEntry &keydata);

protected:
    QString decodeSecret(const KeyEntry &keydata);
    QString decodeHexKey(const KeyEntry &keydata);
    QString decodeBase32Key(const KeyEntry &keydata);

    QString calculateCode(const KeyEntry &keydata, QString decodedSecret);
    QString calculateHotp(const KeyEntry &keydata, QString decodedSecret);
    QString calculateTotp(const KeyEntry &keydata, QString decodedSecret);
};

#endif // OTPHANDLER_H
