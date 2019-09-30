#include <testsuitebase.h>

#include "otpimpl/base32coder.h"

#include "testutils.h"

#include <vector>
#include <iostream>
#include <QDebug>

SIMPLE_TEST_SUITE(Base32CoderTests, Base32Coder);

// Some forward decls.
std::vector<ByteArray> getEncodedTextTests();
std::vector<ByteArray> getClearTextTests();

// Tests from RFC 4648, found at https://tools.ietf.org/html/rfc4648
// The final test was generated using the base32 encoder at : https://emn178.github.io/online-tools/base32_encode.html

TEST_F(Base32CoderTests, EncoderTest1)
{
    std::vector<ByteArray> clearText;
    std::vector<ByteArray> encodedText;
    ByteArray encoded;

    clearText = getClearTextTests();
    encodedText = getEncodedTextTests();

    for (size_t i = 0; i < clearText.size(); i++) {
        encoded = encode(clearText.at(i));
        qDebug("Expected : %s    Calculated : %s", encodedText.at(i).toCharArrayPtr(), encoded.toCharArrayPtr());
        if (encodedText.at(i) != encoded) {
            FAIL() << "Encoded text at index " + QString::number(i).toStdString() + " didn't match!";
        }
    }
}

TEST_F(Base32CoderTests, DecoderTest1)
{
    std::vector<ByteArray> clearText;
    std::vector<ByteArray> encodedText;
    ByteArray decoded;

    // Try to decode some invalid blocks.
    decoded = decode("1234");
    EXPECT_TRUE(decoded.empty());

    decoded = decode("11111111");
    EXPECT_TRUE(decoded.empty());

    clearText = getClearTextTests();
    encodedText = getEncodedTextTests();

    for (size_t i = 0; i < clearText.size(); i++) {
        decoded = decode(encodedText.at(i));

        qDebug("%s  Expected : %s", clearText.at(i).toCharArrayPtr(), TestUtils::binaryToString(clearText.at(i)).c_str());
        qDebug("%s  Decoded  : %s", clearText.at(i).toCharArrayPtr(), TestUtils::binaryToString(decoded).c_str());

        // Compare the results.
        EXPECT_TRUE(clearText.at(i) == decoded);
    }
}

TEST_F(Base32CoderTests, IsBase32EncodedTests)
{
    std::vector<ByteArray> toTest;

    // Start with valid tests.
    toTest = getEncodedTextTests();

    // Skip the value at index 0, because an empty string would be invalid.
    for (size_t i = 1; i < toTest.size(); i++) {
        EXPECT_TRUE(Base32Coder::isBase32Encoded(toTest.at(i)));
    }

    // Then, use the original clear text as negative tests.
    toTest = getClearTextTests();

    for (size_t i = 0; i < toTest.size(); i++) {
        EXPECT_TRUE(!Base32Coder::isBase32Encoded(toTest.at(i)));
    }

    EXPECT_TRUE(!Base32Coder::isBase32Encoded("11111111"));
}

TEST_F(Base32CoderTests, NegativeTests)
{
    ByteArray result;

    // Try to decode something too short.
    EXPECT_TRUE(!decode8Chars(ByteArray("1111"), 0, result));

    // Try to decode something with an invalid character in each position.
    EXPECT_TRUE(!decode8Chars(ByteArray("1ZXW6YTB"), 0, result));
    EXPECT_TRUE(!decode8Chars(ByteArray("M1XW6YTB"), 0, result));
    EXPECT_TRUE(!decode8Chars(ByteArray("MZ1W6YTB"), 0, result));
    EXPECT_TRUE(!decode8Chars(ByteArray("MZX16YTB"), 0, result));
    EXPECT_TRUE(!decode8Chars(ByteArray("MZXW1YTB"), 0, result));
    EXPECT_TRUE(!decode8Chars(ByteArray("MZXW61TB"), 0, result));
    EXPECT_TRUE(!decode8Chars(ByteArray("MZXW6Y1B"), 0, result));
    EXPECT_TRUE(!decode8Chars(ByteArray("MZXW6YT1"), 0, result));
}

std::vector<ByteArray> getClearTextTests()
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

std::vector<ByteArray> getEncodedTextTests()
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
