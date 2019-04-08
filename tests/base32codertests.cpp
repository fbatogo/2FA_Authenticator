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
    std::string test;
    unsigned char *testPtr;
    std::vector<std::string> clearText;
    std::vector<std::string> encodedText;
    std::string encoded;

    clearText = getClearTextTests();
    encodedText = getEncodedTextTests();

    for (size_t i = 0; i < clearText.size(); i++) {
        testPtr = reinterpret_cast<unsigned char *>(strdup(clearText.at(i).c_str()));
        encoded = encoder.encode(testPtr, clearText.at(i).length());
        qDebug("Expected : %s    Calculated : %s", encodedText.at(i).c_str(), encoded.c_str());
        QCOMPARE(encodedText.at(i), encoded);
        free(testPtr);
    }
}

void Base32CoderTests::decoderTest1()
{
    Base32Coder decoder;
    std::string test;
    unsigned char *testPtr;
    size_t testPtrSize;
    std::vector<std::string> clearText;
    std::vector<std::string> encodedText;
    std::string encoded;
    unsigned char *toCompare;

    clearText = getClearTextTests();
    encodedText = getEncodedTextTests();

    for (size_t i = 0; i < clearText.size(); i++) {
        testPtr = decoder.decode(encodedText.at(i), testPtrSize);
        toCompare = reinterpret_cast<unsigned char *>(strdup(clearText.at(i).c_str()));

        qDebug("%s  Expected : %s", clearText.at(i).c_str(), TestUtils::binaryToString(toCompare, clearText.at(i).length()).c_str());
        qDebug("%s  Decoded  : %s", clearText.at(i).c_str(), TestUtils::binaryToString(testPtr, testPtrSize).c_str());

        // The first one we compare is an empty string, so if both have a 0 length, everything is good.
        if (i != 0) {
            QVERIFY(memcmp(testPtr, toCompare, testPtrSize) == 0);
        } else {
            // Otherwise, the lengths should be 0.
            if (testPtrSize != 0) {
                QFAIL("Index 0 wasn't 0 length!\n");
            }
        }

        // Clean up.
        free(testPtr);
        free(toCompare);
    }
}

void Base32CoderTests::isBase32EncodedTests()
{
    std::vector<std::string> toTest;

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

std::vector<std::string> Base32CoderTests::getClearTextTests()
{
    std::vector<std::string> result;

    result.clear();
    result.push_back("");
    result.push_back("f");
    result.push_back("fo");
    result.push_back("foo");
    result.push_back("foob");
    result.push_back("fooba");
    result.push_back("foobar");
    result.push_back("this is a long test for base32 encoding..");

    return result;
}

std::vector<std::string> Base32CoderTests::getEncodedTextTests()
{
    std::vector<std::string> result;

    result.clear();
    result.push_back("");
    result.push_back("MY======");
    result.push_back("MZXQ====");
    result.push_back("MZXW6===");
    result.push_back("MZXW6YQ=");
    result.push_back("MZXW6YTB");
    result.push_back("MZXW6YTBOI======");
    result.push_back("ORUGS4ZANFZSAYJANRXW4ZZAORSXG5BAMZXXEIDCMFZWKMZSEBSW4Y3PMRUW4ZZOFY======");

    return result;
}
