#include "sha256hash.h"

extern "C" {
#include "otpimpl/sha2.h"
}

#include <cstring>

unsigned char *Sha256Hash::hash(unsigned char *bytes, size_t bytesLength)
{
    sha256_ctx ctx;

    if (bytes == nullptr) {
        // Nothing we can do.
        return nullptr;
    }

    memset(&mHashResult, 0x00, sizeof(mHashResult));

    sha256_init(&ctx);
    sha256_update(&ctx, bytes, bytesLength);
    sha256_final(&ctx, (unsigned char *)&mHashResult);

    return reinterpret_cast<unsigned char *>(&mHashResult);
}

size_t Sha256Hash::hashResultLength()
{
    return 32;
}

size_t Sha256Hash::hashBlockLength()
{
    return 64;
}
