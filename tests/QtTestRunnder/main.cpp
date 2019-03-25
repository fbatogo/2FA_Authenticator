#include <QtTest>
#include <QCoreApplication>
#include "TestRunner/runtestsuites.h"
#include "testhelpers/simpletestsuite.h"

#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    RunTestSuites tests;

    // Set our test app info.
    QCoreApplication::setApplicationName("QtTestRunner Tests");
    QCoreApplication::setApplicationVersion("1.0");

    // Set up our command line parser.
    QCommandLineParser parser;

    parser.setApplicationDescription("Test application for QtTestRunner");
    parser.addHelpOption();
    parser.addVersionOption();

    // Add the supported options.
    parser.addOptions({
                          // The log prefix option
                          {{"p", "prefix"}, "Set prefix to use for test output files.", "prefix"},
                          {{"x", "xml"}, "Use XML for the output files."},
                          {{"n", "no-report"}, "Do not report failed tests in the exit code."},
                      });

    // Process the command line options.
    parser.process(app);

    if (parser.isSet("prefix")) {
        // Set up the log prefix to use.
        tests.setLogPrefix(parser.value("prefix"));
    }

    if (parser.isSet("xml")) {
        // Set up the test runner to output an XML file.
        tests.createLogFiles("xml");
    }

    if (parser.isSet("no-report")) {
        // Tell the test runner not to report failed tests in the exit code.
        tests.doNotReportFailedTestsInReturnCode();
    }

    // Add our test objects to the queue.
    tests.addTest(new SimpleTestSuite());

    return tests.executeAll();
}
