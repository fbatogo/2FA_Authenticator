#include "base32codertests.h"

#include <QtTest>
#include "../otpimpl/base32coder.h"

#include "testutils.h"

#include <vector>
#include <iostream>
#include <QDebug>

// Tests from RFC 4648, found at https://tools.ietf.org/html/rfc4648
// The final test was generated using the base32 encoder at : https://emn178.github.io/online-tools/base32_encode.html

void Base32CoderTests::encoderTest1()
{
    Base32Coder encoder;
    std::vector<ByteArray> clearText;
    std::vector<ByteArray> encodedText;
    ByteArray encoded;

    clearText = getClearTextTests();
    encodedText = getEncodedTextTests();

    for (size_t i = 0; i < clearText.size(); i++) {
        encoded = encoder.encode(clearText.at(i));
        qDebug("Expected : %s    Calculated : %s", encodedText.at(i).toCharArrayPtr(), encoded.toCharArrayPtr());
        QCOMPARE(encodedText.at(i), encoded);
    }
}

void Base32CoderTests::decoderTest1()
{
    Base32Coder decoder;
    std::vector<ByteArray> clearText;
    std::vector<ByteArray> encodedText;
    ByteArray decoded;

    clearText = getClearTextTests();
    encodedText = getEncodedTextTests();

    for (size_t i = 0; i < clearText.size(); i++) {
        decoded = decoder.decode(encodedText.at(i));

        qDebug("%s  Expected : %s", clearText.at(i).toCharArrayPtr(), TestUtils::binaryToString(clearText.at(i)).c_str());
        qDebug("%s  Decoded  : %s", clearText.at(i).toCharArrayPtr(), TestUtils::binaryToString(decoded).c_str());

        // Compare the results.
        QCOMPARE(clearText, decoded);
    }
}

void Base32CoderTests::isBase32EncodedTests()
{
    std::vector<ByteArray> toTest;

    // Start with valid tests.
    toTest = getEncodedTextTests();

    // Skip the value at index 0, because an empty string would be invalid.
    for (size_t i = 1; i < toTest.size(); i++) {
        QVERIFY(Base32Coder::isBase32Encoded(toTest.at(i)));
    }

    // Then, use the original clear text as negative tests.
    toTest = getClearTextTests();

    for (size_t i = 0; i < toTest.size(); i++) {
        QVERIFY(!Base32Coder::isBase32Encoded(toTest.at(i)));
    }
}

std::vector<ByteArray> Base32CoderTests::getClearTextTests()
{
    std::vector<ByteArray> result;

    result.clear();
    result.push_back(ByteArray(""));
    result.push_back(ByteArray("f"));
    result.push_back(ByteArray("fo"));
    result.push_back(ByteArray("foo"));
    result.push_back(ByteArray("foob"));
    result.push_back(ByteArray("fooba"));
    result.push_back(ByteArray("foobar"));
    result.push_back(ByteArray("this is a long test for base32 encoding.."));

    return result;
}

std::vector<ByteArray> Base32CoderTests::getEncodedTextTests()
{
    std::vector<ByteArray> result;

    result.clear();
    result.push_back(ByteArray(""));
    result.push_back(ByteArray("MY======"));
    result.push_back(ByteArray("MZXQ===="));
    result.push_back(ByteArray("MZXW6==="));
    result.push_back(ByteArray("MZXW6YQ="));
    result.push_back(ByteArray("MZXW6YTB"));
    result.push_back(ByteArray("MZXW6YTBOI======"));
    result.push_back(ByteArray("ORUGS4ZANFZSAYJANRXW4ZZAORSXG5BAMZXXEIDCMFZWKMZSEBSW4Y3PMRUW4ZZOFY======"));

    return result;
}
