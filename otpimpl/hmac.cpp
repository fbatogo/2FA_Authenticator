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
    size_t blocks;
    size_t bytesIn;
    unsigned char *oneBlock = nullptr;    // In a multi-block calculation, this variable will hold a single block.

    // Validate inputs.
    if ((key == nullptr) || (data == nullptr)) {
        // Can't do anything with this.
        return nullptr;
    }

    // If the data length is <= one hash block size, then just calculate it, and return it.
    if (dataLength <= mHashType->hashBlockLength()) {
        mHashResult = hmacOneBlock(key, keyLength, data, dataLength, resultSize);

        return mHashResult;
    }

    // Otherwise, we need to break the data in to appropriate sized data blocks, hash those blocks
    // one at a time, and return the values concatenated together.  (The caller can know there is
    // more that one result because the result size will be greater that the value returned from
    // the hashResultLength() value in the hash object.

    blocks = dataLength / mHashType->hashBlockLength();

    if ((dataLength % mHashType->hashBlockLength()) > 0) {
        // Add one more block to our count.
        blocks++;
    }

    // Allocate enough memory to hold all of the resulting hashes.
    resultSize = (blocks * mHashType->hashResultLength());
    // XXX

    for (size_t i = 0; i < blocks; i++) {
        // Figure out how many bytes in that we want to copy the block from.
        bytesIn = i * mHashType->hashBlockLength();

        if ((bytesIn + mHashType->hashBlockLength()) > dataLength) {
            // Handle a piece that is less than the block length long.
            // XXX
        } else {
            // Handle a piece that is one block length long.
            // XXX
        }

        // Concatenate the hashed value in to our final result.
        // XXX
    }

    // Return the resulting data.
    return mHashResult;
}

/**
 * @brief Hmac::hmacOneBlock - Generate an HMAC of a single block of data, using the has method provided when
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
unsigned char *Hmac::hmacOneBlock(unsigned char *key, size_t keyLength, unsigned char *data, size_t dataLength, size_t &resultSize)
{
    unsigned char *keyIpad;
    size_t keyIpadLength;
    unsigned char *keyOpad;
    size_t keyOpadLength;
    unsigned char *iPadHashed;
    unsigned char *result;
    unsigned char *keyToUse;
    unsigned char *returnData = nullptr;

    // Validate inputs.
    if ((key == nullptr) || (data == nullptr)) {
        // Nothing we can do.
        return nullptr;
    }

    keyToUse = key;

    // Calculate the HMAC over the 'data' by using the formula :
    //    Hash(key XOR opad, Hash(key XOR ipad, data))

    // If the key length is larger than one block, we need to hash the key to come up
    // with a key of decreased size.
    if (keyLength > mHashType->hashBlockLength()) {
        // Create a hash of the key to use.
        keyToUse = mHashType->hash(key, keyLength);
        keyLength = mHashType->hashResultLength();
    }

    // Figure out how much memory each pad will ultimately need.
    keyIpadLength = mHashType->hashBlockLength() + dataLength;
    keyOpadLength = mHashType->hashBlockLength() + mHashType->hashResultLength();

    // Allocate the memory for the key XOR i/opad values and the data concatenated together.
    keyIpad = static_cast<unsigned char *>(calloc(1, keyIpadLength));
    keyOpad = static_cast<unsigned char *>(calloc(1, keyOpadLength));

    // Then, copy the key in to the Ipad and Opad buffers.
    memcpy(keyIpad, keyToUse, keyLength);
    memcpy(keyOpad, keyToUse, keyLength);

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

    // Set the result size.
    resultSize = mHashType->hashResultLength();

    // Allocate the memory to store the hash result.
    returnData = static_cast<unsigned char *>(calloc(1, mHashType->hashResultLength()));

    // Copy the result data.
    memcpy(returnData, result, mHashType->hashResultLength());

    // And, return the result.
    return returnData;
}

