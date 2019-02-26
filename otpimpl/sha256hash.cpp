#include "sha256hash.h"

#include "sha256impl.h"
#include <cstring>

unsigned char *Sha256Hash::hash(unsigned char *bytes, size_t bytesLength)
{
    if (bytes == nullptr) {
        // Nothing we can do.
        return nullptr;
    }

    memset(&mHashResult, 0x00, 20);

    SHA256(reinterpret_cast<unsigned char *>(&mHashResult), reinterpret_cast<char *>(bytes), static_cast<int>(bytesLength));

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
