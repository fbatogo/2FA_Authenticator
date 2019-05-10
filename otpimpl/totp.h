#ifndef TOTP_H
#define TOTP_H

#include "hmac.h"

#include <string>
#include <cstdint>

class Totp
{
public:
    Totp();
    explicit Totp(std::shared_ptr<Hmac> hmacToUse);

    void setHmac(std::shared_ptr<Hmac> &hmacToUse);

    std::string calculate(const ByteArray &decodedSecret, time_t utcTime, size_t timeStep = 30, size_t digits = 6, uint64_t initialCounter = 0);

private:
    void clear();

    std::shared_ptr<Hmac> mHmacToUse;
};

#endif // TOTP_H
