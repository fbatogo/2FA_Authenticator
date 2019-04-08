#include "qrcodestringparser.h"

#include <QUrl>
#include <QMutexLocker>
#include "logger.h"

QRCodeStringParser::QRCodeStringParser(QObject *parent) :
    QObject(parent)
{
    // Start by allocating our mutex.
    mMutex = new QMutex(QMutex::Recursive);

    clear();
}

/**
 * @brief QRCodeStringParser::isOtpCode - If the code provided is a valid TOTP code, then
 *      return true.
 *
 * @return true if the code parsed appears to be a valid OTP code.  false if it isn't.
 */
bool QRCodeStringParser::isOtpCode() const
{
    QMutexLocker locker(mMutex);
    Q_UNUSED(locker);

    return mIsOtpCode;
}

/**
 * @brief QRCodeStringParser::isCodeProcessing - Return a true/false value that indicates if this
 *      QR code is being processed, or not.
 *
 * @return true if the code is being processed.  false otherwise.
 */
bool QRCodeStringParser::isCodeProcessing() const
{
    QMutexLocker locker(mMutex);
    Q_UNUSED(locker);

    return mCodeProcessing;
}

/**
 * @brief QRCodeStringParser::setCodeProcessing - Set the flag that indicates if this QR code is
 *      being processed by the UI.
 *
 * @param newval - true if the UI should be processing the code.  false otherwise.
 */
void QRCodeStringParser::setCodeProcessing(bool newval)
{
    QMutexLocker locker(mMutex);
    Q_UNUSED(locker);

    mCodeProcessing = newval;
}

/**
 * @brief QRCodeStringParser::type - Return the TYPE value from the parsed URI.
 *
 * @return QString containing the type, as long as isOtpCode() returns true.
 */
QString QRCodeStringParser::type() const
{
    QMutexLocker locker(mMutex);
    Q_UNUSED(locker);

    return mType;
}

/**
 * @brief QRCodeStringParser::label - Return the label portion of the URI that was
 *      read from the QR code.
 *
 * @return QString containing the label.  (May be empty!)
 */
QString QRCodeStringParser::label() const
{
    QMutexLocker locker(mMutex);
    Q_UNUSED(locker);

    return mLabel;
}

/**
 * @brief QRCodeStringParser::parametersAsString - Return the parameters portion of
 *      the URI as a string.
 *
 * @return QString containing the parameters portion.  (May be empty!)
 */
QString QRCodeStringParser::parametersAsString() const
{
    QMutexLocker locker(mMutex);
    Q_UNUSED(locker);

    return mParameterString;
}

/**
 * @brief QRCodeStringParser::parameterByKey - Return the parameter value for the
 *      specified key.
 *
 * @param key - The key that we want to get the value for.
 *
 * @return QString containing the value.  If the key isn't in the map, an empty string is
 *      returned.
 */
QString QRCodeStringParser::parameterByKey(const QString &key)
{
    QMutexLocker locker(mMutex);
    Q_UNUSED(locker);

    return mAttributeValues.value(key);
}

/**
 * @brief QRCodeStringParser::updateEngine - Cache a pointer to the QQmlEngine that
 *      was used when this object was created by the QML engine.
 *
 * @param engine - The new engine pointer to cache.
 */
void QRCodeStringParser::updateEngine(QQmlEngine *engine)
{
    QMutexLocker locker(mMutex);
    Q_UNUSED(locker);

    mQmlEngine = engine;
}

/**
 * @brief QRCodeStringParser::getInstance - Return the singleton instance for the QR
 *      code string parser.
 *
 * @return QRCodeStringParser pointer to the singleton.
 */
QRCodeStringParser::~QRCodeStringParser()
{
    // Clean up the mutex.
    delete mMutex;
    mMutex = nullptr;
}

QRCodeStringParser *QRCodeStringParser::getInstance()
{
    static QRCodeStringParser singleton;

    return &singleton;
}

/**
 * @brief QRCodeStringParser::getQmlSingleton - Return the singleton that will be used by
 *      the QML engine.
 *
 * @param engine - A pointer to the active QML engine.
 * @param scriptEngine - A pointer to the active QML script engine.
 *
 * @return QObject pointer to the singleton.
 */
QObject *QRCodeStringParser::getQmlSingleton(QQmlEngine *engine, QJSEngine *)
{
    QRCodeStringParser *cSingleton;

    cSingleton = getInstance();

    cSingleton->updateEngine(engine);

    QQmlEngine::setObjectOwnership(cSingleton, QQmlEngine::CppOwnership);

    return static_cast<QObject *>(cSingleton);
}

/**
 * @brief QRCodeStringParser::clear - Reset all data values in this object back to their
 *      default values.
 */
void QRCodeStringParser::clear()
{
    QMutexLocker locker(mMutex);
    Q_UNUSED(locker);

    mIsOtpCode = false;
    mType.clear();
    mLabel.clear();
    mParameterString.clear();
    mAttributeValues.clear();
    mCodeProcessing = false;
    mQmlEngine = nullptr;
}

/**
 * @brief QRCodeStringParser::parseCode - Parse the string provided to see if it is a valid OTP URL.
 *      Information on the URI format was found at :
 *          https://github.com/google/google-authenticator/wiki/Key-Uri-Format
 *
 *      According to that site, the format we are looking for is :
 *          otpauth://TYPE/LABEL?PARAMETERS
 *
 *      Where valid parameters are :
 *          - secret - (Required) - Base32 encoded secret value.
 *          - issuer - (Strongly Recommended) - A string indicating the issuer of this secret value.
 *          - algorithm - SHA1 (default), SHA256, SHA512
 *          - digits - (Optional) The number of digits the OTP should have.
 *          - counter - (Required if HOTP) The initial counter value.
 *          - period - (Optional if TOTP) The period that a TOTP code will be valid for.  The default is 30 seconds.
 *
 * @param toParse - The string that was read by ZBar.
 *
 * @return true if the code was a valid OTP QR code.  false otherwise.
 */
