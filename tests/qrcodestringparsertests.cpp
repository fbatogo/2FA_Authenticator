#include "qrcodestringparsertests.h"

#include <QTest>

void QRCodeStringParserTests::qrCodeSimpleStringParserTests()
{
    QRCodeStringParser *test = QRCodeStringParser::getInstance();

    QVERIFY(test->parseCode("otpauth://TYPE/LABEL?secret=secret"));
    QVERIFY(test->isOtpCode());

    QCOMPARE(test->type(), QString("TYPE"));
    QCOMPARE(test->label(), QString("LABEL"));
    QCOMPARE(test->parametersAsString(), QString("secret=secret"));

    QCOMPARE(test->parameterByKey("secret"), QString("secret"));
}

void QRCodeStringParserTests::qrCodeInvalidStringParserTests()
{
    QRCodeStringParser *test = QRCodeStringParser::getInstance();

    QVERIFY(!test->parseCode("nototpauth://TYPE/LABEL?PARAMETERS"));
    QCOMPARE(test->isOtpCode(), false);

    // XXX Add more tests.
}

void QRCodeStringParserTests::qrCodeFullSetStringParametersTests()
{
    QRCodeStringParser *test = QRCodeStringParser::getInstance();

    // Parse a URI with all possible parameters.
    QVERIFY(test->parseCode("otpauth://totp/ACME%20Co:john.doe@email.com?secret=HXDMVJECJJWSRB3HWIZR4IFUGFTMXBOZ&issuer=ACME%20Co&algorithm=SHA1&digits=6&period=30"));
    QVERIFY(test->isOtpCode());

    QCOMPARE(test->type(), QString("totp"));
    QCOMPARE(test->label(), QString("ACME Co:john.doe@email.com"));
    QCOMPARE(test->parametersAsString(), QString("secret=HXDMVJECJJWSRB3HWIZR4IFUGFTMXBOZ&issuer=ACME%20Co&algorithm=SHA1&digits=6&period=30"));

    // Check each of the avp values.
    QCOMPARE(test->parameterByKey("secret"), QString("HXDMVJECJJWSRB3HWIZR4IFUGFTMXBOZ"));
    QCOMPARE(test->parameterByKey("issuer"), QString("ACME Co"));
    QCOMPARE(test->parameterByKey("algorithm"), QString("SHA1"));
    QCOMPARE(test->parameterByKey("digits"), QString("6"));
    QCOMPARE(test->parameterByKey("period"), QString("30"));
}

void QRCodeStringParserTests::qrCodeFullSetStringWithEqualsInValueTests()
{
    QRCodeStringParser *test = QRCodeStringParser::getInstance();

    // Parse a URI with all possible parameters.
    QVERIFY(test->parseCode("otpauth://totp/ACME%20Co:john.doe@email.com?secret=HXDMVJECJJWSRB3HWIZR4IFUGFTMXBOZ&issuer=ACME%3DCo&algorithm=SHA1&digits=6&period=30"));
    QVERIFY(test->isOtpCode());

    QCOMPARE(test->type(), QString("totp"));
    QCOMPARE(test->label(), QString("ACME Co:john.doe@email.com"));
    QCOMPARE(test->parametersAsString(), QString("secret=HXDMVJECJJWSRB3HWIZR4IFUGFTMXBOZ&issuer=ACME%3DCo&algorithm=SHA1&digits=6&period=30"));

    // Check each of the avp values.
    QCOMPARE(test->parameterByKey("secret"), QString("HXDMVJECJJWSRB3HWIZR4IFUGFTMXBOZ"));
    QCOMPARE(test->parameterByKey("issuer"), QString("ACME=Co"));
    QCOMPARE(test->parameterByKey("algorithm"), QString("SHA1"));
    QCOMPARE(test->parameterByKey("digits"), QString("6"));
    QCOMPARE(test->parameterByKey("period"), QString("30"));
}

void QRCodeStringParserTests::qrCodeFullSetStringWithAmpTests()
{
    QRCodeStringParser *test = QRCodeStringParser::getInstance();

    // Parse a URI with all possible parameters.
    QVERIFY(test->parseCode("otpauth://totp/ACME%26Co:john.doe@email.com?secret=HXDMVJECJJWSRB3HWIZR4IFUGFTMXBOZ&issuer=ACME%26Co&algorithm=SHA1&digits=6&period=30"));
    QVERIFY(test->isOtpCode());

    QCOMPARE(test->type(), QString("totp"));
    QCOMPARE(test->label(), QString("ACME&Co:john.doe@email.com"));
    QCOMPARE(test->parametersAsString(), QString("secret=HXDMVJECJJWSRB3HWIZR4IFUGFTMXBOZ&issuer=ACME%26Co&algorithm=SHA1&digits=6&period=30"));

    // Check each of the avp values.
    QCOMPARE(test->parameterByKey("secret"), QString("HXDMVJECJJWSRB3HWIZR4IFUGFTMXBOZ"));
    QCOMPARE(test->parameterByKey("issuer"), QString("ACME&Co"));
    QCOMPARE(test->parameterByKey("algorithm"), QString("SHA1"));
    QCOMPARE(test->parameterByKey("digits"), QString("6"));
    QCOMPARE(test->parameterByKey("period"), QString("30"));
}

