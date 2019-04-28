#ifndef HMAC_H
#define HMAC_H

#include "hashtypebase.h"

class Hmac
{
public:
    Hmac();
    Hmac(Hmac &toCopy);
    Hmac(HashTypeBase *hashType, bool deleteInCtor = false);
    ~Hmac();

    void setHashType(HashTypeBase *hashType, bool takeOwnership = false);

    unsigned char *calculate(const unsigned char *key, size_t keyLength, unsigned char *data, size_t dataLength, size_t &resultSize);

    Hmac& operator=(const Hmac& toCopy);

private:
    void clear();
    void copy(const Hmac &toCopy);

    HashTypeBase *mHashType;
    unsigned char *mHashResult;
    bool mDeleteInCtor;             // Should we handle deletion of mHashType?
};

#endif // HMAC_H
