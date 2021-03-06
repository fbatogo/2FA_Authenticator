#ifndef HOTP_H
#define HOTP_H

#include <string>
#include <cstdlib>
#include <cstdint>

#include "hmac.h"

class Hotp
{
public:
    Hotp();
    explicit Hotp(std::shared_ptr<Hmac> &hmacToUse);

    void setHmac(std::shared_ptr<Hmac> &hmacToUse);

    std::string calculate(const ByteArray &key, uint64_t counter, size_t digits, bool addChecksum = false, int truncationOffset = -1);

private:
    std::string calculateSha1Hotp(unsigned char *key, size_t keyLength, uint64_t counter, size_t digits, bool addChecksum, int truncationOffset);
    std::string calculateHotpFromHmac(const ByteArray &hmac, size_t digits, bool addChecksum, int truncationOffset);
    int64_t calcChecksum(int64_t otp, size_t digits);
    ByteArray dynamicTruncate(const ByteArray &hmac, size_t truncateOffset);

    std::shared_ptr<Hmac> mHmacToUse;
};

#endif // HOTP_H
