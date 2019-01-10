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
 * @brief Logger::log - Write a log line at "normal" level.
 *
 * @param logline - The log line to write.
 */
void Logger::log(const QString &logline)
{
    std::cout << logline.toStdString() << std::endl;
}

/**
 * @brief Logger::logDebug - Write a log line at debug level.
 *
 * @param logline - The log line to write.
 */
void Logger::logDebug(const QString &logline)
{
    std::cout << "*DEBUG* " << logline.toStdString() << std::endl;
}

/**
 * @brief Logger::logError - Write a log line at the error level.
 *
 * @param logline - The log line to write.
 */
void Logger::logError(const QString &logline)
{
    std::cout << "!ERROR! " << logline.toStdString() << std::endl;
}

/**
 * @brief Logger::logWarning - Write a log line at the warning level.
 *
 * @param logline - The log line to write.
 */
void Logger::logWarning(const QString &logline)
{
    std::cout << "<WARNING> " << logline.toStdString() << std::endl;
}
