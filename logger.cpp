#include "logger.h"

#include <iostream>

Logger::Logger()
{

}

/**
 * @brief Logger::getInstance - Get a pointer to the logger singleton instance.
 *
 * @return Logger pointer.
 */
Logger *Logger::getInstance()
{
    static Logger singletonInstance;

    return &singletonInstance;
}

/**
 * @brief Logger::getQmlSingleton - Return the singleton that should be used with
 *      the QML code.
 *
 * @param engine - A pointer to the QML engine that will be used for the QML layouts.
 * @param scriptEngine - A pointer to the QML script engine that will be used.
 *
 * @return QObject pointer to the singleton to use with the QML code.
 */
QObject *Logger::getQmlSingleton(QQmlEngine *, QJSEngine *)
{
    Logger *cSingleton;

    cSingleton = getInstance();

    QQmlEngine::setObjectOwnership(cSingleton, QQmlEngine::CppOwnership);

    return static_cast<QObject *>(cSingleton);
}

/**
 * @brief Logger::log - Write a log line at "normal" level.
 *
 * @param logline - The log line to write.
 */
void Logger::log(const QString &logline)
{
#ifdef USE_QDEBUG
    qInfo("%s", logline.toStdString().c_str());
#else
    std::cout << logline.toStdString() << std::endl;
#endif // USE_QDEBUG
}

/**
 * @brief Logger::logDebug - Write a log line at debug level.
 *
 * @param logline - The log line to write.
 */
void Logger::logDebug(const QString &logline)
{
#ifdef USE_QDEBUG
    qDebug("%s", logline.toStdString().c_str());
#else
    std::cout << "*DEBUG* " << logline.toStdString() << std::endl;
#endif // USE_QDEBUG
}

/**
 * @brief Logger::logError - Write a log line at the error level.
 *
 * @param logline - The log line to write.
 */
void Logger::logError(const QString &logline)
{
#ifdef USE_QDEBUG
    qCritical("%s", logline.toStdString().c_str());
#else
    std::cout << "!ERROR! " << logline.toStdString() << std::endl;
#endif // USE_QDEBUG
}

/**
 * @brief Logger::logWarning - Write a log line at the warning level.
 *
 * @param logline - The log line to write.
 */
void Logger::logWarning(const QString &logline)
{
#ifdef USE_QDEBUG
    qWarning("%s", logline.toStdString().c_str());
#else
    std::cout << "<WARNING> " << logline.toStdString() << std::endl;
#endif // USE_QDEBUG
}
