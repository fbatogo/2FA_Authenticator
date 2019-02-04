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
    OtpEntry *result;
    int startTime;

    // Make sure the key entry provided is valid.
    if (!keydata.valid()) {
        LOG_ERROR("The key data provided to calculate the OTP from was invalid!");
        return nullptr;
    }

    // Start by converting the secret to the correct format for us to use.
    if (!decodeSecret(keydata, &dSecret, &dSize)) {
        LOG_ERROR("Unable to decode the key secret value for identifier : " + keydata.identifier());
        return nullptr;
    }

    // Calculate the OTP code.
    calculatedCode = calculateCode(keydata, dSecret, dSize);

    // Free the memory from dSecret before checking the result.
    free(dSecret);
    dSecret = nullptr;

    if (calculatedCode.isEmpty()) {
        LOG_ERROR("Unable to calculate the OTP value for identifier : " + keydata.identifier());
        return nullptr;
    }

    // Calculate the number of seconds in to the lifetime of the OTP that we are.
    startTime = getStartTime(keydata.timeStep());

    // Take the resulting OTP code and data from the KeyEntry and create the
    // OtpEntry.
    result = new OtpEntry();
    result->setIdentifier(keydata.identifier());
    result->setCurrentCode(calculatedCode);
    result->setStartTime(startTime);
    result->setTimeStep(keydata.timeStep());

    return result;
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
    // Figure out which type of OTP we need to calculate.
    switch (keydata.otpType()) {
    case KEYENTRY_OTPTYPE_TOTP:
        return calculateTotp(keydata, decodedSecret, decodedSize);
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
    otp = totp.calculate(decodedSecret, decodedSize, now, keydata.timeStep(), keydata.outNumberCount());

    // Return the calculated value.
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
