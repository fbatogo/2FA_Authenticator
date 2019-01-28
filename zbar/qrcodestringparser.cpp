#include "qrcodestringparser.h"

#include <QUrl>
#include "logger.h"

QRCodeStringParser::QRCodeStringParser(const QString &codeRead)
{
    mIsOtpCode = false;

    parseCode(codeRead);
}

/**
 * @brief QRCodeStringParser::isOtpCode - If the code provided is a valid TOTP code, then
 *      return true.
 *
 * @return true if the code parsed appears to be a valid OTP code.  false if it isn't.
 */
bool QRCodeStringParser::isOtpCode()
{
    return mIsOtpCode;
}

/**
 * @brief QRCodeStringParser::type - Return the TYPE value from the parsed URI.
 *
 * @return QString containing the type, as long as isOtpCode() returns true.
 */
QString QRCodeStringParser::type()
{
    return mType;
}

QString QRCodeStringParser::label()
{
    return mLabel;
}

QString QRCodeStringParser::parametersAsString()
{
    return mParameterString;
}

QString QRCodeStringParser::parameterByKey(const QString &key)
{
    return "";
}

/**
 * @brief QRCodeStringParser::parseCode - Parse the string provided to see if it is a valid OTP URL.
 *      Information on the URI format was found at :
 *          https://github.com/google/google-authenticator/wiki/Key-Uri-Format
 *
 *      According to that site, the format we are looking for is :
 *          otpauth://TYPE/LABEL?PARAMETERS
 *
 * @param toParse - The string that was read by ZBar.
 */
void QRCodeStringParser::parseCode(const QString &toParse)
{
    QUrl readUrl;

    readUrl = QUrl::fromUserInput(toParse);
    if (!readUrl.isValid()) {
        LOG_ERROR("The QR code read doesn't appear to be a valid URI!");
        mIsOtpCode = false;
        return;
    }

    // Now, verify the components of the URL to make sure they are what we expect.
    LOG_DEBUG("Scheme : " + readUrl.scheme());
    LOG_DEBUG("Host : " + readUrl.host());
    LOG_DEBUG("Path : " + readUrl.path());
    LOG_DEBUG("Query : " + readUrl.query());

    // The scheme should already be lower case, but convert it just to be sure.
    if (readUrl.scheme().toLower() != "otpauth") {
        LOG_DEBUG("The URI provided isn't an OTP type!");
        mIsOtpCode = false;
        return;
    }

    // If we get here, then the code should be valid.
    mIsOtpCode = true;

    mType = readUrl.host();

    // The path will start with /, so we need to strip that off.
    mLabel = readUrl.path().remove(0, 1);
    mParameterString = readUrl.query();

    // XXX Parse the parameter string to its component pieces.
}
