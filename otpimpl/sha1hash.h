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

    ByteArray hash(const ByteArray &toHash);
    size_t hashResultLength();
    size_t hashBlockLength();
};

#endif // SHA1HASH_H
