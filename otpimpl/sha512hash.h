#ifndef SHA512HASH_H
#define SHA512HASH_H

#include "hashtypebase.h"

class Sha512Hash : public HashTypeBase
{
public:
    Sha512Hash() = default;
    ~Sha512Hash() = default;

    unsigned char *hash(unsigned char *bytes, size_t bytesLength);
    size_t hashResultLength();
    size_t hashBlockLength();

private:
    unsigned char mHashResult[64];
};

#endif // SHA512HASH_H
