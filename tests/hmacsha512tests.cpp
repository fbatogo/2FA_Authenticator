#include "hmacsha512tests.h"

#include <QtTest>
#include "../otpimpl/hmac.h"
#include "../otpimpl/sha512hash.h"
#include "testutils.h"

#include <QDebug>

#include <sstream>
#include <iomanip>

void HmacSha512Tests::hmacTestCase1()
{
    unsigned char expectedDigest[64] = { 0x87, 0xaa, 0x7c, 0xde, 0xa5, 0xef, 0x61, 0x9d, 0x4f, 0xf0, 0xb4, 0x24, 0x1a, 0x1d, 0x6c, 0xb0, 0x23, 0x79, 0xf4, 0xe2, 0xce, 0x4e, 0xc2, 0x78, 0x7a, 0xd0, 0xb3, 0x05, 0x45, 0xe1, 0x7c, 0xde, 0xda, 0xa8, 0x33, 0xb7, 0xd6, 0xb8, 0xa7, 0x02, 0x03, 0x8b, 0x27, 0x4e, 0xae, 0xa3, 0xf4, 0xe4, 0xbe, 0x9d, 0x91, 0x4e, 0xeb, 0x61, 0xf1, 0x70, 0x2e, 0x69, 0x6c, 0x20, 0x3a, 0x12, 0x68, 0x54 };
    ByteArray data("Hi There");
    ByteArray key;
    size_t keyLength = 20;
    Hmac dohmac(std::shared_ptr<HashTypeBase>(new Sha512Hash()));
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
    QCOMPARE(result->size(), static_cast<size_t>(64));
    qDebug("Expected : %s", TestUtils::binaryToString(expectedDigest, 64).c_str());
    qDebug("Got      : %s", TestUtils::binaryToString(result).c_str());
    QVERIFY(memcmp(result->toUCharArrayPtr(), expectedDigest, 64) == 0);
}

void HmacSha512Tests::hmacTestCase2()
{
    unsigned char expectedDigest[64] = { 0x16, 0x4b, 0x7a, 0x7b, 0xfc, 0xf8, 0x19, 0xe2, 0xe3, 0x95, 0xfb, 0xe7, 0x3b, 0x56, 0xe0, 0xa3, 0x87, 0xbd, 0x64, 0x22, 0x2e, 0x83, 0x1f, 0xd6, 0x10, 0x27, 0x0c, 0xd7, 0xea, 0x25, 0x05, 0x54, 0x97, 0x58, 0xbf, 0x75, 0xc0, 0x5a, 0x99, 0x4a, 0x6d, 0x03, 0x4f, 0x65, 0xf8, 0xf0, 0xe6, 0xfd, 0xca, 0xea, 0xb1, 0xa3, 0x4d, 0x4a, 0x6b, 0x4b, 0x63, 0x6e, 0x07, 0x0a, 0x38, 0xbc, 0xe7, 0x37 };
    ByteArray data("what do ya want for nothing?");
    ByteArray key("Jefe");
    size_t keyLength = key.size();
    Hmac dohmac(std::shared_ptr<HashTypeBase>(new Sha512Hash()));
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
    QCOMPARE(result->size(), static_cast<size_t>(64));
    qDebug("Expected : %s", TestUtils::binaryToString(expectedDigest, 64).c_str());
    qDebug("Got      : %s", TestUtils::binaryToString(result).c_str());
    QVERIFY(memcmp(result->toUCharArrayPtr(), expectedDigest, 64) == 0);
}

