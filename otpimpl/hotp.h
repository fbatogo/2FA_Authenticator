#ifndef HOTP_H
#define HOTP_H

#include <string>
#include <cstdlib>
#include <cstdint>

#include "hmac.h"

class Hotp
{
public:
    Hotp(Hmac *hmacToUse, bool shouldDelete = false);
    ~Hotp();

    std::string calculate(const unsigned char *key, size_t keyLength, uint64_t counter, size_t digits, bool addChecksum = false, int truncationOffset = -1);

private:
    std::string calculateSha1Hotp(unsigned char *key, size_t keyLength, uint64_t counter, size_t digits, bool addChecksum, int truncationOffset);
    std::string calculateHotpFromHmac(const unsigned char *hmac, size_t hmacSize, size_t digits, bool addChecksum, int truncationOffset);
    int64_t calcChecksum(int64_t otp, size_t digits);
    unsigned char *dynamicTruncate(const unsigned char *hmac, size_t hmacSize, size_t truncateOffset);

    Hmac *mHmacToUse;
    bool mShouldDelete;
};

#endif // HOTP_H
