#include "hmacsha256tests.h"

#include "../otpimpl/hmac.h"
#include "../otpimpl/sha256hash.h"
#include "testutils.h"

#include <QDebug>

#include <sstream>
#include <iomanip>

void HmacSha256Tests::hmacTestCase1()
{
    char *key;
    size_t keyLength = 20;
    unsigned char expectedDigest[32] = { 0xb0, 0x34, 0x4c, 0x61, 0xd8, 0xdb, 0x38, 0x53, 0x5c, 0xa8, 0xaf, 0xce, 0xaf, 0x0b, 0xf1, 0x2b, 0x88, 0x1d, 0xc2, 0x00, 0xc9, 0x83, 0x3d, 0xa7, 0x26, 0xe9, 0x37, 0x6c, 0x2e, 0x32, 0xcf, 0xf7 };
    char *data;
    size_t dataLen;
    Hmac dohmac(new Sha256Hash(), true);
    unsigned char *result;
    size_t resultSize;

    // Build the key, which is 20 bytes of 0x0b for test 1.
    key = reinterpret_cast<char *>(calloc(1, keyLength));
    for (size_t i = 0; i < keyLength; i++) {
        key[i] = 0x0b;
    }

    data = _strdup("Hi There");
    dataLen = strlen(data);

    // Calculate the HMAC.
    result = dohmac.calculate(reinterpret_cast<unsigned char *>(key), keyLength, reinterpret_cast<unsigned char *>(data), dataLen, resultSize);

    // Make sure the length is what we expect.
    QCOMPARE(resultSize, static_cast<size_t>(32));
    if (result != nullptr) {
        qDebug("Expected : %s", TestUtils::binaryToString(expectedDigest, 32).c_str());
        qDebug("Got      : %s", TestUtils::binaryToString(result, resultSize).c_str());
        QVERIFY(memcmp(result, expectedDigest, 32) == 0);
    }

    free(key);
    free(data);
}

void HmacSha256Tests::hmacTestCase2()
{
    char *key;
    size_t keyLength = 4;
    unsigned char expectedDigest[32] = { 0x5b, 0xdc, 0xc1, 0x46, 0xbf, 0x60, 0x75, 0x4e, 0x6a, 0x04, 0x24, 0x26, 0x08, 0x95, 0x75, 0xc7, 0x5a, 0x00, 0x3f, 0x08, 0x9d, 0x27, 0x39, 0x83, 0x9d, 0xec, 0x58, 0xb9, 0x64, 0xec, 0x38, 0x43 };
    char *data;
    size_t dataLen;
    Hmac dohmac(new Sha256Hash(), true);
    unsigned char *result;
    size_t resultSize;

    // Key for test 2.
    key = _strdup("Jefe");

    data = _strdup("what do ya want for nothing?");
    dataLen = strlen(data);

    // Calculate the HMAC.
    result = dohmac.calculate(reinterpret_cast<unsigned char *>(key), keyLength, reinterpret_cast<unsigned char *>(data), dataLen, resultSize);

    // Make sure the length is what we expect.
    QCOMPARE(resultSize, static_cast<size_t>(32));
    if (result != nullptr) {
        qDebug("Expected : %s", TestUtils::binaryToString(expectedDigest, 32).c_str());
        qDebug("Got      : %s", TestUtils::binaryToString(result, resultSize).c_str());
        QVERIFY(memcmp(result, expectedDigest, 32) == 0);
    }

    free(key);
    free(data);
}

