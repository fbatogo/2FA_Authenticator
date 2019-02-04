#include "hmacsha1tests.h"

#include "../otpimpl/hmac.h"
#include "../otpimpl/sha1hash.h"
#include "testutils.h"

#include <QDebug>

#include <sstream>
#include <iomanip>

// Test vectors taken from RFC 2202 at https://tools.ietf.org/html/rfc2202

void HmacSha1Tests::hmacTestCase1()
{
    char *key;
    size_t keyLength = 20;
    unsigned char expectedDigest[20] = { 0xb6, 0x17, 0x31, 0x86, 0x55, 0x05, 0x72, 0x64, 0xe2, 0x8b, 0xc0, 0xb6, 0xfb, 0x37, 0x8c, 0x8e, 0xf1, 0x46, 0xbe, 0x00 };
    char *data;
    size_t dataLen;
    Hmac dohmac(new Sha1Hash(), true);
    unsigned char *result;
    size_t resultSize;

    // Build the key, which is 20 bytes of 0x0b for test 1.
    key = reinterpret_cast<char *>(calloc(1, keyLength));
    for (size_t i = 0; i < keyLength; i++) {
        key[i] = 0x0b;
    }

    data = strdup("Hi There");
    dataLen = strlen(data);

    // Calculate the HMAC.
    result = dohmac.calculate(reinterpret_cast<unsigned char *>(key), keyLength, reinterpret_cast<unsigned char *>(data), dataLen, resultSize);

    // Make sure the length is what we expect.
    QCOMPARE(resultSize, static_cast<size_t>(20));
    if (result != nullptr) {
        qDebug("Expected : %s", TestUtils::binaryToString(expectedDigest, 20).c_str());
        qDebug("Got      : %s", TestUtils::binaryToString(result, resultSize).c_str());
        QVERIFY(memcmp(result, expectedDigest, 20) == 0);
    }
}

void HmacSha1Tests::hmacTestCase2()
{
    char *key;
    size_t keyLength = 4;
    unsigned char expectedDigest[20] = { 0xef, 0xfc, 0xdf, 0x6a, 0xe5, 0xeb, 0x2f, 0xa2, 0xd2, 0x74, 0x16, 0xd5, 0xf1, 0x84, 0xdf, 0x9c, 0x25, 0x9a, 0x7c, 0x79 };
    char *data;
    size_t dataLen;
    Hmac dohmac(new Sha1Hash(), true);
    unsigned char *result;
    size_t resultSize;

    // Key for test 2.
    key = strdup("Jefe");

    data = strdup("what do ya want for nothing?");
    dataLen = strlen(data);

    // Calculate the HMAC.
    result = dohmac.calculate(reinterpret_cast<unsigned char *>(key), keyLength, reinterpret_cast<unsigned char *>(data), dataLen, resultSize);

    // Make sure the length is what we expect.
    QCOMPARE(resultSize, static_cast<size_t>(20));
    if (result != nullptr) {
        qDebug("Expected : %s", TestUtils::binaryToString(expectedDigest, 20).c_str());
        qDebug("Got      : %s", TestUtils::binaryToString(result, resultSize).c_str());
        QVERIFY(memcmp(result, expectedDigest, 20) == 0);
    }
}

void HmacSha1Tests::hmacTestCase3()
{
    char *key;
    size_t keyLength = 20;
    unsigned char expectedDigest[20] = { 0x12, 0x5d, 0x73, 0x42, 0xb9, 0xac, 0x11, 0xcd, 0x91, 0xa3, 0x9a, 0xf4, 0x8a, 0xa1, 0x7b, 0x4f, 0x63, 0xf1, 0x75, 0xd3 };
    char *data;
    size_t dataLen;
    Hmac dohmac(new Sha1Hash(), true);
    unsigned char *result;
    size_t resultSize;

    // Build the key, which is 20 bytes of 0x0b for test 1.
    key = reinterpret_cast<char *>(calloc(1, keyLength));
    for (size_t i = 0; i < keyLength; i++) {
        key[i] = 0xaa;
    }

    data = static_cast<char *>(calloc(1, 50));
    for (size_t i = 0; i < 50; i++) {
        data[i] = 0xdd;
    }
    dataLen = 50;

    // Calculate the HMAC.
    result = dohmac.calculate(reinterpret_cast<unsigned char *>(key), keyLength, reinterpret_cast<unsigned char *>(data), dataLen, resultSize);

    // Make sure the length is what we expect.
    QCOMPARE(resultSize, static_cast<size_t>(20));
    if (result != nullptr) {
        qDebug("Expected : %s", TestUtils::binaryToString(expectedDigest, 20).c_str());
        qDebug("Got      : %s", TestUtils::binaryToString(result, resultSize).c_str());
        QVERIFY(memcmp(result, expectedDigest, 20) == 0);
    }
}

void HmacSha1Tests::hmacTestCase4()
{
    char key[25] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19 };
    size_t keyLength = 25;
    unsigned char expectedDigest[20] = { 0x4c, 0x90, 0x07, 0xf4, 0x02, 0x62, 0x50, 0xc6, 0xbc, 0x84, 0x14, 0xf9, 0xbf, 0x50, 0xc8, 0x6c, 0x2d, 0x72, 0x35, 0xda };
    char *data;
    size_t dataLen;
    Hmac dohmac(new Sha1Hash(), true);
    unsigned char *result;
    size_t resultSize;

    data = static_cast<char *>(calloc(1, 50));
    for (size_t i = 0; i < 50; i++) {
        data[i] = 0xcd;
    }
    dataLen = 50;

    // Calculate the HMAC.
    result = dohmac.calculate(reinterpret_cast<unsigned char *>(&key), keyLength, reinterpret_cast<unsigned char *>(data), dataLen, resultSize);

    // Make sure the length is what we expect.
    QCOMPARE(resultSize, static_cast<size_t>(20));
    if (result != nullptr) {
        qDebug("Expected : %s", TestUtils::binaryToString(expectedDigest, 20).c_str());
        qDebug("Got      : %s", TestUtils::binaryToString(result, resultSize).c_str());
        QVERIFY(memcmp(result, expectedDigest, 20) == 0);
    }
}

