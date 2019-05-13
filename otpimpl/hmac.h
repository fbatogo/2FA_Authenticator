#ifndef HMAC_H
#define HMAC_H

#include <memory>
#include "hashtypebase.h"
#include "container/bytearray.h"

class Hmac
{
public:
    Hmac();
    explicit Hmac(std::shared_ptr<HashTypeBase> hashType);
    ~Hmac();

    void setHashType(const std::shared_ptr<HashTypeBase> &hashType);

    std::shared_ptr<ByteArray> calculate(const ByteArray &key, const ByteArray &data);

private:
    void clear();

    std::shared_ptr<HashTypeBase> mHashType;
    std::shared_ptr<ByteArray> mHashResult;
};

#endif // HMAC_H
