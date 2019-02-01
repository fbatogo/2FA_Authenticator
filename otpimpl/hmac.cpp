#include "hmac.h"

#include <cstring>
#include <iostream>

Hmac::Hmac(HashTypeBase *hashType, bool deleteInCtor)
{
    mHashType = hashType;
    mHashResult = nullptr;
    mDeleteInCtor = deleteInCtor;
}

Hmac::~Hmac()
{
    // If we have a result stored, free the memory.
    if (mHashResult != nullptr) {
        free(mHashResult);
        mHashResult = nullptr;
    }

    if (mDeleteInCtor) {
        delete mHashType;
    }
    mHashType = nullptr;
}

/**
 * @brief Hmac::calculate - Generate an HMAC of the provided data, using the has method provided when
 *      this object was created.
 *
 * @param key - The key data that should be used to generate the HMAC.
 * @param keyLength - The length of the key data provided.
 * @param data - The data that we want to get the HMAC for.
 * @param dataLength - The length of the data that we want to get the HMAC for.
 * @param resultSize - If this method returns true, this parameter will contain the length of the
 *      HMAC data that was returned.
 *
 * @return unsigned char * containing the calculated HMAC value.  On error, nullptr will be
 *      returned.
 */
unsigned char *Hmac::calculate(unsigned char *key, size_t keyLength, unsigned char *data, size_t dataLength, size_t &resultSize)
{
    unsigned char *keyIpad;
    size_t keyIpadLength;
    unsigned char *keyOpad;
    size_t keyOpadLength;
    unsigned char *iPadHashed;
    unsigned char *result;

    // Validate inputs.
    if ((key == nullptr) || (data == nullptr)) {
        // Nothing we can do.
        return nullptr;
    }

    // Calculate the HMAC over the 'data' by using the formula :
    //    Hash(key XOR opad, Hash(key XOR ipad, data))

    // Figure out how much memory each pad will ultimately need.
    keyIpadLength = mHashType->hashBlockLength() + dataLength;
    keyOpadLength = mHashType->hashBlockLength() + mHashType->hashResultLength();

    // Allocate the memory for the key XOR i/opad values and the data concatenated together.
    keyIpad = static_cast<unsigned char *>(calloc(1, keyIpadLength));
    keyOpad = static_cast<unsigned char *>(calloc(1, keyOpadLength));

    // Then, copy the key in to the Ipad and Opad buffers.
    memcpy(keyIpad, key, keyLength);
    memcpy(keyOpad, key, keyLength);

    // Iterate the two values, XORing them with 0x36 for the ipad, and
    // 0x5c for the opad.
    for (size_t i = 0; i < mHashType->hashBlockLength(); i++) {
        keyIpad[i] ^= 0x36;
        keyOpad[i] ^= 0x5c;
    }

    // Then, concatenate the data on to the inner hash value.
    memcpy(&keyIpad[mHashType->hashBlockLength()], data, dataLength);

    // Then, perform the inner hash on the data provided.
    iPadHashed = mHashType->hash(keyIpad, keyIpadLength);

    // Then, concatenate the iPadHashed value to the keyOpad value.
    memcpy(&keyOpad[mHashType->hashBlockLength()], iPadHashed, mHashType->hashResultLength());

    // And hash that.
    result = mHashType->hash(keyOpad, keyOpadLength);

    // Finally, we need to make a copy of the data, since the Hash object retains ownership of
    // the resulting value.
    if (mHashResult != nullptr) {
        // Free the current memory.
        free(mHashResult);
        mHashResult = nullptr;
    }

    // Set the result size.
    resultSize = mHashType->hashResultLength();

    // Allocate the memory to store the hash result.
    mHashResult = static_cast<unsigned char *>(calloc(1, mHashType->hashResultLength()));

    // Copy the result data.
    memcpy(mHashResult, result, mHashType->hashResultLength());

    // And, return the result.
    return mHashResult;
}

