#ifndef SHA1HASH_H
#define SHA1HASH_H

#include "hashtypebase.h"

/**
 * @brief The Sha1Hash class is a wrapper about the sha1impl.c/h SHA1 implementation to
 *      make it more usable with other objects in this program.
 */
class Sha1Hash : public HashTypeBase
{
public:
    Sha1Hash() = default;
    ~Sha1Hash() = default;

    std::shared_ptr<ByteArray> hash(const std::shared_ptr<ByteArray> &toHash);
    size_t hashResultLength();
    size_t hashBlockLength();

private:
    unsigned char mHashResult[20];
};

#endif // SHA1HASH_H
