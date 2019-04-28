#include "hotp.h"

#include "hmac.h"
#include "sha1hash.h"
#include "../logger.h"

Hotp::Hotp()
{
    mHmacToUse = nullptr;
    mShouldDelete = false;
}

Hotp::Hotp(Hotp &toCopy)
{
    copy(toCopy);
}

Hotp::Hotp(Hmac *hmacToUse, bool shouldDelete)
{
    mHmacToUse = hmacToUse;
    mShouldDelete = shouldDelete;
}

Hotp::~Hotp()
{
    clear();
}

/**
 * @brief Hotp::setHmac - Set the HMAC object to use when calculating the HOTP value.
 *
 * @param hmacToUse - The HMAC object that should be used to calculate the HOTP value.
 * @param takeOwnership - If true, this object will take ownership of the HMAC object, and
 *      will delete it in the dtor.  If false, the caller is responsible for freeing the
 *      HMAC object when it knows it is no longer in use.
 */
void Hotp::setHmac(Hmac *hmacToUse, bool takeOwnership)
{
    // If we are currently set to take ownership of the HMAC object, and an HMAC object
    // is set, clean it up.
    clear();

    // Set the new values to use.
    mHmacToUse = hmacToUse;
    mShouldDelete = takeOwnership;
}

/**
 * @brief Hotp::calculate - Calculate and return an HOTP value.
 *
 * @param key - The DECODED key to use.  (20 byte string, *NOT* base32 encoded)
 * @param counter - The counter value to use.
 *
 * @return std::string containing the HOTP value.  On error, this string will be
 *      empty.
 */
std::string Hotp::calculate(const unsigned char *key, size_t keyLength, uint64_t counter, size_t digits, bool addChecksum, int truncationOffset)
{
    unsigned char *hashValue;
    size_t resultSize;
    unsigned char number[8];
    unsigned char inverse;

    // Validate inputs.
    if (key == nullptr) {
        // We can't work with this!
        LOG_ERROR("A null key was provided for the HOTP calculation!");
        return "";
    }

    // Make sure we are configured properly.
    if (mHmacToUse == nullptr) {
        LOG_ERROR("No HMAC object was set when attempting to calculate an HOTP value!");
        return "";
    }

    if ((digits < 6) || (digits > 8)) {
        LOG_ERROR("The number of digits provided must be 6, 7, or 8!");
        return "";
    }

    // Convert the value of the counter to an unsigned char *, without worrying about endianness.
    inverse = 7;
    for (size_t i = 0; i < 8; i++) {        // A 64 bit number is 8 bytes.
        number[i] = ((counter >> (inverse * 8)) & 0xff);
        inverse--;
    }

    // Calculate the SHA1 hash of the key and counter.
    hashValue = mHmacToUse->calculate(key, keyLength, number, 8, resultSize);
    if (hashValue == nullptr) {
        LOG_ERROR("Failed to caculate HMAC-SHA1 portion of the HOTP!");
        return "";
    }

    return calculateHotpFromHmac(hashValue, resultSize, digits, addChecksum, truncationOffset);
}

Hotp &Hotp::operator=(const Hotp &toCopy)
{
    copy(toCopy);

    return (*this);
}

/**
 * @brief Hotp::clear - If necessary, delete the current HMAC object.  Then, set the current
 *      object to be a nullptr.
 */
void Hotp::clear()
{
    if ((mShouldDelete) && (mHmacToUse != nullptr)) {
        delete mHmacToUse;
    }

    mHmacToUse = nullptr;
}

/**
 * @brief Hotp::calculateHotpFromHmac - Calculate the HOTP from the HMAC that was calculated against the
 *      key and counter that were originally passed in.
 *
 * @param hmac - The HMAC that was calculated using one of the hashing algorithms.
 * @param hmacSize - The number of bytes that the HMAC is.
 * @param digits - The number of digits that the HOTP should have. (6, 7, or 8)
 * @param addChecksum - If true, we will add checksum digits to the HOTP value.
 * @param truncationOffset - A value of 0..(hmacSize - 4) that indicates the truncation value we should use.  If this
 *      value is anything but 0..15, dynamic truncation will be used.
 *
 * @return std::string containing the calculated HOTP value. On error, the string will be empty.
 */
