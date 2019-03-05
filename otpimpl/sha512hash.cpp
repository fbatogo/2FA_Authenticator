#include "sha512hash.h"

#include <stdint.h>

extern "C" {
#include "sha2.h"
}

#include <cstring>

unsigned char *Sha512Hash::hash(unsigned char *bytes, size_t bytesLength)
{
    sha512_ctx ctx;

    if (bytes == nullptr) {
        // Nothing we can do.
        return nullptr;
    }

    memset(&mHashResult, 0x00, sizeof(mHashResult));

    // Test string is "abc".
    sha512_init(&ctx);
    sha512_update(&ctx, bytes, bytesLength);
    sha512_final(&ctx, (unsigned char *)&mHashResult);

    return reinterpret_cast<unsigned char *>(&mHashResult);
}

size_t Sha512Hash::hashResultLength()
{
    return 64;
}

size_t Sha512Hash::hashBlockLength()
{
    return 128;
}
