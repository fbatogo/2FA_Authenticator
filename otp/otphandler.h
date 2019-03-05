#ifndef OTPHANDLER_H
#define OTPHANDLER_H

#include "otpentry.h"
#include "keystorage/keyentry.h"

class OtpHandler
{
public:
    OtpHandler();

    static OtpEntry *calculateFromKeyEntry(const KeyEntry &keydata);

protected:
    static bool decodeSecret(const KeyEntry &keydata, unsigned char **decodedSecret, size_t *decodedSize);
    static bool decodeBase32Key(const KeyEntry &keydata, unsigned char **decodedSecret, size_t *decodedSize);
    static bool decodeHexKey(const KeyEntry &keydata, unsigned char **decodedSecret, size_t *decodedSize);

    static QString calculateCode(const KeyEntry &keydata, const unsigned char *decodedSecret, size_t decodedSize);
    static QString calculateTotp(const KeyEntry &keydata, const unsigned char *decodedSecret, size_t decodedSize);
    static QString calculateHotp(const KeyEntry &keydata, const unsigned char *decodedSecret, size_t decodedSize);

    static int getStartTime(int timeStep);

    static OtpEntry *createOtpEntry(const KeyEntry &keydata, const QString &calculatedCode, int startTime, const QString &invalidReason = "");
};

#endif // OTPHANDLER_H
