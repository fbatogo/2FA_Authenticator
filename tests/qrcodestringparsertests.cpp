#include "qrcodestringparsertests.h"

#include <QTest>

void QRCodeStringParserTests::qrCodeSimpleStringParserTests()
{
    QRCodeStringParser *test = QRCodeStringParser::getInstance();

    QVERIFY(test->parseCode("otpauth://TYPE/LABEL?PARAMETERS"));
    QVERIFY(test->isOtpCode());

    QCOMPARE(test->type(), "TYPE");
    QCOMPARE(test->label(), "LABEL");
    QCOMPARE(test->parametersAsString(), "PARAMETERS");

    QCOMPARE(test->parameterByKey("PARAMETERS"), "");
}

void QRCodeStringParserTests::qrCodeInvalidStringParserTests()
{
    QRCodeStringParser *test = QRCodeStringParser::getInstance();

    QVERIFY(test->parseCode("nototpauth://TYPE/LABEL?PARAMETERS"));
    QCOMPARE(test->isOtpCode(), false);

    // XXX Add more tests.
}

void QRCodeStringParserTests::qrCodeFullSetStringParametersTests()
{
    QRCodeStringParser *test = QRCodeStringParser::getInstance();

    // Parse a URI with all possible parameters.
    QVERIFY(test->parseCode("otpauth://totp/ACME%20Co:john.doe@email.com?secret=HXDMVJECJJWSRB3HWIZR4IFUGFTMXBOZ&issuer=ACME%20Co&algorithm=SHA1&digits=6&period=30"));
    QVERIFY(test->isOtpCode());

    QCOMPARE(test->type(), "totp");
    QCOMPARE(test->label(), "ACME Co:john.doe@email.com");
    QCOMPARE(test->parametersAsString(), "secret=HXDMVJECJJWSRB3HWIZR4IFUGFTMXBOZ&issuer=ACME%20Co&algorithm=SHA1&digits=6&period=30");

    // Check each of the avp values.
    QCOMPARE(test->parameterByKey("secret"), "HXDMVJECJJWSRB3HWIZR4IFUGFTMXBOZ");
    QCOMPARE(test->parameterByKey("issuer"), "ACME Co");
    QCOMPARE(test->parameterByKey("algorithm"), "SHA1");
    QCOMPARE(test->parameterByKey("digits"), "6");
    QCOMPARE(test->parameterByKey("period"), "30");
}

void QRCodeStringParserTests::qrCodeFullSetStringWithEqualsInValueTests()
{
    QRCodeStringParser *test = QRCodeStringParser::getInstance();

    // Parse a URI with all possible parameters.
    QVERIFY(test->parseCode("otpauth://totp/ACME%20Co:john.doe@email.com?secret=HXDMVJECJJWSRB3HWIZR4IFUGFTMXBOZ&issuer=ACME%3DCo&algorithm=SHA1&digits=6&period=30"));
    QVERIFY(test->isOtpCode());

    QCOMPARE(test->type(), "totp");
    QCOMPARE(test->label(), "ACME Co:john.doe@email.com");
    QCOMPARE(test->parametersAsString(), "secret=HXDMVJECJJWSRB3HWIZR4IFUGFTMXBOZ&issuer=ACME%3DCo&algorithm=SHA1&digits=6&period=30");

    // Check each of the avp values.
    QCOMPARE(test->parameterByKey("secret"), "HXDMVJECJJWSRB3HWIZR4IFUGFTMXBOZ");
    QCOMPARE(test->parameterByKey("issuer"), "ACME=Co");
    QCOMPARE(test->parameterByKey("algorithm"), "SHA1");
    QCOMPARE(test->parameterByKey("digits"), "6");
    QCOMPARE(test->parameterByKey("period"), "30");
}

void QRCodeStringParserTests::qrCodeFullSetStringWithAmpTests()
{
    QRCodeStringParser *test = QRCodeStringParser::getInstance();

    // Parse a URI with all possible parameters.
    QVERIFY(test->parseCode("otpauth://totp/ACME%26Co:john.doe@email.com?secret=HXDMVJECJJWSRB3HWIZR4IFUGFTMXBOZ&issuer=ACME%26Co&algorithm=SHA1&digits=6&period=30"));
    QVERIFY(test->isOtpCode());

    QCOMPARE(test->type(), "totp");
    QCOMPARE(test->label(), "ACME&Co:john.doe@email.com");
    QCOMPARE(test->parametersAsString(), "secret=HXDMVJECJJWSRB3HWIZR4IFUGFTMXBOZ&issuer=ACME%26Co&algorithm=SHA1&digits=6&period=30");

    // Check each of the avp values.
    QCOMPARE(test->parameterByKey("secret"), "HXDMVJECJJWSRB3HWIZR4IFUGFTMXBOZ");
    QCOMPARE(test->parameterByKey("issuer"), "ACME&Co");
    QCOMPARE(test->parameterByKey("algorithm"), "SHA1");
    QCOMPARE(test->parameterByKey("digits"), "6");
    QCOMPARE(test->parameterByKey("period"), "30");
}

