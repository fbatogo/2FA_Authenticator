#include "otphandler.h"

#include "logger.h"
#include "../otpimpl/base32coder.h"
#include "../otpimpl/hexdecoder.h"
#include "../otpimpl/totp.h"
#include "../otpimpl/hotp.h"
#include "../otpimpl/hmac.h"
#include "../otpimpl/sha1hash.h"
#include "../otpimpl/sha256hash.h"
#include "../otpimpl/sha512hash.h"

#include <time.h>


OtpHandler::OtpHandler()
{

}

/**
 * @brief OtpHandler::calculateOtpForKeyEntry - Calculate the OTP for the provided key entry.
 *
 * @param keydata - A pointer to a KeyEntry object that we want to calculate the OTP for.
 */
void OtpHandler::calculateOtpForKeyEntry(KeyEntry *keydata)
{
    unsigned char *dSecret;
    size_t dSize;
    QString calculatedCode;

    if (keydata == nullptr) {
        LOG_ERROR("No key data provided while attempting to calculate an OTP!");

        // Nothing we can do.
        return;
    }

    // Make sure the key entry provided is valid.
    if (!keydata->valid()) {
        // It isn't valid, so just copy as much information as possible to our OtpEntry.
        LOG_ERROR("The key data provided to calculate the OTP from was invalid!");

        // Set the invalid reason, and flag the code as invalid.
        keydata->setInvalidReason("The key data provided to calculate the OTP from was invalid!");
        keydata->setCodeValid(false);
        return;
    }

    // Start by converting the secret to the correct format for us to use.
    if (!decodeSecret((*keydata), &dSecret, &dSize)) {
        LOG_ERROR("Unable to decode the key secret value for identifier : " + keydata->identifier());

        // Set the invalid reason, and flag the code as invalid.
        keydata->setInvalidReason("Unable to decode the key secret value!");
        keydata->setCodeValid(false);
        return;
    }

    // Calculate the OTP code.
    calculatedCode = calculateCode((*keydata), dSecret, dSize);

    // Free the memory from dSecret before checking the result.
    free(dSecret);
    dSecret = nullptr;

    if (calculatedCode.isEmpty()) {
        LOG_ERROR("Unable to calculate the OTP value for identifier : " + keydata->identifier());

        // Set the invalid reason, and flag the code as invalid.
        keydata->setInvalidReason("Unable to calculate the OTP value for identifier : " + keydata->identifier());
        keydata->setCodeValid(false);
        return;
    }

    // Calculate the number of seconds in to the lifetime of the OTP that we are.
    keydata->setStartTime(getStartTime(keydata->timeStep()));
    LOG_DEBUG("New start time is : " + QString::number(keydata->startTime()));

    // The code should be valid.
    keydata->setCodeValid(true);
    keydata->setCurrentCode(calculatedCode);
}

/**
 * @brief OtpHandler::decodeSecret - Decode the secret value in to the format that
 *      liboath wants.
 *
 * @param keydata - A KeyEntry object that contains the secret value we want to
 *      decode.
 *
 * @return QString containing the decoded key value.  On failure, an empty string
 *      will be returned.
 */
bool OtpHandler::decodeSecret(const KeyEntry &keydata, unsigned char **decodedSecret, size_t *decodedSize)
{
    // Figure out what type of encoding we have, and make the correct call to
    // handle it.
    switch (keydata.keyType()) {
    case KEYENTRY_KEYTYPE_BASE32:
        return decodeBase32Key(keydata, decodedSecret, decodedSize);

    case KEYENTRY_KEYTYPE_HEX:
        return decodeHexKey(keydata, decodedSecret, decodedSize);
    }

    // If we get here, then we don't know how to decode the key type.
    LOG_ERROR("Unknown key encoding of '" + QString::number(keydata.keyType()) + "' for identifier : " + keydata.identifier());
    return false;
}

/**
 * @brief OtpHandler::decodeBase32Key - Use liboath to decode the base32 key data in
 *      to the format liboath wants to use.
 *
 * @param keydata - A KeyEntry object that contains the information for the secret that
 *      we want to decode.
 * @param[OUT] decodedSecret - The decoded secret value.
 * @param[OUT] decodedSize - The size of the decoded secret value.
 *
 * @return bool indicating if the key was decoded properly.
 */
bool OtpHandler::decodeBase32Key(const KeyEntry &keydata, unsigned char **decodedSecret, size_t *decodedSize)
{
    Base32Coder decode;

    (*decodedSecret) = decode.decode(keydata.secret().toStdString(), (*decodedSize));

    return true;
}

/**
 * @brief OtpHandler::decodeHexKey - Decode a key that is encoded in an ASCII representation of HEX.
 *
 * @param keydata - A keyEntry object that contains the information for the secret that
 *      we want to decode.
 * @param decodedScret - The decoded secret value.
 * @param deciodedSize - The size of the decoded secret value.
 *
 * @return bool indicating if the key was decoded properly.
 */
bool OtpHandler::decodeHexKey(const KeyEntry &keydata, unsigned char **decodedSecret, size_t *decodedSize)
{
    HexDecoder decode;

    (*decodedSecret) = decode.decode(keydata.secret().toStdString(), (*decodedSize));

    return true;
}

