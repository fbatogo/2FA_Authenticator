#ifndef TOTP_H
#define TOTP_H

#include "hmac.h"

#include <string>
#include <cstdint>

class Totp
{
public:
    Totp(Hmac *hmacToUse, bool shouldDelete = false);
    ~Totp();

    std::string calculate(unsigned char *key, size_t keyLength, uint64_t utcTime, size_t timeStep, size_t digits, uint64_t initialCounter = 0);

private:
    Hmac *mHmacToUse;
    bool mShouldDelete;
};

#endif // TOTP_H
