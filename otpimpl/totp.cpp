#include "totp.h"

#include "hotp.h"
#include "logger.h"

Totp::Totp()
{
    mHmacToUse = nullptr;
}

Totp::Totp(Totp &toCopy)
{
    copy(toCopy);
}

Totp::Totp(std::shared_ptr<Hmac> hmacToUse)
{
    mHmacToUse = hmacToUse;
}

Totp::~Totp()
{
    clear();
}

/**
 * @brief Totp::setHmac - Set the HMAC object that should be used to calculate the TOTP value.
 *
 * @param hmacToUse - A pointer to the HMAC object that should be used when calculating a
 *      TOTP value.
 */
void Totp::setHmac(std::shared_ptr<Hmac> &hmacToUse)
{
    // If we are currently set to delete the HMAC object, and we have an HMAC object set,
    // clean it up.
    clear();

    // Set the new values to use.
    mHmacToUse = hmacToUse;
}

/**
 * @brief Totp::calculate - Calculate a TOTP value.
 *
 * @param decodedSecret - The key to use when calculating the TOTP value.
 * @param utcTime - The current time in the UTC time zone.
 * @param timeStep - The length of time that the TOTP should be valid for.  (Should usually be
 *      left at the default of 30.)
 * @param digits - The number of digits to return for the TOTP.
 * @param initialCounter - An offset to apply to the UTC time when calculating the TOTP value.
 *
 * @return std::string containing the OTP value.  On error, an empty string is returned.
 */
std::string Totp::calculate(const ByteArray &decodedSecret, time_t utcTime, size_t timeStep, size_t digits, uint64_t initialCounter)
{
    Hotp hotp(mHmacToUse);
    uint64_t calcTime;

    // Make sure we are configured properly to calculate the TOTP.
    if (mHmacToUse == nullptr) {
        LOG_ERROR("Unable to calculate the TOTP!  No HMAC object was set!");
        return "";
    }

    calcTime = (static_cast<uint64_t>(utcTime) - initialCounter)/timeStep;

    // Then, calculate the HOTP using the key, and the calcTime.
    return hotp.calculate(decodedSecret, calcTime, digits);
}

Totp &Totp::operator=(const Totp &toCopy)
{
    if (this == &toCopy) {
        return (*this);
    }

    copy(toCopy);

    return (*this);
}

/**
 * @brief Totp::clear - Clear out the values stored in this object, deleting any objects
 *      that we are configured to delete.
 */
void Totp::clear()
{
    mHmacToUse = nullptr;
}

/**
 * @brief Totp::copy - Copy the members of the \c toCopy object in to this object.
 *
 * @param toCopy - The object to copy members from.
 */
void Totp::copy(const Totp &toCopy)
{
    mHmacToUse = toCopy.mHmacToUse;
}
