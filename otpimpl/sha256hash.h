#ifndef SHA256HASH_H
#define SHA256HASH_H

#include "hashtypebase.h"

class Sha256Hash : public HashTypeBase
{
public:
    Sha256Hash() = default;
    ~Sha256Hash() = default;

    ByteArray hash(const ByteArray &toHash);
    size_t hashResultLength();
    size_t hashBlockLength();
};

#endif // SHA256HASH_H
