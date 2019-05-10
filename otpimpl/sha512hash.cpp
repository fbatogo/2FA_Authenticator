#include "sha512hash.h"

#include <stdint.h>
#include <cstring>

extern "C" {
#include "sha2.h"                   //NOSONAR
}

ByteArray Sha512Hash::hash(const ByteArray &toHash)
{
    sha512_ctx ctx;
    unsigned char hashResult[64];
    ByteArray result;

    memset(&hashResult, 0x00, sizeof(hashResult));

    sha512_init(&ctx);
    sha512_update(&ctx, toHash.toUCharArrayPtr(), toHash.size());
    sha512_final(&ctx, hashResult);

    result.fromUCharArray(hashResult, sizeof(hashResult));

    return result;
}

size_t Sha512Hash::hashResultLength()
{
    return 64;
}

size_t Sha512Hash::hashBlockLength()
{
    return 128;
}
