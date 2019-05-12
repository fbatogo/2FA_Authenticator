#include <QtTest>
#include <QCoreApplication>
#include <iostream>

#include "TestRunner/runtestsuites.h"

// Tests
#include "base32codertests.h"
#include "hexdecodertests.h"
#include "hmacsha1tests.h"
#include "hmacsha256tests.h"
#include "hmacsha512tests.h"
#include "hotptests.h"
#include "qrcodestringparsertests.h"
#include "secretdatabasetests.h"
#include "sha1tests.h"
#include "sha256tests.h"
#include "sha512tests.h"
#include "totptests.h"
#include "container/bytearraytests.h"
#include "generalinfosingletontests.h"
#include "settingshandlertests.h"
#include "keystorage/database/databasekeystoragetests.h"
#include "keystorage/keystoragetests.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    RunTestSuites tests;

    // Set our test app info.
    QCoreApplication::setApplicationName("Rollin' Tests");
    QCoreApplication::setApplicationVersion("1.0");

    // Set up our command line parser.
    QCommandLineParser parser;

    parser.setApplicationDescription("Unit Test application for Rollin'");
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
    tests.addTest(new Base32CoderTests());
    tests.addTest(new HexDecoderTests());
    tests.addTest(new HmacSha1Tests());
    tests.addTest(new HmacSha256Tests());
    tests.addTest(new HmacSha512Tests());
    tests.addTest(new hotpTests());
    tests.addTest(new QRCodeStringParserTests());
    tests.addTest(new SecretDatabaseTests());
    tests.addTest(new Sha1Tests());
    tests.addTest(new Sha256Tests());
    tests.addTest(new Sha512Tests());
    tests.addTest(new totpTests());
    tests.addTest(new ByteArrayTests());
    tests.addTest(new GeneralInfoSingletonTests());
    tests.addTest(new SettingsHandlerTests());
    tests.addTest(new DatabaseKeyStorageTests());
    tests.addTest(new KeyStorageTests());

    return tests.executeAll();
}
