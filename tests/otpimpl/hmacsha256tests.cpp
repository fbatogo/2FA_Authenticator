#include <testsuitebase.h>

#include "otpimpl/hmac.h"
#include "otpimpl/sha256hash.h"
#include "testutils.h"

#include <QDebug>

#include <sstream>
#include <iomanip>

EMPTY_TEST_SUITE(HmacSha256Tests);

TEST_F(HmacSha256Tests, HmacTestCase1)
{
    unsigned char expectedDigest[32] = { 0xb0, 0x34, 0x4c, 0x61, 0xd8, 0xdb, 0x38, 0x53, 0x5c, 0xa8, 0xaf, 0xce, 0xaf, 0x0b, 0xf1, 0x2b, 0x88, 0x1d, 0xc2, 0x00, 0xc9, 0x83, 0x3d, 0xa7, 0x26, 0xe9, 0x37, 0x6c, 0x2e, 0x32, 0xcf, 0xf7 };
    ByteArray data("Hi There");
    ByteArray key;
    size_t keyLength = 20;
    std::shared_ptr<HashTypeBase> hashType(new Sha256Hash());
    Hmac dohmac(hashType);
    std::shared_ptr<ByteArray> result;

    // Pre-allocate enough memory to store all of our data.
    key.setExtraAllocation(keyLength);

    // Build the key.
    for (size_t i = 0; i < keyLength; i++) {
        key.append(0x0b);
    }

    // Calculate the HMAC.
    result = dohmac.calculate(key, data);

    // Make sure the length is what we expect.
    EXPECT_EQ(result->size(), static_cast<size_t>(hashType->hashResultLength()));
    qDebug("Expected : %s", TestUtils::binaryToString(expectedDigest, 32).c_str());
    qDebug("Got      : %s", TestUtils::binaryToString(result).c_str());
    EXPECT_TRUE(memcmp(result->toUCharArrayPtr(), expectedDigest, 32) == 0);
}

TEST_F(HmacSha256Tests, HmacTestCase2)
{
    unsigned char expectedDigest[32] = { 0x5b, 0xdc, 0xc1, 0x46, 0xbf, 0x60, 0x75, 0x4e, 0x6a, 0x04, 0x24, 0x26, 0x08, 0x95, 0x75, 0xc7, 0x5a, 0x00, 0x3f, 0x08, 0x9d, 0x27, 0x39, 0x83, 0x9d, 0xec, 0x58, 0xb9, 0x64, 0xec, 0x38, 0x43 };
    ByteArray data("what do ya want for nothing?");
    ByteArray key("Jefe");
    Hmac dohmac(std::shared_ptr<HashTypeBase>(new Sha256Hash()));
    std::shared_ptr<ByteArray> result;

    // Calculate the HMAC.
    result = dohmac.calculate(key, data);

    // Make sure the length is what we expect.
    EXPECT_EQ(result->size(), static_cast<size_t>(32));
    qDebug("Expected : %s", TestUtils::binaryToString(expectedDigest, 32).c_str());
    qDebug("Got      : %s", TestUtils::binaryToString(result).c_str());
    EXPECT_TRUE(memcmp(result->toUCharArrayPtr(), expectedDigest, 32) == 0);
}

TEST_F(HmacSha256Tests, HmacTestCase3)
{
    unsigned char expectedDigest[32] = { 0x77, 0x3e, 0xa9, 0x1e, 0x36, 0x80, 0x0e, 0x46, 0x85, 0x4d, 0xb8, 0xeb, 0xd0, 0x91, 0x81, 0xa7, 0x29, 0x59, 0x09, 0x8b, 0x3e, 0xf8, 0xc1, 0x22, 0xd9, 0x63, 0x55, 0x14, 0xce, 0xd5, 0x65, 0xfe };
    ByteArray data;
    ByteArray key;
    size_t keyLength = 20;
    Hmac dohmac(std::shared_ptr<HashTypeBase>(new Sha256Hash()));
    std::shared_ptr<ByteArray> result;

    // Pre-allocate enough memory to store all of our data.
    key.setExtraAllocation(keyLength);

    // Build the key.
    for (size_t i = 0; i < keyLength; i++) {
        key.append((char)0xaa);
    }

    // Build the data.
    data.setExtraAllocation(50);
    for (size_t i = 0; i < 50; i++) {
        data.append((char)0xdd);
    }

    // Calculate the HMAC.
    result = dohmac.calculate(key, data);

    // Make sure the length is what we expect.
    EXPECT_EQ(result->size(), static_cast<size_t>(32));
    qDebug("Expected : %s", TestUtils::binaryToString(expectedDigest, 32).c_str());
    qDebug("Got      : %s", TestUtils::binaryToString(result).c_str());
    EXPECT_TRUE(memcmp(result->toUCharArrayPtr(), expectedDigest, 32) == 0);
}