void HmacSha512Tests::hmacTestCase3()
{
    unsigned char expectedDigest[64] = { 0xfa, 0x73, 0xb0, 0x08, 0x9d, 0x56, 0xa2, 0x84, 0xef, 0xb0, 0xf0, 0x75, 0x6c, 0x89, 0x0b, 0xe9, 0xb1, 0xb5, 0xdb, 0xdd, 0x8e, 0xe8, 0x1a, 0x36, 0x55, 0xf8, 0x3e, 0x33, 0xb2, 0x27, 0x9d, 0x39, 0xbf, 0x3e, 0x84, 0x82, 0x79, 0xa7, 0x22, 0xc8, 0x06, 0xb4, 0x85, 0xa4, 0x7e, 0x67, 0xc8, 0x07, 0xb9, 0x46, 0xa3, 0x37, 0xbe, 0xe8, 0x94, 0x26, 0x74, 0x27, 0x88, 0x59, 0xe1, 0x32, 0x92, 0xfb };
    ByteArray data;
    ByteArray key;
    size_t keyLength = 20;
    Hmac dohmac(std::shared_ptr<HashTypeBase>(new Sha512Hash()));
    std::shared_ptr<ByteArray> result;

    // Pre-allocate enough memory to store all of our data.
    key.setExtraAllocation(keyLength);

    // Build the key.
    for (size_t i = 0; i < keyLength; i++) {
        key.append(0xaa);
    }

    // Build the data.
    data.setExtraAllocation(50);
    for (size_t i = 0; i < 50; i++) {
        data.append(0xdd);
    }

    // Calculate the HMAC.
    result = dohmac.calculate(key, data);

    // Make sure the length is what we expect.
    QCOMPARE(result->size(), static_cast<size_t>(64));
    qDebug("Expected : %s", TestUtils::binaryToString(expectedDigest, 64).c_str());
    qDebug("Got      : %s", TestUtils::binaryToString(result).c_str());
    QVERIFY(memcmp(result->toUCharArrayPtr(), expectedDigest, 64) == 0);
}

void HmacSha512Tests::hmacTestCase4()
{
    size_t keyLength = 25;
    ByteArray key({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19 }, keyLength);
    unsigned char expectedDigest[64] = { 0xb0, 0xba, 0x46, 0x56, 0x37, 0x45, 0x8c, 0x69, 0x90, 0xe5, 0xa8, 0xc5, 0xf6, 0x1d, 0x4a, 0xf7, 0xe5, 0x76, 0xd9, 0x7f, 0xf9, 0x4b, 0x87, 0x2d, 0xe7, 0x6f, 0x80, 0x50, 0x36, 0x1e, 0xe3, 0xdb, 0xa9, 0x1c, 0xa5, 0xc1, 0x1a, 0xa2, 0x5e, 0xb4, 0xd6, 0x79, 0x27, 0x5c, 0xc5, 0x78, 0x80, 0x63, 0xa5, 0xf1, 0x97, 0x41, 0x12, 0x0c, 0x4f, 0x2d, 0xe2, 0xad, 0xeb, 0xeb, 0x10, 0xa2, 0x98, 0xdd };
    ByteArray data;
    Hmac dohmac(std::shared_ptr<HashTypeBase>(new Sha512Hash()));
    std::shared_ptr<ByteArray> result;

    // Build the data.
    data.setExtraAllocation(50);
    for (size_t i = 0; i < 50; i++) {
        data.append(0xcd);
    }

    // Calculate the HMAC.
    result = dohmac.calculate(key, data);

    // Make sure the length is what we expect.
    QCOMPARE(result->size(), static_cast<size_t>(64));
    qDebug("Expected : %s", TestUtils::binaryToString(expectedDigest, 64).c_str());
    qDebug("Got      : %s", TestUtils::binaryToString(result).c_str());
    QVERIFY(memcmp(result->toUCharArrayPtr(), expectedDigest, 64) == 0);
}

void HmacSha512Tests::hmacTestCase5()
{
    unsigned char expectedDigest[16] = { 0x41, 0x5f, 0xad, 0x62, 0x71, 0x58, 0x0a, 0x53, 0x1d, 0x41, 0x79, 0xbc, 0x89, 0x1d, 0x87, 0xa6 };
    ByteArray data("Test With Truncation");
    ByteArray key;
    size_t keyLength = 20;
    Hmac dohmac(std::shared_ptr<HashTypeBase>(new Sha512Hash()));
    std::shared_ptr<ByteArray> result;

    // Pre-allocate enough memory to store all of our data.
    key.setExtraAllocation(keyLength);

    // Build the key.
    for (size_t i = 0; i < keyLength; i++) {
        key.append(0x0c);
    }

    // Calculate the HMAC.
    result = dohmac.calculate(key, data);

    // Make sure the length is what we expect.
    QCOMPARE(result->size(), static_cast<size_t>(64));
    qDebug("Expected : %s", TestUtils::binaryToString(expectedDigest, 64).c_str());
    qDebug("Got      : %s", TestUtils::binaryToString(result).c_str());
    QVERIFY(memcmp(result->toUCharArrayPtr(), expectedDigest, 64) == 0);
}

