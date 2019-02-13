#include "otphandler.h"

#include "logger.h"
#include "../otpimpl/base32coder.h"
#include "../otpimpl/hexdecoder.h"
#include "../otpimpl/totp.h"
#include "../otpimpl/hmac.h"
#include "../otpimpl/sha1hash.h"

#include <time.h>


OtpHandler::OtpHandler()
{

}

/**
 * @brief OtpHandler::calculateFromKeyEntry - Given a KeyEntry object, calculate the
 *      OTP that we should currently be showing, and return it as part of an OtpEntry
 *      object.
 *
 * @param keydata - A KeyEntry object that contains the information we need to calcualte
 *      an OTP and return an OtpEntry object.
 *
 * @return OtpEntry pointer on success.  On error, nullptr will be returned.
 */
OtpEntry *OtpHandler::calculateFromKeyEntry(const KeyEntry &keydata)
{
    unsigned char *dSecret;
    size_t dSize;
    QString calculatedCode;
    int startTime;

    // Make sure the key entry provided is valid.
    LOG_DEBUG("Checking if keydata is valid.");
    if (!keydata.valid()) {
        // It isn't valid, so just copy as much information as possible to our OtpEntry.
        LOG_ERROR("The key data provided to calculate the OTP from was invalid!");
        return createOtpEntry(keydata, "", -1);
    }

    // Start by converting the secret to the correct format for us to use.
    LOG_DEBUG("Decoding secret.");
    if (!decodeSecret(keydata, &dSecret, &dSize)) {
        LOG_ERROR("Unable to decode the key secret value for identifier : " + keydata.identifier());

        // Set our invalidReason, and return the otp object.
        return createOtpEntry(keydata, "", -1, "Unable to decode the key secret value!");
    }

    // Calculate the OTP code.
    LOG_DEBUG("Calculating code.");
    calculatedCode = calculateCode(keydata, dSecret, dSize);
    LOG_DEBUG("Code calculated.");

    // Free the memory from dSecret before checking the result.
    free(dSecret);
    dSecret = nullptr;

    LOG_DEBUG("Checking if code is empty.");
    if (calculatedCode.isEmpty()) {
        LOG_ERROR("Unable to calculate the OTP value for identifier : " + keydata.identifier());

        // Set our invalidReason, and return the otp object.
        return createOtpEntry(keydata, "", -1, "Unable to calculate the OTP value!");
    }

    // Calculate the number of seconds in to the lifetime of the OTP that we are.
    LOG_DEBUG("Calculating the start time.");
    startTime = getStartTime(keydata.timeStep());

    return createOtpEntry(keydata, calculatedCode, startTime);
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
        temp = calculateTotp(keydata, decodedSecret, decodedSize);
        LOG_DEBUG("After calc totp.");
        return temp;
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
    Totp totp(new Hmac(new Sha1Hash(), true), true);

    // Get the current time, so we can calculate the OTP.
    now = time(nullptr);

    // Calculate the TOTP with HMAC-SHA1
    LOG_DEBUG("Calculating totp.");
    otp = totp.calculate(decodedSecret, decodedSize, now, keydata.timeStep(), keydata.outNumberCount());

    // Return the calculated value.
    LOG_DEBUG("Done.");
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

/**
 * @brief OtpHandler::createOtpEntry - Create an OTP entry using the information from a
 *      KeyEntry object, and other values we might need to use.
 *
 * @param keydata - The KeyData object that we want to create an OTP entry for.
 * @param calculatedCode - The code that has been calculated to be used.  If the keydata
 *      entry is invalid, this parameter should be an empty string.
 * @param startTime - The time that the calculatedCode originally became valid.  If the
 *      keydata object is invalid, this should be set to -1.
 * @param invalidReason - If this value is not an empty string, this invalid reason will
 *      be used in the resulting OtpEntry object.  If it *IS* an empty string, then the
 *      object will either be valid, or contain an invalidReason copied from the KeyEntry.
 *
 * @return OtpEntry pointer containing the useful components of the parameters provided.
 */
OtpEntry *OtpHandler::createOtpEntry(const KeyEntry &keydata, const QString &calculatedCode, int startTime, const QString &invalidReason)
{
    OtpEntry *result;

    // Create our OtpEntry object.
    result = new OtpEntry();
    if (result == nullptr) {
        LOG_ERROR("Unable to allocate memory to store an OTP entry!");
        return nullptr;
    }

    // Copy over the relevant values.
    result->setIdentifier(keydata.identifier());
    result->setCurrentCode(calculatedCode);
    result->setStartTime(startTime);
    result->setTimeStep(keydata.timeStep());

    if (invalidReason.isEmpty()) {
        result->setInvalidReason(keydata.invalidReason());
        result->setValid(keydata.valid());
    } else {
        result->setInvalidReason(invalidReason);
        result->setValid(false);
    }

    return result;
}