TEST_F(HmacSha256Tests, HmacTestCase4)
{
    size_t keyLength = 25;
    ByteArray key({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19 }, keyLength);
    unsigned char expectedDigest[32] = { 0x82, 0x55, 0x8a, 0x38, 0x9a, 0x44, 0x3c, 0x0e, 0xa4, 0xcc, 0x81, 0x98, 0x99, 0xf2, 0x08, 0x3a, 0x85, 0xf0, 0xfa, 0xa3, 0xe5, 0x78, 0xf8, 0x07, 0x7a, 0x2e, 0x3f, 0xf4, 0x67, 0x29, 0x66, 0x5b };
    ByteArray data;
    Hmac dohmac(std::shared_ptr<HashTypeBase>(new Sha256Hash()));
    std::shared_ptr<ByteArray> result;

    // Build the data.
    data.setExtraAllocation(50);

    for (size_t i = 0; i < 50; i++) {
        data.append((char)0xcd);
    }

    // Calculate the HMAC.
    result = dohmac.calculate(key, data);

    // Make sure the length is what we expect.
    EXPECT_EQ(result->size(), static_cast<size_t>(32));
    qDebug("Expected : %s", TestUtils::binaryToString(expectedDigest, 32).c_str());
    qDebug("Got      : %s", TestUtils::binaryToString(result).c_str());
    EXPECT_TRUE(memcmp(result->toUCharArrayPtr(), expectedDigest, 32) == 0);
}

TEST_F(HmacSha256Tests, HmacTestCase5)
{
    unsigned char expectedDigest[16] = { 0xa3, 0xb6, 0x16, 0x74, 0x73, 0x10, 0x0e, 0xe0, 0x6e, 0x0c, 0x79, 0x6c, 0x29, 0x55, 0x55, 0x2b };
    ByteArray data("Test With Truncation");
    ByteArray key;
    size_t keyLength = 20;
    Hmac dohmac(std::shared_ptr<HashTypeBase>(new Sha256Hash()));
    std::shared_ptr<ByteArray> result;

    // Pre-allocate enough memory to store all of our data.
    key.setExtraAllocation(keyLength);

    // Build the key.
    for (size_t i = 0; i < keyLength; i++) {
        key.append(0x0c);
    }

    // Calculate the HMAC.
    result = dohmac.calculate(key, data);

    // We only want 128 bits worth of result.
    EXPECT_TRUE(result->truncate(16));

    // Make sure the length is what we expect.
    EXPECT_EQ(result->size(), static_cast<size_t>(16));
    qDebug("Expected : %s", TestUtils::binaryToString(expectedDigest, 16).c_str()); // We only expect 16 bytes, since it was truncated.
    qDebug("Got      : %s", TestUtils::binaryToString(result).c_str());
    EXPECT_TRUE(memcmp(result->toUCharArrayPtr(), expectedDigest, 16) == 0);
}

TEST_F(HmacSha256Tests, HmacTestCase6)
{
    unsigned char expectedDigest[32] = { 0x60, 0xe4, 0x31, 0x59, 0x1e, 0xe0, 0xb6, 0x7f, 0x0d, 0x8a, 0x26, 0xaa, 0xcb, 0xf5, 0xb7, 0x7f, 0x8e, 0x0b, 0xc6, 0x21, 0x37, 0x28, 0xc5, 0x14, 0x05, 0x46, 0x04, 0x0f, 0x0e, 0xe3, 0x7f, 0x54 };
    ByteArray data("Test Using Larger Than Block-Size Key - Hash Key First");
    ByteArray key;
    size_t keyLength = 131;
    Hmac dohmac(std::shared_ptr<HashTypeBase>(new Sha256Hash()));
    std::shared_ptr<ByteArray> result;

    // Pre-allocate enough memory to store all of our data.
    key.setExtraAllocation(keyLength);

    // Build the key.
    for (size_t i = 0; i < keyLength; i++) {
        key.append((char)0xaa);
    }

    // Calculate the HMAC.
    result = dohmac.calculate(key, data);

    // Make sure the length is what we expect.
    EXPECT_EQ(result->size(), static_cast<size_t>(32));
    qDebug("Expected : %s", TestUtils::binaryToString(expectedDigest, 32).c_str());
    qDebug("Got      : %s", TestUtils::binaryToString(result).c_str());
    EXPECT_TRUE(memcmp(result->toUCharArrayPtr(), expectedDigest, 32) == 0);
}

TEST_F(HmacSha256Tests, HmacTestCase7)
{
    unsigned char expectedDigest1[32] = { 0x9b, 0x09, 0xff, 0xa7, 0x1b, 0x94, 0x2f, 0xcb, 0x27, 0x63, 0x5f, 0xbc, 0xd5, 0xb0, 0xe9, 0x44, 0xbf, 0xdc, 0x63, 0x64, 0x4f, 0x07, 0x13, 0x93, 0x8a, 0x7f, 0x51, 0x53, 0x5c, 0x3a, 0x35, 0xe2 };
    ByteArray data("This is a test using a larger than block-size key and a larger than block-size data. The key needs to be hashed before being used by the HMAC algorithm.");
    ByteArray key;
    size_t keyLength = 131;
    Hmac dohmac(std::shared_ptr<HashTypeBase>(new Sha256Hash()));
    std::shared_ptr<ByteArray> result;

    // Pre-allocate enough memory to store all of our data.
    key.setExtraAllocation(keyLength);

    // Build the key.
    for (size_t i = 0; i < keyLength; i++) {
        key.append((char)0xaa);
    }

    // Calculate the HMAC.
    result = dohmac.calculate(key, data);

    // Make sure the length is what we expect.
    EXPECT_EQ(result->size(), static_cast<size_t>(32));
    qDebug("Expected : %s", TestUtils::binaryToString(expectedDigest1, 32).c_str());
    qDebug("Got      : %s", TestUtils::binaryToString(result).c_str());
    EXPECT_TRUE(memcmp(result->toUCharArrayPtr(), expectedDigest1, 32) == 0);
}
