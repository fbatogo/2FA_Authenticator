#include "sha256hash.h"

extern "C" {
#include "otpimpl/sha2.h"
}

#include <cstring>

ByteArray Sha256Hash::hash(const ByteArray &toHash)
{
    sha256_ctx ctx;
    unsigned char hashResult[32];

    memset(&hashResult, 0x00, sizeof(hashResult));

    sha256_init(&ctx);
    sha256_update(&ctx, reinterpret_cast<unsigned char*>(toHash.toCharArrayPtr()), static_cast<int>(toHash.toCharArraySize()));
    sha256_final(&ctx, reinterpret_cast<unsigned char *>(&hashResult));

    // Store it in our member variable.
    mHashResult.fromCharArray(reinterpret_cast<char *>(&hashResult), sizeof(hashResult));

    return mHashResult;
}

size_t Sha256Hash::hashResultLength()
{
    return 32;
}

size_t Sha256Hash::hashBlockLength()
{
    return 64;
}
