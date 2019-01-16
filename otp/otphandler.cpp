#include "otphandler.h"

#include "logger.h"

#ifndef _WIN32
extern "C" {
#include <oath.h>
}
#endif

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
    QString dSecret;
    QString calculatedCode;
    OtpEntry *result;

    // Make sure the key entry provided is valid.
    if (!keydata.valid()) {
        LOG_ERROR("The key data provided to calculate the OTP from was invalid!");
        return nullptr;
    }

    // Start by converting the secret to the correct format for us to use.
    dSecret = decodeSecret(keydata);
    if (dSecret.isEmpty()) {
        LOG_ERROR("Unable to decode the key secret value for identifier : " + keydata.identifier());
        return nullptr;
    }

    // Calculate the OTP code.
    calculatedCode = calculateCode(keydata, dSecret);
    if (calculatedCode.isEmpty()) {
        LOG_ERROR("Unable to calculate the OTP value for identifier : " + keydata.identifier());
        return nullptr;
    }

    // XXX Need to calculate the remaining time this code is valid.

    // Take the resulting OTP code and data from the KeyEntry and create the
    // OtpEntry.
    result = new OtpEntry();
    result->setIdentifier(keydata.identifier());
    result->setCurrentCode(calculatedCode);
    result->setTimeRemaining(30);            // Update with a better calculation!

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
QString OtpHandler::decodeSecret(const KeyEntry &keydata)
{
    // Figure out what type of encoding we have, and make the correct call to
    // handle it.
    switch (keydata.keyType()) {
    case KEYENTRY_KEYTYPE_HEX:
        return decodeHexKey(keydata);

    case KEYENTRY_KEYTYPE_BASE32:
        return decodeBase32Key(keydata);
    }

    // If we get here, then we don't know how to decode the key type.
    LOG_ERROR("Unknown key encoding of '" + QString::number(keydata.keyType()) + "' for identifier : " + keydata.identifier());
    return "";
}

/**
 * @brief OtpHandler::decodeHexKey - Use liboath to decode the hex key data in to the
 *      format liboath wants to use.
 *
 * @param keydata - A KeyEntry object that contains the information for the secret that
 *      we want to decode.
 *
 * @return QString containing the decoded hex key.  On error, an empty string will be
 *      returned.
 */
QString OtpHandler::decodeHexKey(const KeyEntry &keydata)
{
    return "";
}

/**
 * @brief OtpHandler::decodeBase32Key - Use liboath to decode the base32 key data in
 *      to the format liboath wants to use.
 *
 * @param keydata - A KeyEntry object that contains the information for the secret that
 *      we want to decode.
 *
 * @return QString containing the decoded base32 key.  On error, an empty string will
 *      be returned.
 */
QString OtpHandler::decodeBase32Key(const KeyEntry &keydata)
{
#ifdef _WIN32
    return "";
#else
    int rc;
    char *result;
    size_t resultSize;
    const char *error;

    rc = oath_base32_decode(keydata.secret().toStdString().c_str(), keydata.secret().length(), &result, &resultSize);
    if (rc != OATH_OK) {
        error = oath_strerror(rc);
        LOG_ERROR("Unable to decode a base32 secret value!  Error : " + QString::fromLocal8Bit(error, strlen(error)));
        return "";
    }

    // Otherwise, stuff the result in our QString and return it.
    return QString::fromLocal8Bit(result, resultSize);
#endif
}

/**
 * @brief OtpHandler::calculateCode - Calculate the OTP code based on the information
 *      in the KeyEntry provided, and the decoded secret value.
 *
 * @param keydata - The KeyEntry to use to calculate the OTP.
 * @param decodedSecret - The secret value that should be used to calculate the OTP.
 *
 * @return QString containing the calculated OTP.  On failure, an empty string will
 *      be returned.
 */
QString OtpHandler::calculateCode(const KeyEntry &keydata, QString decodedSecret)
{
    // Figure out which type of OTP we need to calculate.
    switch (keydata.otpType()) {
    case KEYENTRY_OTPTYPE_HOTP:
        return calculateHotp(keydata, decodedSecret);

    case KEYENTRY_OTPTYPE_TOTP:
        return calculateTotp(keydata, decodedSecret);
    }

    // If we get here, then we don't know the OTP type to generate.
    LOG_ERROR("Unknown OTP type of '" + QString::number(keydata.otpType()) + "' for identifier : " + keydata.identifier());
    return "";
}

/**
 * @brief OtpHandler::calculateHotp - Calculate an HOTP value.
 *
 * @param keydata - A KeyEntry that contains most of the information we need to
 *      calculate an HOTP value.
 * @param decodedSecret - The decoded secret value to use to calculate the HOTP
 *      value.
 *
 * @return QString containing the calculated HOTP.  On error, an empty string
 *      will be returned.
 */
QString OtpHandler::calculateHotp(const KeyEntry &keydata, QString decodedSecret)
{
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
QString OtpHandler::calculateTotp(const KeyEntry &keydata, QString decodedSecret)
{
#ifdef _WIN32
    return "";
#else
    time_t now;
    int rc;
    char otp[10];
    const char *error;

    // Get the current time, so we can calculate the OTP.
    now = time(nullptr);

    rc = oath_totp_generate(decodedSecret.toStdString().c_str(), decodedSecret.length(), now, OATH_TOTP_DEFAULT_TIME_STEP_SIZE, 0, keydata.outNumberCount(), (char *)&otp);
    if (rc != OATH_OK) {
        error = oath_strerror(rc);
        LOG_ERROR("Unable to calculate the TOTP value!  Error : " + QString::fromLocal8Bit(error, strlen(error)));
        return "";
    }

    // Return the calculated value.
    return QString::fromLocal8Bit(otp);
#endif
}