void HmacSha512Tests::hmacTestCase6()
{
    unsigned char expectedDigest[64] = { 0x80, 0xb2, 0x42, 0x63, 0xc7, 0xc1, 0xa3, 0xeb, 0xb7, 0x14, 0x93, 0xc1, 0xdd, 0x7b, 0xe8, 0xb4, 0x9b, 0x46, 0xd1, 0xf4, 0x1b, 0x4a, 0xee, 0xc1, 0x12, 0x1b, 0x01, 0x37, 0x83, 0xf8, 0xf3, 0x52, 0x6b, 0x56, 0xd0, 0x37, 0xe0, 0x5f, 0x25, 0x98, 0xbd, 0x0f, 0xd2, 0x21, 0x5d, 0x6a, 0x1e, 0x52, 0x95, 0xe6, 0x4f, 0x73, 0xf6, 0x3f, 0x0a, 0xec, 0x8b, 0x91, 0x5a, 0x98, 0x5d, 0x78, 0x65, 0x98 };
    ByteArray data("Test Using Larger Than Block-Size Key - Hash Key First");
    ByteArray key;
    size_t keyLength = 131;
    Hmac dohmac(std::shared_ptr<HashTypeBase>(new Sha512Hash()));
    std::shared_ptr<ByteArray> result;

    // Pre-allocate enough memory to store all of our data.
    key.setExtraAllocation(keyLength);

    // Build the key.
    for (size_t i = 0; i < keyLength; i++) {
        key.append(0x0c);
    }

    // Calculate the HMAC.
    result = dohmac.calculate(key, data);

    // Make sure the length is what we expect.
    QCOMPARE(result->size(), static_cast<size_t>(20));
    qDebug("Expected : %s", TestUtils::binaryToString(expectedDigest, 20).c_str());
    qDebug("Got      : %s", TestUtils::binaryToString(result).c_str());
    QVERIFY(memcmp(result->toUCharArrayPtr(), expectedDigest, 20) == 0);
}

void HmacSha512Tests::hmacTestCase7()
{
    unsigned char expectedDigest1[64] = { 0xe3, 0x7b, 0x6a, 0x77, 0x5d, 0xc8, 0x7d, 0xba, 0xa4, 0xdf, 0xa9, 0xf9, 0x6e, 0x5e, 0x3f, 0xfd, 0xde, 0xbd, 0x71, 0xf8, 0x86, 0x72, 0x89, 0x86, 0x5d, 0xf5, 0xa3, 0x2d, 0x20, 0xcd, 0xc9, 0x44, 0xb6, 0x02, 0x2c, 0xac, 0x3c, 0x49, 0x82, 0xb1, 0x0d, 0x5e, 0xeb, 0x55, 0xc3, 0xe4, 0xde, 0x15, 0x13, 0x46, 0x76, 0xfb, 0x6d, 0xe0, 0x44, 0x60, 0x65, 0xc9, 0x74, 0x40, 0xfa, 0x8c, 0x6a, 0x58 };
    ByteArray data("This is a test using a larger than block-size key and a larger than block-size data. The key needs to be hashed before being used by the HMAC algorithm.");
    ByteArray key;
    size_t keyLength = 131;
    Hmac dohmac(std::shared_ptr<HashTypeBase>(new Sha512Hash()));
    std::shared_ptr<ByteArray> result;

    // Pre-allocate enough memory to store all of our data.
    key.setExtraAllocation(keyLength);

    // Build the key.
    for (size_t i = 0; i < keyLength; i++) {
        key.append(0xaa);
    }

    // Calculate the HMAC.
    result = dohmac.calculate(key, data);

    // Make sure the length is what we expect.
    QCOMPARE(result->size(), static_cast<size_t>(64));
    qDebug("Expected : %s", TestUtils::binaryToString(expectedDigest1, 64).c_str());
    qDebug("Got      : %s", TestUtils::binaryToString(result).c_str());
    QVERIFY(memcmp(result->toUCharArrayPtr(), expectedDigest1, 64) == 0);
}
