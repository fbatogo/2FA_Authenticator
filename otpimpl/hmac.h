#ifndef HMAC_H
#define HMAC_H

#include "hashtypebase.h"

class Hmac
{
public:
    Hmac();
    Hmac(HashTypeBase *hashType, bool deleteInCtor = false);
    ~Hmac();

    void setHashType(HashTypeBase *hashType, bool takeOwnership = false);

    unsigned char *calculate(const unsigned char *key, size_t keyLength, unsigned char *data, size_t dataLength, size_t &resultSize);

private:
    void clear();

    HashTypeBase *mHashType;
    unsigned char *mHashResult;
    bool mDeleteInCtor;             // Should we handle deletion of mHashType?
};

#endif // HMAC_H
