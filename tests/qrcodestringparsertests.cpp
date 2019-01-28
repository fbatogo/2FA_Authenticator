#include "qrcodestringparsertests.h"

#include <QTest>

void QRCodeStringParserTests::qrCodeSimpleStringParserTests()
{
    QRCodeStringParser *test;

    test = new QRCodeStringParser("otpauth://TYPE/LABEL?PARAMETERS");
    QVERIFY(test->isOtpCode());

    QCOMPARE(test->type(), "TYPE");
    QCOMPARE(test->label(), "LABEL");
    QCOMPARE(test->parametersAsString(), "PARAMETERS");

    // Clean up.
    delete test;
}

void QRCodeStringParserTests::qrCodeInvalidStringParserTests()
{
    QRCodeStringParser *test;

    test = new QRCodeStringParser("nototpauth://TYPE/LABEL?PARAMETERS");
    QCOMPARE(test->isOtpCode(), false);

    // XXX Add more tests..

    // Clean up.
    delete test;
}

void QRCodeStringParserTests::qrCodeFullSetStringParametersTests()
{
    QRCodeStringParser *test;

    // Parse a URI with all possible parameters.
    test = new QRCodeStringParser("otpauth://totp/ACME%20Co:john.doe@email.com?secret=HXDMVJECJJWSRB3HWIZR4IFUGFTMXBOZ&issuer=ACME%20Co&algorithm=SHA1&digits=6&period=30");
    QVERIFY(test->isOtpCode());

    QCOMPARE(test->type(), "totp");
    QCOMPARE(test->label(), "ACME Co:john.doe@email.com");
    QCOMPARE(test->parametersAsString(), "secret=HXDMVJECJJWSRB3HWIZR4IFUGFTMXBOZ&issuer=ACME Co&algorithm=SHA1&digits=6&period=30");
}
