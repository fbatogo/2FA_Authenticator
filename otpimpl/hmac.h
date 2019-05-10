#ifndef HMAC_H
#define HMAC_H

#include <memory>
#include "hashtypebase.h"
#include "container/bytearray.h"

class Hmac
{
public:
    Hmac();
    Hmac(Hmac &toCopy);
    explicit Hmac(std::shared_ptr<HashTypeBase> hashType);
    ~Hmac();

    void setHashType(const std::shared_ptr<HashTypeBase> &hashType);

    std::shared_ptr<ByteArray> calculate(const ByteArray &key, const ByteArray &data);

    Hmac& operator=(const Hmac& toCopy);

private:
    void clear();
    void copy(const Hmac &toCopy);

    std::shared_ptr<HashTypeBase> mHashType;
    std::shared_ptr<ByteArray> mHashResult;
};

#endif // HMAC_H
