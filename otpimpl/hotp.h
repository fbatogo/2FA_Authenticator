#ifndef HOTP_H
#define HOTP_H

#include <cstdlib>

// Define the valid hash type values that are supported by this class.
#define HOTP_HASH_SHA1      0

class Hotp
{
public:
    Hotp();

    unsigned char *calculate(unsigned char *key, size_t counter, size_t hashType = HOTP_HASH_SHA1);

private:
    unsigned char *calculateSha1Hotp(unsigned char *key, size_t counter);

    unsigned char *mCalculatedValue;
};

#endif // HOTP_H
