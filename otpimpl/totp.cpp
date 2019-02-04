#include "totp.h"

#include "hotp.h"

Totp::Totp(Hmac *hmacToUse, bool shouldDelete)
{
    mHmacToUse = hmacToUse;
    mShouldDelete = shouldDelete;
}

Totp::~Totp()
{
    if (mShouldDelete) {
        delete mHmacToUse;
    }

    mHmacToUse = nullptr;
}

std::string Totp::calculate(unsigned char *key, size_t keyLength, uint64_t utcTime, size_t timeStep, size_t digits, uint64_t initialCounter)
{
    Hotp hotp(mHmacToUse);
    uint64_t calcTime;

    calcTime = (utcTime - initialCounter)/timeStep;

    // Then, calculate the HOTP using the key, and the calcTime.
    return hotp.calculate(key, keyLength, calcTime, digits);
}
