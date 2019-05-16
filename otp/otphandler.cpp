#include "otphandler.h"

#include "logger.h"
#include "../otpimpl/base32coder.h"
#include "../otpimpl/hexdecoder.h"
#include "../otpimpl/totp.h"
#include "../otpimpl/hotp.h"
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
    ByteArray dSecret;
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
    if (!decodeSecret((*keydata), dSecret)) {
        LOG_ERROR("Unable to decode the key secret value for identifier : " + keydata->identifier());

        // Set the invalid reason, and flag the code as invalid.
        keydata->setInvalidReason("Unable to decode the key secret value!");
        keydata->setCodeValid(false);
        return;
    }

    // Calculate the OTP code.
    calculatedCode = calculateCode((*keydata), dSecret);

    dSecret.clear();

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
 * @param decodedSecret[OUT] - The secret value in its cleartext format.
 *
 * @return bool containing true if the secret was decoded.  false on error.
 */
bool OtpHandler::decodeSecret(const KeyEntry &keydata, ByteArray &decodedSecret)
{
    // Figure out what type of encoding we have, and make the correct call to
    // handle it.
    if (KEYENTRY_KEYTYPE_BASE32 == keydata.keyType()) {
        return decodeBase32Key(keydata, decodedSecret);
    } else if (KEYENTRY_KEYTYPE_HEX == keydata.keyType()) {
        return decodeHexKey(keydata, decodedSecret);
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
 * @param decodedSecret[OUT] - The decoded secret value.
 *
 * @return bool indicating if the key was decoded properly.
 */
bool OtpHandler::decodeBase32Key(const KeyEntry &keydata, ByteArray &decodedSecret)
{
    Base32Coder decode;

    decodedSecret = decode.decode(keydata.secret());

    return true;
}

/**
 * @brief OtpHandler::decodeHexKey - Decode a key that is encoded in an ASCII representation of HEX.
 *
 * @param keydata - A keyEntry object that contains the information for the secret that
 *      we want to decode.
 * @param decodedScret[OUT] - The decoded secret value.
 *
 * @return bool indicating if the key was decoded properly.
 */
bool OtpHandler::decodeHexKey(const KeyEntry &keydata, ByteArray &decodedSecret)
{
    HexDecoder decode;

    decodedSecret = decode.decode(keydata.secret());

    return true;
}

/**
 * @brief OtpHandler::calculateCode - Calculate the OTP code based on the information
 *      in the KeyEntry provided, and the decoded secret value.
 *
 * @param keydata - The KeyEntry to use to calculate the OTP.
 * @param decodedSecret[OUT] - The secret value that should be used to calculate the OTP.
 *
 * @return QString containing the calculated OTP.  On failure, an empty string will
 *      be returned.
 */
QString OtpHandler::calculateCode(const KeyEntry &keydata, const ByteArray &decodedSecret)
{
    // Figure out which type of OTP we need to calculate.
    if (KEYENTRY_OTPTYPE_TOTP == keydata.otpType()) {
        return calculateTotp(keydata, decodedSecret);
    } else if (KEYENTRY_OTPTYPE_HOTP == keydata.otpType()) {
        return calculateHotp(keydata, decodedSecret);
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
 *
 * @return QString containing the calculated TOTP.  On error, an empty
 *      string will be returned.
 */
QString OtpHandler::calculateTotp(const KeyEntry &keydata, const ByteArray &decodedSecret)
{
    time_t now;
    std::string otp;
    Totp totp;
    std::shared_ptr<Hmac> hmac;

    hmac = getHmacForKeyData(keydata);
    if (nullptr == hmac) {
        // Couldn't get a valid HMAC object.
        return "";
    }

    // Then, set the HMAC object to use with the calculation.
    totp.setHmac(hmac);

    // Get the current time, so we can calculate the OTP.
    now = time(nullptr);

    // Calculate the TOTP with HMAC-SHA1
    otp = totp.calculate(decodedSecret, now, keydata.timeStep(), keydata.outNumberCount());

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
 *
 * @return QString containing the calculated HOTP.  On error, an empty
 *      string will be returned.
 */
QString OtpHandler::calculateHotp(const KeyEntry &keydata, const ByteArray &decodedSecret)
{
    std::string otp;
    Hotp hotp;
    std::shared_ptr<Hmac> hmac;

    hmac = getHmacForKeyData(keydata);
    if (nullptr == hmac) {
        // Got an error getting the HMAC method.
        return "";
    }

    // Then, set the HMAC object to use with the calculation.
    hotp.setHmac(hmac);

    // Calculate the HOTP value.
    otp = hotp.calculate(decodedSecret, keydata.hotpCounter(), keydata.outNumberCount());

    return QString::fromStdString(otp);
}

/**
 * @brief OtpHandler::getStartTime - Get the number of seconds in to the lifetime of the OTP
 *      that have elapsed.
 *
 * @param timeStep - The 'time step' for the OTP.
 *
 * @return size_t containing the number of seconds in to the lifetime of the current OTP.
 */
size_t OtpHandler::getStartTime(size_t timeStep)
{
    time_t t;
    tm *now;
    size_t seconds;

    t = time(nullptr);
    now = localtime(&t);            //NOSONAR

    seconds = static_cast<size_t>(now->tm_sec);

    // Return the number of seconds beyond the time step that have elapsed.
    return (seconds & timeStep);
}

/**
 * @brief OtpHandler::getHmacForKeyData - Get an HMAC object for the algorithm specified in
 *      the KeyData object.
 *
 * @param keydata - The KeyData object to get the HMAC object for.
 *
 * @return std::shared_ptr<Hmac> for the hash algorithm specified in the KeyData object.
 */
std::shared_ptr<Hmac> OtpHandler::getHmacForKeyData(const KeyEntry &keydata)
{
    std::shared_ptr<HashTypeBase> hashToUse;
    std::shared_ptr<Hmac> hmac;

    // Figure out what type of hash we should be using.
    switch (keydata.algorithm()) {
    case KEYENTRY_ALG_SHA1:
        hashToUse = std::shared_ptr<HashTypeBase>(new Sha1Hash());
        break;
    case KEYENTRY_ALG_SHA256:
        hashToUse = std::shared_ptr<HashTypeBase>(new Sha256Hash());
        break;
    case KEYENTRY_ALG_SHA512:
        hashToUse = std::shared_ptr<HashTypeBase>(new Sha512Hash());
        break;
    default:
        LOG_ERROR("Unknown hash algorithm identifier of : " + QString::number(keydata.algorithm()));
        return nullptr;
    }

    hmac = std::shared_ptr<Hmac>(new Hmac());
    // Set the hash in to our HMAC object, and transfer ownership to the HMAC object.
    hmac->setHashType(hashToUse);

    return hmac;
}
