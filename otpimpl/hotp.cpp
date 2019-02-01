#include "hotp.h"

extern "C" {
    #include "sha1impl.h"
}

Hotp::Hotp()
{

}

/**
 * @brief Hotp::calculate - Calculate and return an HOTP value.
 *
 * @param key - The DECODED key to use.  (20 byte string, *NOT* base32 encoded)
 * @param counter - The counter value to use.
 *
 * @return unsigned char pointer to the HOTP value.  This result is owned by this
 *      class, and MUST NOT be freed by the caller!  On error, nullptr is returned.
 */
unsigned char *Hotp::calculate(unsigned char *key, size_t counter, size_t hashType)
{
    // Validate inputs.
    if (key == nullptr) {
        // We can't work with this!
        return nullptr;
    }

    // For now, we only support SHA1, but in the future we might support other types,
    // so make sure nothing but SHA1 is passed in as out hashType.
    if (hashType != HOTP_HASH_SHA1) {
        // Nope!
        return nullptr;
    }

    return calculateSha1Hotp(key, counter);
}

/**
 * @brief Hotp::calculateSha1Hotp - Implement the HOTP algorithm, using SHA1 as defined
 *      in section 5.3 of RFC 4226.
 *
 * @param key - A 20 byte string of binary characeters used as the key.
 * @param counter - The counter value used to calculate the HOTP.
 *
 * @return unsigned char pointer to the HOTP value.  This result is owned by this
 *      class, and MUST NOT be freed by the caller!  On error, nullptr is returned.
 */
unsigned char *Hotp::calculateSha1Hotp(unsigned char *key, size_t counter)
{
    // Since this method is private, and we already validated inputs in the caller, we can skip that
    // here, and get on with it!

    // Calculate the SHA1 hash of the key and counter.

}
