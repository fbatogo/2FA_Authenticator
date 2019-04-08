#include "generalinfosingleton.h"

#include "logger.h"
#include "otpimpl/hexdecoder.h"
#include "otpimpl/base32coder.h"

GeneralInfoSingleton::GeneralInfoSingleton() :
    QObject(nullptr)
{
}

/**
 * @brief GeneralInfoSingleton::getInstance - Get the singleton instance for this class.
 *
 * @return GeneralInfoSingleton pointer.
 */
GeneralInfoSingleton::~GeneralInfoSingleton()
{
}

GeneralInfoSingleton *GeneralInfoSingleton::getInstance()
{
    static GeneralInfoSingleton singleton;

    return &singleton;
}

/**
 * @brief GeneralInfoSingleton::getQmlSingleton - Return the singleton that will be used by
 *      the QML engine.
 *
 * @param engine - A pointer to the active QML engine.
 * @param scriptEngine - A pointer to the active QML script engine.
 *
 * @return QObject pointer to the singleton.
 */
QObject *GeneralInfoSingleton::getQmlSingleton(QQmlEngine *engine, QJSEngine *)
{
    GeneralInfoSingleton *cSingleton;

    cSingleton = getInstance();

    cSingleton->updateEngine(engine);

    QQmlEngine::setObjectOwnership(cSingleton, QQmlEngine::CppOwnership);

    return static_cast<QObject *>(cSingleton);
}

/**
 * @brief GeneralInfoSingleton::updateEngine - Save a pointer to the QML engine that was
 *      passed to us when we were inited.
 *
 * @param engine - A pointer to the engine provided by the QML engine.
 */
void GeneralInfoSingleton::updateEngine(QQmlEngine *engine)
{
    mQmlEngine = engine;
}

/**
 * @brief GeneralInfoSingleton::version - Return the version string for this version of
 *      the app.
 *
 * @return QString containing the version information for this app.
 */
QString GeneralInfoSingleton::version()
{
    return APP_VERSION;
}

/**
 * @brief GeneralInfoSingleton::licenseText - Read the license text file from the resources
 *      area and return it as a QString.
 *
 * @return QString containing the license text.  On error, an empty string is returned.
 */
QString GeneralInfoSingleton::licenseText()
{
    QFile licenseFile(":/resources/licenses.txt");
    QString result;

    if (!licenseFile.open(QIODevice::ReadOnly)) {
        LOG_ERROR("Failed to open the licenses.txt file!");
        return "";
    }

    // Read the data in to a string.
    QTextStream in(&licenseFile);
    result = in.readAll();

    // Close the file.
    licenseFile.close();

    return result;
}

/**
 * @brief GeneralInfoSingleton::isEncodedProperly - Make sure the secret is encoded
 *      properly.
 *
 * @param encodingType - One of the KEYENTRY_KEYTYPE_* values.
 * @param valueToCheck - The string that we want to check the encoding on.
 *
 * @return true if the string appears to be encoded properly.  false if it isn't.
 */
bool GeneralInfoSingleton::isEncodedProperly(int encodingType, const QString &valueToCheck)
{
    switch (encodingType) {
    case KEYENTRY_KEYTYPE_HEX:
        return HexDecoder::isHexEncoded(valueToCheck.toStdString());

    case KEYENTRY_KEYTYPE_BASE32:
        return Base32Coder::isBase32Encoded(valueToCheck.toStdString());
    }

    // If we get here, we don't understand the encodingType, so return false.
    LOG_ERROR("Unknown secret encoding type of '" + QString::number(encodingType) + "'!");
    return false;
}

/**
 * @brief GeneralInfoSingleton::haveZBar - Indicate to the UI if we were built with
 *  ZBar, or not.
 *
 * @return true if we were built with ZBar, false if not.
 */
bool GeneralInfoSingleton::haveZBar()
{
#ifndef NO_ZBAR
    return true;
#else
    return false;
#endif // NO_ZBAR
}

