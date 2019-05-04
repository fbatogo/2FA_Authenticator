#ifndef OTPHANDLER_H
#define OTPHANDLER_H

#include "keystorage/keyentry.h"
#include "container/bytearray.h"
#include "../otpimpl/hmac.h"
#include <memory>

class OtpHandler
{
public:
    OtpHandler();

    static void calculateOtpForKeyEntry(KeyEntry *keydata);

protected:
    static bool decodeSecret(const KeyEntry &keydata, ByteArray &decodedSecret);
    static bool decodeBase32Key(const KeyEntry &keydata, ByteArray &decodedSecret);
    static bool decodeHexKey(const KeyEntry &keydata, ByteArray &decodedSecret);

    static QString calculateCode(const KeyEntry &keydata, const ByteArray &decodedSecret);
    static QString calculateTotp(const KeyEntry &keydata, const ByteArray &decodedSecret);
    static QString calculateHotp(const KeyEntry &keydata, const ByteArray &decodedSecret);

    static size_t getStartTime(size_t timeStep);
};

#endif // OTPHANDLER_H
