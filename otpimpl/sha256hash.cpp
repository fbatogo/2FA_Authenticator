#include "sha256hash.h"

#include <cstring>

extern "C" {
#include "otpimpl/sha2.h"
}

ByteArray Sha256Hash::hash(const ByteArray &toHash)
{
    sha256_ctx ctx;
    unsigned char hashResult[32];
    ByteArray result;

    memset(&hashResult, 0x00, sizeof(hashResult));

    sha256_init(&ctx);
    sha256_update(&ctx, reinterpret_cast<const unsigned char*>(toHash.toCharArrayPtr()), static_cast<int>(toHash.size()));
    sha256_final(&ctx, hashResult);

    // Store it in our member variable.
    result.fromUCharArray(hashResult, sizeof(hashResult));

    return result;
}

size_t Sha256Hash::hashResultLength()
{
    return 32;
}

size_t Sha256Hash::hashBlockLength()
{
    return 64;
}
