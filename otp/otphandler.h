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

    static QString calculateCode(const KeyEntry &keydata);
    static QString calculateTotp(const KeyEntry &keydata);
    static QString calculateHotp(const KeyEntry &keydata);

    static unsigned int getStartTime(unsigned int timeStep);

private:
    static std::shared_ptr<Hmac> getHmacForKeyData(const KeyEntry &keydata);
};

#endif // OTPHANDLER_H
