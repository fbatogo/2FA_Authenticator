#ifndef SHA256HASH_H
#define SHA256HASH_H

#include "hashtypebase.h"

class Sha256Hash : public HashTypeBase
{
public:
    Sha256Hash() = default;
    ~Sha256Hash() = default;

    unsigned char *hash(unsigned char *bytes, size_t bytesLength);
    size_t hashResultLength();
    size_t hashBlockLength();

private:
    unsigned char mHashResult[32];
};

#endif // SHA256HASH_H
