#ifndef HMAC_H
#define HMAC_H

#include "hashtypebase.h"

class Hmac
{
public:
    Hmac(HashTypeBase *hashType, bool deleteInCtor = false);
    ~Hmac();

    unsigned char *calculate(unsigned char *key, size_t keyLength, unsigned char *data, size_t dataLength, size_t &resultSize);

private:
    HashTypeBase *mHashType;
    unsigned char *mHashResult;
    bool mDeleteInCtor;             // Should we handle deletion of mHashType?
};

#endif // HMAC_H