void HmacSha256Tests::hmacTestCase3()
{
    char *key;
    size_t keyLength = 20;
    unsigned char expectedDigest[32] = { 0x77, 0x3e, 0xa9, 0x1e, 0x36, 0x80, 0x0e, 0x46, 0x85, 0x4d, 0xb8, 0xeb, 0xd0, 0x91, 0x81, 0xa7, 0x29, 0x59, 0x09, 0x8b, 0x3e, 0xf8, 0xc1, 0x22, 0xd9, 0x63, 0x55, 0x14, 0xce, 0xd5, 0x65, 0xfe };
    char *data;
    size_t dataLen;
    Hmac dohmac(new Sha256Hash(), true);
    unsigned char *result;
    size_t resultSize;

    // Build the key, which is 20 bytes of 0x0b for test 1.
    key = reinterpret_cast<char *>(calloc(1, keyLength));
    for (size_t i = 0; i < keyLength; i++) {
        key[i] = (char)0xaa;
    }

    data = static_cast<char *>(calloc(1, 50));
    for (size_t i = 0; i < 50; i++) {
        data[i] = (char)0xdd;
    }
    dataLen = 50;

    // Calculate the HMAC.
    result = dohmac.calculate(reinterpret_cast<unsigned char *>(key), keyLength, reinterpret_cast<unsigned char *>(data), dataLen, resultSize);

    // Make sure the length is what we expect.
    QCOMPARE(resultSize, static_cast<size_t>(32));
    if (result != nullptr) {
        qDebug("Expected : %s", TestUtils::binaryToString(expectedDigest, 32).c_str());
        qDebug("Got      : %s", TestUtils::binaryToString(result, resultSize).c_str());
        QVERIFY(memcmp(result, expectedDigest, 32) == 0);
    }

    free(key);
    free(data);
}

void HmacSha256Tests::hmacTestCase4()
{
    char key[25] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19 };
    size_t keyLength = 25;
    unsigned char expectedDigest[32] = { 0x82, 0x55, 0x8a, 0x38, 0x9a, 0x44, 0x3c, 0x0e, 0xa4, 0xcc, 0x81, 0x98, 0x99, 0xf2, 0x08, 0x3a, 0x85, 0xf0, 0xfa, 0xa3, 0xe5, 0x78, 0xf8, 0x07, 0x7a, 0x2e, 0x3f, 0xf4, 0x67, 0x29, 0x66, 0x5b };
    char *data;
    size_t dataLen;
    Hmac dohmac(new Sha256Hash(), true);
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
    QCOMPARE(resultSize, static_cast<size_t>(32));
    if (result != nullptr) {
        qDebug("Expected : %s", TestUtils::binaryToString(expectedDigest, 32).c_str());
        qDebug("Got      : %s", TestUtils::binaryToString(result, resultSize).c_str());
        QVERIFY(memcmp(result, expectedDigest, 32) == 0);
    }

    free(data);
}

void HmacSha256Tests::hmacTestCase5()
{
    char *key;
    size_t keyLength = 20;
    unsigned char expectedDigest[16] = { 0xa3, 0xb6, 0x16, 0x74, 0x73, 0x10, 0x0e, 0xe0, 0x6e, 0x0c, 0x79, 0x6c, 0x29, 0x55, 0x55, 0x2b };
    char *data;
    size_t dataLen;
    Hmac dohmac(new Sha256Hash(), true);
    unsigned char *result;
    size_t resultSize;

    // Build the key, which is 20 bytes of 0x0c for test 5.
    key = reinterpret_cast<char *>(calloc(1, keyLength));
    for (size_t i = 0; i < keyLength; i++) {
        key[i] = static_cast<char>(0xa0c);
    }

    data = _strdup("Test With Truncation");
    dataLen = strlen(data);

    // Calculate the HMAC.
    result = dohmac.calculate(reinterpret_cast<unsigned char *>(key), keyLength, reinterpret_cast<unsigned char *>(data), dataLen, resultSize);

    // Make sure the length is what we expect.  (Before truncation)
    QCOMPARE(resultSize, static_cast<size_t>(32));
    if (result != nullptr) {
        qDebug("Expected : %s", TestUtils::binaryToString(expectedDigest, 16).c_str());
        qDebug("Got      : %s", TestUtils::binaryToString(result, resultSize).c_str());
        QVERIFY(memcmp(result, expectedDigest, 16) == 0);
    }

    free(key);
    free(data);
}