std::string Hotp::calculateHotpFromHmac(const unsigned char *hmac, size_t hmacSize, size_t digits, bool addChecksum, int truncationOffset)
{
    int64_t DIGITS_POWER[9] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000 };
    unsigned char *truncValue;
    size_t calcDigits;
    int32_t truncData;
    int64_t otp;
    std::string result;

    // Validate inputs.
    if (hmac == nullptr) {
        LOG_ERROR("No HMAC value was provided to calculate the HOTP!");
        return "";
    }

    if ((digits < 6) || (digits > 8)) {
        LOG_ERROR("An invalid number of digits was requested!  It must be 6, 7, or 8!");
        return "";
    }

    // Figure out how many digits to calculate.
    calcDigits = addChecksum ? (digits + 1) : digits;

    // Do the dynamic truncation on it.
    truncValue = dynamicTruncate(hmac, hmacSize, truncationOffset);
    if (truncValue == nullptr) {
        LOG_ERROR("Unable to execute the dynamic truncation of the HMAC-SHA1 value generated!");
        return "";
    }

    // Convert the truncated value to a 32 bit number.
    truncData = (((truncValue[0] & 0x7f) << 24) | ((truncValue[1] & 0xff) << 16) | ((truncValue[2] & 0xff) << 8) | (truncValue[3] & 0xff));

    // Free the memory from the truncation.
    if (truncValue != nullptr) {
        free(truncValue);
        truncValue = nullptr;
    }

    // Then, calculate the otp.
    otp = truncData % DIGITS_POWER[calcDigits];

    // If we need a checksum, calculate that.
    if (addChecksum) {
        otp = (otp * 10) + calcChecksum(otp, calcDigits);
    }

    // Convert the otp number to a string.
    result = std::to_string(otp);

    // Prepend 0s if needed.
    while (result.length() < digits) {
        result = "0" + result;
    }

    return result;
}

/**
 * @brief Hotp::calcChecksum - Calculate the checksum for the OTP.
 *
 * @param otp - The OTP without the checksum.
 * @param digits - The number of digits the OTP should be when done.
 *
 * @return int containing the checksum.
 */
int64_t Hotp::calcChecksum(int64_t otp, size_t digits)
{
    int doubleDigits[10] = { 0, 2, 4, 6, 8, 1, 3, 5, 7, 9 };
    bool doubleDigit = true;
    int total = 0;
    int digit;
    int result;

    while (0 < digits--) {
        digit = (otp % 10);

        otp /= 10;

        if (doubleDigit) {
            digit = doubleDigits[digit];
        }

        total += digit;
        doubleDigit = !doubleDigit;
    }

    result = total % 10;
    if (result > 0) {
        result = 10 - result;
    }

    return result;
}

/**
 * @brief Hotp::dynamicTruncate - Given an HMAC calculation, execute the dynamic truncation
 *      algorithm, and return that result.
 *
 * @param hmac - The HMAC generated to be used for the HOTP value.
 * @param hmacSize - The size of the HMAC provided.
 * @param truncateOffset - A value of 0..(hmacSize - 4) to be used for the truncationOffset.  If it is
 *      any other number, dynamic truncation will be used.
 *
 * @return unsigned char pointer to the dynamically truncated value.  The caller will take
 *      ownership of this pointer and *MUST* free it!  On error, nullptr will be returned.
 */
unsigned char *Hotp::dynamicTruncate(const unsigned char *hmac, size_t hmacSize, size_t truncateOffset)
{
    size_t offset;
    unsigned char *result;

    if (hmac == nullptr) {
        LOG_ERROR("No HMAC value provided while attempting dynamic truncation!");
        return nullptr;
    }

    if (truncateOffset < (hmacSize - 4)) {
        // Use the provided truncation value.
        offset = static_cast<size_t>(truncateOffset);
    } else {
        // Get the offset bits from the last byte of the HMAC.
        offset = (hmac[(hmacSize - 1)] & 0x0f);
    }

    // Allocate 4 bytes for our result
    result = static_cast<unsigned char *>(calloc(1, 4));
    if (result == nullptr) {
        LOG_ERROR("Unable to allocate memory for the dynamically truncated value!");
        return nullptr;
    }

    // Copy the 4 bytes at the offset.
    for (size_t i = 0; i < 4; i++) {
        result[i] = hmac[offset + i];
    }

    return result;
}

/**
 * @brief Hotp::copy - Copy the members of the \c toCopy object in to this object.
 *
 * @param toCopy - The object to copy data from.
 */
void Hotp::copy(const Hotp &toCopy)
{
    mHmacToUse = toCopy.mHmacToUse;
    mShouldDelete = toCopy.mShouldDelete;
}
