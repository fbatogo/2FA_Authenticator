#ifndef TOTP_H
#define TOTP_H

#include "hmac.h"

#include <string>
#include <cstdint>

class Totp
{
public:
    Totp();
    Totp(Totp &toCopy);
    Totp(std::shared_ptr<Hmac> hmacToUse);
    ~Totp();

    void setHmac(std::shared_ptr<Hmac> &hmacToUse);

    std::string calculate(const ByteArray &decodedSecret, time_t utcTime, size_t timeStep = 30, size_t digits = 6, uint64_t initialCounter = 0);

    Totp& operator=(const Totp& toCopy);

private:
    void clear();
    void copy(const Totp& toCopy);

    std::shared_ptr<Hmac> mHmacToUse;
};

#endif // TOTP_H
