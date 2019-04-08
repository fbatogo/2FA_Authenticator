#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QString>
#include <QQmlEngine>
#include <QJSEngine>
#include <QFile>
#include <QTextStream>
#include <QMutex>

class Logger : public QObject
{
    Q_OBJECT

public:
    ~Logger();

    static Logger *getInstance();
    static QObject *getQmlSingleton(QQmlEngine *engine, QJSEngine *scriptEngine);

    Q_INVOKABLE void setLogToFile(bool shouldLogToFile);

    Q_INVOKABLE void log(const QString &logline);
    Q_INVOKABLE void logDebug(const QString &logline);
    Q_INVOKABLE void logError(const QString &logline);
    Q_INVOKABLE void logWarning(const QString &logline);

private:
    void openLogFile();
    void closeLogFile();
    void logToFile(const QString &logline);

    Logger();

    QFile mLogFile;
    QTextStream mLogStream;
    QMutex mMutex;
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
