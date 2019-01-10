#ifndef LOGGER_H
#define LOGGER_H

#include <QString>

class Logger
{
public:
    static Logger *getInstance();

    void log(const QString &logline);
    void logDebug(const QString &logline);
    void logError(const QString &logline);
    void logWarning(const QString &logline);

private:
    Logger();
};


/*******
 * Some macros to make it a little easier to use the logger.
 */
#define LOG(a)              Logger::getInstance()->log(a)
#define LOG_DEBUG(a)        Logger::getInstance()->logDebug(a)
#define LOG_ERROR(a)        Logger::getInstance()->logError(a)
#define LOG_WARNING(a)      Logger::getInstance()->logWarning(a)

#define LOG_CONDITIONAL_ERROR(a, b)     if (a) { \
                                            Logger::getInstance()->logError(b); \
                                        }

#endif // LOGGER_H
