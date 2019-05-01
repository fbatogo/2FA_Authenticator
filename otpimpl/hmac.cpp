#include "hmac.h"

#include <cstring>
#include <iostream>
#include "../logger.h"

Hmac::Hmac()
{
    mHashType = nullptr;
    mHashResult = nullptr;
}

Hmac::Hmac(Hmac &toCopy)
{
    copy(toCopy);
}

Hmac::Hmac(std::shared_ptr<HashTypeBase> hashType) :
    mHashType(hashType)
{
    mHashResult = nullptr;
}

Hmac::~Hmac()
{
    clear();
}

/**
 * @brief Hmac::setHashType - Set the hash algorithm to use to create an HMAC.  This
 *      is an alternative to using the ctor to set this.
 *
 * @param hashType - A HashTypeBase object that implements the hash algorithm to be used
 *      to create an HMAC.
 */
void Hmac::setHashType(const std::shared_ptr<HashTypeBase> &hashType)
{
    // Clean out anything that might already be configured.
    clear();

    // Set the new values.
    mHashType = hashType;
}

/**
 * @brief Hmac::calculate - Generate an HMAC of the provided data, using the has method provided when
 *      this object was created.
 *
 * @param key - The key data that should be used to generate the HMAC.
 * @param data - The data that we want to get the HMAC for.
 *
 * @return unsigned char * containing the calculated HMAC value.  On error, nullptr will be
 *      returned.
 */
std::shared_ptr<ByteArray> Hmac::calculate(const std::shared_ptr<ByteArray> &key, const std::shared_ptr<ByteArray> &data)
{
    ByteArray keyIpad;
    size_t keyIpadLength;
    ByteArray keyOpad;
    size_t keyOpadLength;
    ByteArray iPadHashed;
    ByteArray result;
    ByteArray keyToUse;
    size_t keyLength;

    // Validate inputs.
    if ((key == nullptr) || (data == nullptr)) {
        // Nothing we can do.
        LOG_ERROR("Either the key or data block provided to HMAC was NULL!");
        return nullptr;
    }

    // Make sure we are properly configured to do the hashing.
    if (mHashType == nullptr) {
        // Need to have the object set before we can calculate the HMAC.
        LOG_ERROR("No hash object was configured when attempting to create an HMAC value!");
        return nullptr;
    }

    // Make a copy of our key data.
    keyToUse = (*key.get());

    // Start by settng the key length to the length of the key data provided.
    keyLength = keyToUse.size();

    // Calculate the HMAC over the 'data' by using the formula :
    //    Hash(key XOR opad, Hash(key XOR ipad, data))

    // If the key length is larger than one block, we need to hash the key to come up
    // with a key of decreased size.
    if (keyToUse.size() > mHashType->hashBlockLength()) {
        // Create a hash of the key to use.
        keyToUse = mHashType->hash(keyToUse);
        keyLength = mHashType->hashResultLength();
    }

    // Figure out how much memory each pad will ultimately need.
    keyIpadLength = mHashType->hashBlockLength() + dataLength;
    keyOpadLength = mHashType->hashBlockLength() + mHashType->hashResultLength();

    // Allocate the memory for the key XOR i/opad values and the data concatenated together.
    keyIpad = static_cast<unsigned char *>(calloc(1, keyIpadLength));
    if (keyIpad == nullptr) {
        // Failed to allocate memory.
        LOG_ERROR("Failed to allocate memory to store the key ipad value!");
        return nullptr;
    }

    keyOpad = static_cast<unsigned char *>(calloc(1, keyOpadLength));
    if (keyOpad == nullptr) {
        // Failed to allocate memory.
        LOG_ERROR("Failed to allocate memory to store the key opad value!");
        return nullptr;
    }

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

    // Free the memory we used.
    if (keyIpad != nullptr) {
        free(keyIpad);
        keyIpad = nullptr;
    }

    if (keyOpad != nullptr) {
        free(keyOpad);
        keyOpad = nullptr;
    }

    // Clear the hash result.
    mHashResult = nullptr;

    // Store the hash result.
    mHashResult->fromCharArray(static_cast<char *>(result), mHashType->hashResultLength());

    // And, return the result.
    return mHashResult;
}

Hmac &Hmac::operator=(const Hmac &toCopy)
{
    if (this == &toCopy) {
        return (*this);
    }

    copy(toCopy);

    return (*this);
}

/**
 * @brief Hmac::clear - Clean up the internal variables, deleting any objects that we are
 *      configured to delete.
 */
void Hmac::clear()
{
    mHashType = nullptr;
}

/**
 * @brief Hmac::copy - Copy the members of the \c toCopy object in to the current
 *      object.
 *
 * @param toCopy - The object to copy from.
 */
void Hmac::copy(const Hmac &toCopy)
{
    mHashType = toCopy.mHashType;
    mHashResult = toCopy.mHashResult;
}

