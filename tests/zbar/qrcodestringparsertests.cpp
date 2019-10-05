#if 0
#include <testsuitebase.h>

#include "zbar/qrcodestringparser.h"

EMPTY_TEST_SUITE(QRCodeStringParserTests);

TEST_F(QRCodeStringParserTests, QRCodeSimpleStringParserTests)
{
    QRCodeStringParser *test = QRCodeStringParser::getInstance();

    EXPECT_TRUE(test->parseCode("otpauth://TYPE/LABEL?secret=secret"));
    EXPECT_TRUE(test->isOtpCode());

    EXPECT_EQ(test->type(), QString("TYPE"));
    EXPECT_EQ(test->label(), QString("LABEL"));
    EXPECT_EQ(test->parametersAsString(), QString("secret=secret"));

    EXPECT_EQ(test->parameterByKey("secret"), QString("secret"));
}

TEST_F(QRCodeStringParserTests, QRCodeInvalidStringParserTests)
{
    QRCodeStringParser *test = QRCodeStringParser::getInstance();

    EXPECT_TRUE(!test->parseCode("nototpauth://TYPE/LABEL?PARAMETERS"));
    EXPECT_EQ(test->isOtpCode(), false);

    // XXX Add more tests.
}

TEST_F(QRCodeStringParserTests, QRCodeFullSetStringParserTests)
{
    QRCodeStringParser *test = QRCodeStringParser::getInstance();

    // Parse a URI with all possible parameters.
    EXPECT_TRUE(test->parseCode("otpauth://totp/ACME%20Co:john.doe@email.com?secret=HXDMVJECJJWSRB3HWIZR4IFUGFTMXBOZ&issuer=ACME%20Co&algorithm=SHA1&digits=6&period=30"));
    EXPECT_TRUE(test->isOtpCode());

    EXPECT_EQ(test->type(), QString("totp"));
    EXPECT_EQ(test->label(), QString("ACME Co:john.doe@email.com"));
    EXPECT_EQ(test->parametersAsString(), QString("secret=HXDMVJECJJWSRB3HWIZR4IFUGFTMXBOZ&issuer=ACME%20Co&algorithm=SHA1&digits=6&period=30"));

    // Check each of the avp values.
    EXPECT_EQ(test->parameterByKey("secret"), QString("HXDMVJECJJWSRB3HWIZR4IFUGFTMXBOZ"));
    EXPECT_EQ(test->parameterByKey("issuer"), QString("ACME Co"));
    EXPECT_EQ(test->parameterByKey("algorithm"), QString("SHA1"));
    EXPECT_EQ(test->parameterByKey("digits"), QString("6"));
    EXPECT_EQ(test->parameterByKey("period"), QString("30"));
}

TEST_F(QRCodeStringParserTests, QRCodeFullSetStringWithEqualsInValueTests)
{
    QRCodeStringParser *test = QRCodeStringParser::getInstance();

    // Parse a URI with all possible parameters.
    EXPECT_TRUE(test->parseCode("otpauth://totp/ACME%20Co:john.doe@email.com?secret=HXDMVJECJJWSRB3HWIZR4IFUGFTMXBOZ&issuer=ACME%3DCo&algorithm=SHA1&digits=6&period=30"));
    EXPECT_TRUE(test->isOtpCode());

    EXPECT_EQ(test->type(), QString("totp"));
    EXPECT_EQ(test->label(), QString("ACME Co:john.doe@email.com"));
    EXPECT_EQ(test->parametersAsString(), QString("secret=HXDMVJECJJWSRB3HWIZR4IFUGFTMXBOZ&issuer=ACME%3DCo&algorithm=SHA1&digits=6&period=30"));

    // Check each of the avp values.
    EXPECT_EQ(test->parameterByKey("secret"), QString("HXDMVJECJJWSRB3HWIZR4IFUGFTMXBOZ"));
    EXPECT_EQ(test->parameterByKey("issuer"), QString("ACME=Co"));
    EXPECT_EQ(test->parameterByKey("algorithm"), QString("SHA1"));
    EXPECT_EQ(test->parameterByKey("digits"), QString("6"));
    EXPECT_EQ(test->parameterByKey("period"), QString("30"));
}

TEST_F(QRCodeStringParserTests, QRCodeFullSetStringWithAmpTests)
{
    QRCodeStringParser *test = QRCodeStringParser::getInstance();

    // Parse a URI with all possible parameters.
    EXPECT_TRUE(test->parseCode("otpauth://totp/ACME%26Co:john.doe@email.com?secret=HXDMVJECJJWSRB3HWIZR4IFUGFTMXBOZ&issuer=ACME%26Co&algorithm=SHA1&digits=6&period=30"));
    EXPECT_TRUE(test->isOtpCode());

    EXPECT_EQ(test->type(), QString("totp"));
    EXPECT_EQ(test->label(), QString("ACME&Co:john.doe@email.com"));
    EXPECT_EQ(test->parametersAsString(), QString("secret=HXDMVJECJJWSRB3HWIZR4IFUGFTMXBOZ&issuer=ACME%26Co&algorithm=SHA1&digits=6&period=30"));

    // Check each of the avp values.
    EXPECT_EQ(test->parameterByKey("secret"), QString("HXDMVJECJJWSRB3HWIZR4IFUGFTMXBOZ"));
    EXPECT_EQ(test->parameterByKey("issuer"), QString("ACME&Co"));
    EXPECT_EQ(test->parameterByKey("algorithm"), QString("SHA1"));
    EXPECT_EQ(test->parameterByKey("digits"), QString("6"));
    EXPECT_EQ(test->parameterByKey("period"), QString("30"));
}
#endif