bool QRCodeStringParser::parseCode(const QString &toParse)
{
    QMutexLocker locker(mMutex);
    Q_UNUSED(locker);

    QString stripped;
    ResultWithRemainder result;

    LOG_DEBUG("URI : " + toParse);

    // Start by making sure that the "scheme" portion is "otpauth".
    if (!toParse.startsWith("otpauth://")) {
        LOG_ERROR("The QR code read doesn't appear to be a valid OTP code!");
        mIsOtpCode = false;
        return mIsOtpCode;
    }

    // Get the URI with the scheme stripped off.
    stripped = toParse.mid(10);
    LOG_DEBUG("Scheme stripped : " + stripped);

    result = getType(stripped);

    mType = result.result;
    stripped = result.remainder;

    // If the next part starts with /, strip that off.
    if (stripped.startsWith("/")) {
        stripped = stripped.mid(1);
    }

    result = getLabel(stripped);

    mLabel = result.result;

    // Everything left should be the parameters.
    mParameterString = result.remainder;

    // Parse the parameters in to AVPs.
    parseParameters(mParameterString);

    // The secret value is required, so make sure we have it before saying this OTP code is valid.
    if (parameterByKey("secret").isEmpty()) {
        LOG_ERROR("No OTP secret was included in the QR code!");
        mIsOtpCode = false;
        return mIsOtpCode;
    }

    // If we get here, then the URI is valid.
    mIsOtpCode = true;

    // Indicate that we are waiting for the UI to process the code.
    mCodeProcessing = true;

    LOG_DEBUG("Type : " + mType);
    LOG_DEBUG("Label : " + mLabel);
    LOG_DEBUG("Parameters : " + mParameterString);

    return mIsOtpCode;
}

/**
 * @brief QRCodeStringParser::getType - Given a stripped string, pull out the type string.
 *
 * @param schemeStripped - A URI with the otpauth:// portion removed.
 *
 * @return QString containing the type that was encoded in the URI.
 */
ResultWithRemainder QRCodeStringParser::getType(const QString &schemeStripped)
{
    QStringList parts;
    ResultWithRemainder result;

    // If we split on /, the first piece should be our type.
    parts = schemeStripped.split("/");

    if (parts.size() < 1) {
        // We didn't have a type?
        result.result = "";
        result.remainder = "";
        return result;
    }

    result.result = parts.at(0);
    result.remainder = parts.at(1);

    return result;
}

/**
 * @brief QRCodeStringParser::getLabel - Get the label portion of the stripped string.
 *
 * @param typeStripped - A URI with everything up to, and including, the type (and trailing
 *      "/") stripped.
 *
 * @return QString containing the label that was encoded in the URI.
 */
ResultWithRemainder QRCodeStringParser::getLabel(const QString &typeStripped)
{
    QStringList parts;
    ResultWithRemainder result;

    // If we split on ?, the first piece should be our label.
    parts = typeStripped.split("?");

    if (parts.size() < 1) {
        // We didn't have a label?
        result.result = "";
        result.remainder = "";
        return result;
    }

    result.result = QUrl::fromPercentEncoding(parts.at(0).toLatin1());
    result.remainder = parts.at(1);

    return result;
}

/**
 * @brief QRCodeStringParser::parseParameters - Parse the parameter string
 *      that is in the QR code, and map its values for later retrival.
 *
 * @param parameters - The parameters string, following the "?" portion.
 */
void QRCodeStringParser::parseParameters(const QString &parameters)
{
    QStringList avpStrings;

    avpStrings = getAvpStringList(parameters);

    // Split the strings in to attribuets and values, and add them to the mAttributeValues map.
    for (auto avpString : avpStrings) {
        splitAndAddAvp(avpString);
    }
}

/**
 * @brief QRCodeStringParser::getAvpStringList - Given a parameter string (following the "?" portion),
 *      split it in to individual AVP strings.
 *
 * @param parameters - The parameter string to split.
 *
 * @return QStringList containing the split AVP strings.
 */
QStringList QRCodeStringParser::getAvpStringList(const QString &parameters)
{
    return parameters.split("&");
}

/**
 * @brief QRCodeStringParser::splitAndAddAvp - Split a string with a single AVP in it, in to the key and value, and
 *      stash it in our map for later reading.
 *
 * @param avpString - The AVP string that we want to split in to key and value pairs.
 */
void QRCodeStringParser::splitAndAddAvp(const QString &avpString)
{
    QStringList parts;
    QString key, value;

    parts.clear();
    key.clear();
    value.clear();

    parts = avpString.split("=");

    switch (parts.size()) {
    case 0:
        // Nothing to do.
        return;

    case 1:
        // Do nothing.  Use an empty value.
        break;

    case 2:
        // URL decode the value.
        value = QUrl::fromPercentEncoding(parts.at(1).toLatin1());
        break;

    default:
        // Rebuild a value portion with an = in it.
        for (int i = 1; i < parts.size(); i++) {
            value += parts.at(i);

            if (i < (parts.size() - 1)) {
                value += "=";
            }
        }
        value = QUrl::fromPercentEncoding(value.toLatin1());
        break;
    }

    // We should have at least 1 entry in the list.  Set that as the key.
    key = parts.at(0);

    // Add it to our AVP list.
    mAttributeValues.insert(key, value);
}
