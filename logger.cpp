#include "logger.h"

#include <iostream>
#include <settingshandler.h>
#include <QDebug>
#include <QDateTime>
#include "generalinfosingleton.h"
#include "settingshandler.h"
#include <QMutexLocker>

Logger::Logger()
{
    mLogFilename.clear();       // Make sure it starts out empty.
}

/**
 * @brief Logger::getInstance - Get a pointer to the logger singleton instance.
 *
 * @return Logger pointer.
 */
Logger::~Logger()
{
    closeLogFile();
}

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
 * @brief Logger::setLogFile - Set the path and file name for the log file that
 *      we will write to.  If a log file is already open, it will be closed and
 *      a file with the new name and path will be created.
 *
 * @param logFilePathAndName - The path and file name for the log file that we
 *      want to write to.
 *
 * @note If calling this function, there is no need to call setLogToFile(true), as
 *      this function will do that.
 */
void Logger::setLogFile(const QString &logFilePathAndName)
{
    mLogFilename = logFilePathAndName;

    // We want to write to a log file, or else this function wouldn't have been called.
    setLogToFile(true);
}

/**
 * @brief Logger::setLogToFile - Enable or disable logging to a file.
 *
 * @param shouldLogToFile - true if we should write our log data to a log file.
 *      false if we should *NOT* write our log data to a log file.
 */
void Logger::setLogToFile(bool shouldLogToFile)
{
    if (shouldLogToFile) {
        openLogFile();
        return;
    }

    // Otherwise, close the file.
    closeLogFile();
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
    std::cout << "[" << QDateTime::currentDateTime().toSecsSinceEpoch() << "] " << logline.toStdString() << std::endl;
#endif // USE_QDEBUG

    // Write to our file, if needed.
    logToFile(logline);
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
    std::cout << "*DEBUG* " << "[" << QDateTime::currentDateTime().toSecsSinceEpoch() << "] " << logline.toStdString() << std::endl;
#endif // USE_QDEBUG

    // Write to our file, if needed.
    logToFile("[DEBUG] - " + logline);
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
    std::cout << "!ERROR! " << "[" << QDateTime::currentDateTime().toSecsSinceEpoch() << "] " << logline.toStdString() << std::endl;
#endif // USE_QDEBUG

    // Write to our file, if needed.
    logToFile("!!!ERROR!!! - " + logline);
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
    std::cout << "<WARNING> " << "[" << QDateTime::currentDateTime().toSecsSinceEpoch() << "] " << logline.toStdString() << std::endl;
#endif // USE_QDEBUG

    // Write to our file, if needed.
    logToFile("<WARNING> - " + logline);
}

/**
 * @brief Logger::openLogFile - Create or open the log file that we want
 *      to write to.
 *
 * @return true if the file was already open, or was opened (or created).
 *      false if the file wasn't open, and couldn't be opened.
 */
void Logger::openLogFile()
{
    if (mLogFile.isOpen()) {
        // Nothing to do.
        return;
    }

    // If we don't have a log file defined, set the default path using the
    // dataPath() from the SettingsHandler.
    if (mLogFilename.isEmpty()) {
        mLogFilename = SettingsHandler::getInstance()->dataPath() + "Rollin.log";
    }

    // Try to open the file.
    mLogFile.setFileName(mLogFilename);

    if (!mLogFile.open((QIODevice::WriteOnly | QIODevice::Append))) {
        qCritical("Unable to open/create a log file at '%s', error : %s", (SettingsHandler::getInstance()->dataPath() + "Rollin.log").toStdString().c_str(), mLogFile.errorString().toStdString().c_str());
        return;
    }

    // Bind the file to the text stream.
    mLogStream.setDevice(&mLogFile);

    // Write a header to the log to indicate when this run was.
    mLogStream << "---------------- Rollin' " << APP_VERSION << " --  File logging started on " << QDateTime::currentDateTime().toString("M/d/yyyy") << " at " << QDateTime::currentDateTime().toString("hh:mm:ss") << "\n";

    // Flush the buffer to disk.
    mLogStream.flush();

    // Return success.
    return;
}

/**
 * @brief Logger::closeLogFile - If our log file is open, close it.
 */
void Logger::closeLogFile()
{
    if (mLogFile.isOpen()) {
        qDebug("Closing the log file!");
        // Write a log footer, and make sure it is flushed.
        mLogStream << "---------------- Log file complete...\n\n";
        mLogStream.flush();

        // Close the file.
        mLogFile.close();
    }
}

/**
 * @brief Logger::logToFile - Write a log line to the log file, followed by
 *      flushing the log line, to be safe.
 *
 * @param logline - The log line to write to the log file, *IF* we have a
 *      log file open for use.
 */
void Logger::logToFile(const QString &logline)
{
    QMutexLocker locker(&mMutex);

    if (mLogFile.isOpen()) {
        // Write the log line to the log file, prefixed with a timestamp.
        mLogStream << "[" << QDateTime::currentDateTime().toSecsSinceEpoch() << "] - " << logline << "\n";
        mLogStream.flush();
    }
}