/**
 * @brief OtpHandler::calculateCode - Calculate the OTP code based on the information
 *      in the KeyEntry provided, and the decoded secret value.
 *
 * @param keydata - The KeyEntry to use to calculate the OTP.
 * @param decodedSecret - The secret value that should be used to calculate the OTP.
 * @param decodedSize - The size of the data pointed to by decodedSecret.
 *
 * @return QString containing the calculated OTP.  On failure, an empty string will
 *      be returned.
 */
QString OtpHandler::calculateCode(const KeyEntry &keydata, const unsigned char *decodedSecret, size_t decodedSize)
{
    QString temp;

    // Figure out which type of OTP we need to calculate.
    switch (keydata.otpType()) {
    case KEYENTRY_OTPTYPE_TOTP:
        return calculateTotp(keydata, decodedSecret, decodedSize);

    case KEYENTRY_OTPTYPE_HOTP:
        return calculateHotp(keydata, decodedSecret, decodedSize);
    }

    // If we get here, then we don't know the OTP type to generate.
    LOG_ERROR("Unknown OTP type of '" + QString::number(keydata.otpType()) + "' for identifier : " + keydata.identifier());
    return "";
}

/**
 * @brief OtpHandler::calculateTotp - Calculate a TOTP value.
 *
 * @param keydata - A KeyEntry that contains most of the information we need
 *      to calculate a TOTP value.
 * @param decodedSecret - The decoded secret value to use to calculate the
 *      TOTP value.
 * @param decodedSize - The size of the data pointed to by decodedSecret.
 *
 * @return QString containing the calculated TOTP.  On error, an empty
 *      string will be returned.
 */
QString OtpHandler::calculateTotp(const KeyEntry &keydata, const unsigned char *decodedSecret, size_t decodedSize)
{
    time_t now;
    std::string otp;
    Totp totp;
    Hmac hmac;
    HashTypeBase *hashToUse;

    // Figure out what type of hash we should be using.
    switch (keydata.algorithm()) {
    case KEYENTRY_ALG_SHA1:
        hashToUse = new Sha1Hash();
        break;

    case KEYENTRY_ALG_SHA256:
        hashToUse = new Sha256Hash();
        break;

    case KEYENTRY_ALG_SHA512:
        hashToUse = new Sha512Hash();
        break;

    default:
        LOG_ERROR("Unknown hash algorithm identifier of : " + QString::number(keydata.algorithm()));
        return "";
    }

    // Set the hash in to our HMAC object, and transfer ownership to the HMAC object.
    hmac.setHashType(hashToUse);

    // Then, set the HMAC object to use with the calculation, but keep ownership here.
    totp.setHmac(&hmac, false);

    // Get the current time, so we can calculate the OTP.
    now = time(nullptr);

    // Calculate the TOTP with HMAC-SHA1
    otp = totp.calculate(decodedSecret, decodedSize, now, keydata.timeStep(), keydata.outNumberCount());

    // Return the calculated value.
    return QString::fromStdString(otp);
}

/**
 * @brief OtpHandler::calculateHotp - Calculate an HOTP value.
 *
 * @param keydata - A KeyEntry that contains most of the information we need
 *      to calculate an HOTP value.
 * @param decodedSecret - The decoded secret value to use to calculate the
 *      HOTP value.
 * @param decodedSize - The size of the data pointed to by decodedSecret.
 *
 * @return QString containing the calculated HOTP.  On error, an empty
 *      string will be returned.
 */
QString OtpHandler::calculateHotp(const KeyEntry &keydata, const unsigned char *decodedSecret, size_t decodedSize)
{
    std::string otp;
    Hotp hotp;
    Hmac hmac;
    HashTypeBase *hashToUse;

    // Figure out what type of hash we should be using.
    switch (keydata.algorithm()) {
    case KEYENTRY_ALG_SHA1:
        hashToUse = new Sha1Hash();
        break;

    case KEYENTRY_ALG_SHA256:
        hashToUse = new Sha256Hash();
        break;

    case KEYENTRY_ALG_SHA512:
        hashToUse = new Sha512Hash();
        break;

    default:
        LOG_ERROR("Unknown hash algorithm identifier of : " + QString::number(keydata.algorithm()));
        return "";
    }

    // Set the hash in to our HMAC object, and transfer ownership to the HMAC object.
    hmac.setHashType(hashToUse, true);

    // Then, set the HMAC object to use with the calculation, but keep ownership here.
    hotp.setHmac(&hmac, false);


    otp = hotp.calculate(decodedSecret, decodedSize, keydata.hotpCounter(), keydata.outNumberCount());

    return QString::fromStdString(otp);
}

/**
 * @brief OtpHandler::getStartTime - Get the number of seconds in to the lifetime of the OTP
 *      that have elapsed.
 *
 * @param timeStep - The 'time step' for the OTP.
 *
 * @return int containing the number of seconds in to the lifetime of the current OTP.
 */
int OtpHandler::getStartTime(int timeStep)
{
    time_t t;
    tm *now;
    int seconds;

    t = time(nullptr);
    now = localtime(&t);

    seconds = now->tm_sec;

    // Return the number of seconds beyond the time step that have elapsed.
    return (seconds & timeStep);
}
