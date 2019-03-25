#include "runtestsuites.h"

#include <iostream>

#include <QtTest>

RunTestSuites::RunTestSuites()
{
    // Start with an empty log prefix.
    mLogPrefix.clear();

    // Make sure the log file type is an empty string.
    mLogFileType.clear();

    // By default, report the number of failed tests as the return code.
    mReportFailedTestsInReturnCode = true;

    // Set the default verbosity. (Which is silent.)
    mVerbosity = 0;

    // By default, log to the console in addition to any log file we may log to.
    mLogToConsole = true;
}

/**
 * @brief RunTestSuites::setLogPrefix - Set a log file prefix that we should use when creating
 *      log files.
 *
 * @param prefix - The new log prefix to use.
 */
void RunTestSuites::setLogPrefix(const QString &prefix)
{
    mLogPrefix = prefix;
    std::cout << "Will use a prefix of '" << mLogPrefix.toStdString() << "' when creating output files." << std::endl;
}

/**
 * @brief RunTestSuites::createLogFiles - Indicate that the test runner should create log files as
 *      tests are run.
 *
 * @param fileType - One of the file types that is allowed to be created.
 */
void RunTestSuites::createLogFiles(QString fileType)
{
    mLogFileType = fileType;
    std::cout << "Will use an output file type of '" << mLogFileType.toStdString() << "' when creating output files." << std::endl;
}

/**
 * @brief RunTestSuites::setLogToConsole - Set the value that indicates if test results should
 *      be written to the console, in addition to any log file.
 *
 * @param shouldLogToConsole - Set to true (the default) to log to the console in addition to any
 *      log files.  Set to false to only log to log files.
 */
void RunTestSuites::setLogToConsole(bool shouldLogToConsole)
{
    mLogToConsole = shouldLogToConsole;
}

/**
 * @brief RunTestSuites::doNotReportFailedTestsInReturnCode - Don't report the number of failed tests in the result
 *      code from the executable.   (Basically, always return 0.)
 */
void RunTestSuites::doNotReportFailedTestsInReturnCode()
{
    mReportFailedTestsInReturnCode = true;
    std::cout << "Will *NOT* report failed tests in the return code." << std::endl;
}

/**
 * @brief RunTestSuites::addTest - Add a test object to the queue of test suites to run.
 *
 * @param testObject - A QObject based object containing tests that we want to run.
 */
void RunTestSuites::addTest(QObject *testObject)
{
    mTestObjects.push_back(testObject);
}

/**
 * @brief RunTestSuites::executeAll - Execute all of the tests that are currently queued up to run.
 *
 * @return int containing the number of tests that failed.
 */
int RunTestSuites::executeAll()
{
    int failedTests = 0;
    QStringList parameters;

    // Add some space below the previous output.
    std::cout << std::endl << std::endl;

    for (const auto testObject : mTestObjects) {
        parameters = buildArgumentListForObject(testObject);

        // Run the tests in the test object.
        failedTests += QTest::qExec(testObject, parameters);

        // Add some space below the previous output.
        std::cout << std::endl << std::endl;
    }

    if (failedTests > 0) {
        std::cout << "!!!! " << failedTests << " test(s) failed!" << std::endl;
    }

    if (mReportFailedTestsInReturnCode) {
        return failedTests;
    }

    // Otherwise, report 0.
    return 0;
}

/**
 * @brief RunTestSuites::executeByPattern - Execute tests based on a string, or string pattern.
 *
 * @param pattern - The string, or string pattern to use to select tests to execute.
 *
 * @return int containing the number of tests that failed.
 */
/*
int RunTestSuites::executeByPattern(QString pattern)
{
    int failedTests = 0;

    // XXX Figure out tests to run, and run them here.

    if (mReportFailedTestsInReturnCode) {
        return failedTests;
    }

    // Otherwise, report 0.
    return 0;
}*/

/**
 * @brief RunTestSuites::buildArgumentListForObject - Build the list of arguments that we are going to
 *      pass to the QTest runner.
 *
 * @return QStringList containing the list of arguments that we should use when running.
 */
QStringList RunTestSuites::buildArgumentListForObject(QObject *toTest)
{
    QStringList result;
    std::string objectName;

    // Make sure it starts out clear.
    result.clear();

    // The QTest::qExec call wants the string list to be populated exactly like the argv passed
    // in to the app would be.  So, we need to start with an app name.  (Doesn't matter what it is.)
    result.push_back("appname");

    if (mLogToConsole) {
        // Make sure we log to the console in addition to any other logging we may do.
        result.push_back("-o");
        result.push_back("-,txt");
    }

    // See if we need to crank up the verbosity.
    if (mVerbosity > 0) {
        switch (mVerbosity) {
        case 1:
            result.push_back("-v1");
            break;

        case 2:
            result.push_back("-v2");
            break;

        case 3:
            result.push_back("-vs");
            break;
        }
    }

    // See if we should be logging results.
    if (!mLogFileType.isEmpty()) {
        // Get the test object name, so we can name the log file.
        objectName = toTest->metaObject()->className();

        // Add the parameter to indicate that we want to log to a file.
        result.push_back("-o");

        // Then, add the name of the log file, with prefix, if needed.
        if (mLogPrefix.isEmpty()) {
            result.push_back(QString::fromStdString(objectName) + "." + mLogFileType + "," + mLogFileType);
        } else {
            result.push_back(mLogPrefix + QString::fromStdString(objectName) + "." + mLogFileType + "," + mLogFileType);
        }
    }

    return result;
}

