#ifndef SHA512HASH_H
#define SHA512HASH_H

#include "hashtypebase.h"

class Sha512Hash : public HashTypeBase
{
public:
    Sha512Hash() = default;
    ~Sha512Hash() = default;

    ByteArray hash(const ByteArray &toHash);
    size_t hashResultLength();
    size_t hashBlockLength();
};

#endif // SHA512HASH_H
