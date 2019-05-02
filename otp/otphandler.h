#ifndef OTPHANDLER_H
#define OTPHANDLER_H

#include "keystorage/keyentry.h"
#include "container/bytearray.h"
#include "../otpimpl/hmac.h"

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

    static int getStartTime(int timeStep);

private:
    static std::shared_ptr<Hmac> mHmac;
};

#endif // OTPHANDLER_H
