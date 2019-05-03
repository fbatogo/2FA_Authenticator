#include "sha512hash.h"

#include <stdint.h>

extern "C" {
#include "sha2.h"
}

#include <cstring>

ByteArray Sha512Hash::hash(const ByteArray &toHash)
{
    sha512_ctx ctx;
    unsigned char hashResult[64];

    memset(&hashResult, 0x00, sizeof(hashResult));

    sha512_init(&ctx);
    sha512_update(&ctx, toHash.toUCharArrayPtr(), toHash.size());
    sha512_final(&ctx, reinterpret_cast<unsigned char *>(&hashResult));

    mHashResult.fromUCharArray(hashResult, sizeof(hashResult));

    return mHashResult;
}

size_t Sha512Hash::hashResultLength()
{
    return 64;
}

size_t Sha512Hash::hashBlockLength()
{
    return 128;
}