void HmacSha256Tests::hmacTestCase6()
{
    char *key;
    size_t keyLength = 80;
    unsigned char expectedDigest[32] = { 0x60, 0xe4, 0x31, 0x59, 0x1e, 0xe0, 0xb6, 0x7f, 0x0d, 0x8a, 0x26, 0xaa, 0xcb, 0xf5, 0xb7, 0x7f, 0x8e, 0x0b, 0xc6, 0x21, 0x37, 0x28, 0xc5, 0x14, 0x05, 0x46, 0x04, 0x0f, 0x0e, 0xe3, 0x7f, 0x54 };
    char *data;
    size_t dataLen;
    Hmac dohmac(new Sha256Hash(), true);
    unsigned char *result;
    size_t resultSize;

    // Build the key, which is 80 bytes of 0xaa for test 6.
    key = reinterpret_cast<char *>(calloc(1, keyLength));
    for (size_t i = 0; i < keyLength; i++) {
        key[i] = static_cast<char>(0xaa);
    }

    data = _strdup("Test Using Larger Than Block-Size Key - Hash Key First");
    dataLen = strlen(data);

    // Calculate the HMAC.
    result = dohmac.calculate(reinterpret_cast<unsigned char *>(key), keyLength, reinterpret_cast<unsigned char *>(data), dataLen, resultSize);

    // Make sure the length is what we expect.
    QCOMPARE(resultSize, static_cast<size_t>(32));
    if (result != nullptr) {
        qDebug("Expected : %s", TestUtils::binaryToString(expectedDigest, 32).c_str());
        qDebug("Got      : %s", TestUtils::binaryToString(result, resultSize).c_str());
        QVERIFY(memcmp(result, expectedDigest, 32) == 0);
    }

    free(key);
    free(data);
}

void HmacSha256Tests::hmacTestCase7()
{
    char *key;
    size_t keyLength = 80;
    unsigned char expectedDigest1[32] = { 0x9b, 0x09, 0xff, 0xa7, 0x1b, 0x94, 0x2f, 0xcb, 0x27, 0x63, 0x5f, 0xbc, 0xd5, 0xb0, 0xe9, 0x44, 0xbf, 0xdc, 0x63, 0x64, 0x4f, 0x07, 0x13, 0x93, 0x8a, 0x7f, 0x51, 0x53, 0x5c, 0x3a, 0x35, 0xe2 };
    //unsigned char expectedDigest2[20] = { 0x4c, 0x1a, 0x03, 0x42, 0x4b, 0x55, 0xe0, 0x7f, 0xe7, 0xf2, 0x7b, 0xe1, 0xd5, 0x8b, 0xb9, 0x32, 0x4a, 0x9a, 0x5a, 0x04 };  // We don't calculate the 2nd block in this test.
    char *data;
    size_t dataLen;
    Hmac dohmac(new Sha256Hash(), true);
    unsigned char *result;
    size_t resultSize;

    // Build the key, which is 80 bytes of 0xaa for test 7.
    key = reinterpret_cast<char *>(calloc(1, keyLength));
    for (size_t i = 0; i < keyLength; i++) {
        key[i] = static_cast<char>(0xaa);
    }

    data = _strdup("Test Using Larger Than Block-Size Key and Larger Than One Block-Size Data");
    dataLen = strlen(data);

    // Calculate the HMAC.
    result = dohmac.calculate(reinterpret_cast<unsigned char *>(key), keyLength, reinterpret_cast<unsigned char *>(data), dataLen, resultSize);

    // Make sure the length is what we expect.
    QCOMPARE(resultSize, static_cast<size_t>(32));
    if (result != nullptr) {
        qDebug("Expected : %s", TestUtils::binaryToString(expectedDigest1, 32).c_str());
        qDebug("Got      : %s", TestUtils::binaryToString(result, resultSize).c_str());
        QVERIFY(memcmp(result, expectedDigest1, 32) == 0);
    }

    free(key);
    free(data);
}
