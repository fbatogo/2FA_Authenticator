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
    sha512_update(&ctx, toHash.toCharArrayPtr(), toHash.toCharArraySize());
    sha512_final(&ctx, (unsigned char *)&hashResult);

    mHashResult.fromCharArray(hashResult, sizeof(hashResult));

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