void HmacSha1Tests::hmacTestCase5()
{
    char *key;
    size_t keyLength = 20;
    unsigned char expectedDigest[20] = { 0x4c, 0x1a, 0x03, 0x42, 0x4b, 0x55, 0xe0, 0x7f, 0xe7, 0xf2, 0x7b, 0xe1, 0xd5, 0x8b, 0xb9, 0x32, 0x4a, 0x9a, 0x5a, 0x04 };
    char *data;
    size_t dataLen;
    Hmac dohmac(new Sha1Hash(), true);
    unsigned char *result;
    size_t resultSize;

    // Build the key, which is 20 bytes of 0x0c for test 5.
    key = reinterpret_cast<char *>(calloc(1, keyLength));
    for (size_t i = 0; i < keyLength; i++) {
        key[i] = static_cast<char>(0xa0c);
    }

    data = strdup("Test With Truncation");
    dataLen = strlen(data);

    // Calculate the HMAC.
    result = dohmac.calculate(reinterpret_cast<unsigned char *>(key), keyLength, reinterpret_cast<unsigned char *>(data), dataLen, resultSize);

    // Make sure the length is what we expect.
    QCOMPARE(resultSize, static_cast<size_t>(20));
    if (result != nullptr) {
        qDebug("Expected : %s", TestUtils::binaryToString(expectedDigest, 20).c_str());
        qDebug("Got      : %s", TestUtils::binaryToString(result, resultSize).c_str());
        QVERIFY(memcmp(result, expectedDigest, 20) == 0);
    }
}

void HmacSha1Tests::hmacTestCase6()
{
    char *key;
    size_t keyLength = 80;
    unsigned char expectedDigest[20] = { 0xaa, 0x4a, 0xe5, 0xe1, 0x52, 0x72, 0xd0, 0x0e, 0x95, 0x70, 0x56, 0x37, 0xce, 0x8a, 0x3b, 0x55, 0xed, 0x40, 0x21, 0x12 };
    char *data;
    size_t dataLen;
    Hmac dohmac(new Sha1Hash(), true);
    unsigned char *result;
    size_t resultSize;

    // Build the key, which is 80 bytes of 0xaa for test 6.
    key = reinterpret_cast<char *>(calloc(1, keyLength));
    for (size_t i = 0; i < keyLength; i++) {
        key[i] = static_cast<char>(0xaa);
    }

    data = strdup("Test Using Larger Than Block-Size Key - Hash Key First");
    dataLen = strlen(data);

    // Calculate the HMAC.
    result = dohmac.calculate(reinterpret_cast<unsigned char *>(key), keyLength, reinterpret_cast<unsigned char *>(data), dataLen, resultSize);

    // Make sure the length is what we expect.
    QCOMPARE(resultSize, static_cast<size_t>(20));
    if (result != nullptr) {
        qDebug("Expected : %s", TestUtils::binaryToString(expectedDigest, 20).c_str());
        qDebug("Got      : %s", TestUtils::binaryToString(result, resultSize).c_str());
        QVERIFY(memcmp(result, expectedDigest, 20) == 0);
    }
}

void HmacSha1Tests::hmacTestCase7()
{
    char *key;
    size_t keyLength = 80;
    unsigned char expectedDigest1[20] = { 0xe8, 0xe9, 0x9d, 0x0f, 0x45, 0x23, 0x7d, 0x78, 0x6d, 0x6b, 0xba, 0xa7, 0x96, 0x5c, 0x78, 0x08, 0xbb, 0xff, 0x1a, 0x91 };
    //unsigned char expectedDigest2[20] = { 0x4c, 0x1a, 0x03, 0x42, 0x4b, 0x55, 0xe0, 0x7f, 0xe7, 0xf2, 0x7b, 0xe1, 0xd5, 0x8b, 0xb9, 0x32, 0x4a, 0x9a, 0x5a, 0x04 };  // We don't calculate the 2nd block in this test.
    char *data;
    size_t dataLen;
    Hmac dohmac(new Sha1Hash(), true);
    unsigned char *result;
    size_t resultSize;

    // Build the key, which is 80 bytes of 0xaa for test 7.
    key = reinterpret_cast<char *>(calloc(1, keyLength));
    for (size_t i = 0; i < keyLength; i++) {
        key[i] = static_cast<char>(0xaa);
    }

    data = strdup("Test Using Larger Than Block-Size Key and Larger Than One Block-Size Data");
    dataLen = strlen(data);

    // Calculate the HMAC.
    result = dohmac.calculate(reinterpret_cast<unsigned char *>(key), keyLength, reinterpret_cast<unsigned char *>(data), dataLen, resultSize);

    // Make sure the length is what we expect.
    QCOMPARE(resultSize, static_cast<size_t>(20));
    if (result != nullptr) {
        qDebug("Expected : %s", TestUtils::binaryToString(expectedDigest1, 20).c_str());
        qDebug("Got      : %s", TestUtils::binaryToString(result, resultSize).c_str());
        QVERIFY(memcmp(result, expectedDigest1, 20) == 0);
    }
}
