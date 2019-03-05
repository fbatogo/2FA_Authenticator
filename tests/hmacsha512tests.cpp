#include "hmacsha512tests.h"

#include "../otpimpl/hmac.h"
#include "../otpimpl/sha512hash.h"
#include "testutils.h"

#include <QDebug>

#include <sstream>
#include <iomanip>

void HmacSha512Tests::hmacTestCase1()
{
    char *key;
    size_t keyLength = 20;
    unsigned char expectedDigest[64] = { 0x87, 0xaa, 0x7c, 0xde, 0xa5, 0xef, 0x61, 0x9d, 0x4f, 0xf0, 0xb4, 0x24, 0x1a, 0x1d, 0x6c, 0xb0, 0x23, 0x79, 0xf4, 0xe2, 0xce, 0x4e, 0xc2, 0x78, 0x7a, 0xd0, 0xb3, 0x05, 0x45, 0xe1, 0x7c, 0xde, 0xda, 0xa8, 0x33, 0xb7, 0xd6, 0xb8, 0xa7, 0x02, 0x03, 0x8b, 0x27, 0x4e, 0xae, 0xa3, 0xf4, 0xe4, 0xbe, 0x9d, 0x91, 0x4e, 0xeb, 0x61, 0xf1, 0x70, 0x2e, 0x69, 0x6c, 0x20, 0x3a, 0x12, 0x68, 0x54 };
    char *data;
    size_t dataLen;
    Hmac dohmac(new Sha512Hash(), true);
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
    QCOMPARE(resultSize, static_cast<size_t>(64));
    if (result != nullptr) {
        qDebug("Expected : %s", TestUtils::binaryToString(expectedDigest, 64).c_str());
        qDebug("Got      : %s", TestUtils::binaryToString(result, resultSize).c_str());
        QVERIFY(memcmp(result, expectedDigest, 64) == 0);
    }

    free(key);
    free(data);
}

void HmacSha512Tests::hmacTestCase2()
{
    char *key;
    size_t keyLength = 4;
    unsigned char expectedDigest[64] = { 0x16, 0x4b, 0x7a, 0x7b, 0xfc, 0xf8, 0x19, 0xe2, 0xe3, 0x95, 0xfb, 0xe7, 0x3b, 0x56, 0xe0, 0xa3, 0x87, 0xbd, 0x64, 0x22, 0x2e, 0x83, 0x1f, 0xd6, 0x10, 0x27, 0x0c, 0xd7, 0xea, 0x25, 0x05, 0x54, 0x97, 0x58, 0xbf, 0x75, 0xc0, 0x5a, 0x99, 0x4a, 0x6d, 0x03, 0x4f, 0x65, 0xf8, 0xf0, 0xe6, 0xfd, 0xca, 0xea, 0xb1, 0xa3, 0x4d, 0x4a, 0x6b, 0x4b, 0x63, 0x6e, 0x07, 0x0a, 0x38, 0xbc, 0xe7, 0x37 };
    char *data;
    size_t dataLen;
    Hmac dohmac(new Sha512Hash(), true);
    unsigned char *result;
    size_t resultSize;

    // Key for test 2.
    key = strdup("Jefe");

    data = strdup("what do ya want for nothing?");
    dataLen = strlen(data);

    // Calculate the HMAC.
    result = dohmac.calculate(reinterpret_cast<unsigned char *>(key), keyLength, reinterpret_cast<unsigned char *>(data), dataLen, resultSize);

    // Make sure the length is what we expect.
    QCOMPARE(resultSize, static_cast<size_t>(64));
    if (result != nullptr) {
        qDebug("Expected : %s", TestUtils::binaryToString(expectedDigest, 64).c_str());
        qDebug("Got      : %s", TestUtils::binaryToString(result, resultSize).c_str());
        QVERIFY(memcmp(result, expectedDigest, 64) == 0);
    }

    free(key);
    free(data);
}

void HmacSha512Tests::hmacTestCase3()
{
    char *key;
    size_t keyLength = 20;
    unsigned char expectedDigest[20] = { 0x12, 0x5d, 0x73, 0x42, 0xb9, 0xac, 0x11, 0xcd, 0x91, 0xa3, 0x9a, 0xf4, 0x8a, 0xa1, 0x7b, 0x4f, 0x63, 0xf1, 0x75, 0xd3 };
    char *data;
    size_t dataLen;
    Hmac dohmac(new Sha512Hash(), true);
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

    free(key);
    free(data);
}

void HmacSha512Tests::hmacTestCase4()
{
    char key[25] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19 };
    size_t keyLength = 25;
    unsigned char expectedDigest[20] = { 0x4c, 0x90, 0x07, 0xf4, 0x02, 0x62, 0x50, 0xc6, 0xbc, 0x84, 0x14, 0xf9, 0xbf, 0x50, 0xc8, 0x6c, 0x2d, 0x72, 0x35, 0xda };
    char *data;
    size_t dataLen;
    Hmac dohmac(new Sha512Hash(), true);
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

    free(data);
}

void HmacSha512Tests::hmacTestCase5()
{
    char *key;
    size_t keyLength = 20;
    unsigned char expectedDigest[20] = { 0x4c, 0x1a, 0x03, 0x42, 0x4b, 0x55, 0xe0, 0x7f, 0xe7, 0xf2, 0x7b, 0xe1, 0xd5, 0x8b, 0xb9, 0x32, 0x4a, 0x9a, 0x5a, 0x04 };
    char *data;
    size_t dataLen;
    Hmac dohmac(new Sha512Hash(), true);
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

    free(key);
    free(data);
}

void HmacSha512Tests::hmacTestCase6()
{
    char *key;
    size_t keyLength = 80;
    unsigned char expectedDigest[20] = { 0xaa, 0x4a, 0xe5, 0xe1, 0x52, 0x72, 0xd0, 0x0e, 0x95, 0x70, 0x56, 0x37, 0xce, 0x8a, 0x3b, 0x55, 0xed, 0x40, 0x21, 0x12 };
    char *data;
    size_t dataLen;
    Hmac dohmac(new Sha512Hash(), true);
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

    free(key);
    free(data);
}

void HmacSha512Tests::hmacTestCase7()
{
    char *key;
    size_t keyLength = 80;
    unsigned char expectedDigest1[20] = { 0xe8, 0xe9, 0x9d, 0x0f, 0x45, 0x23, 0x7d, 0x78, 0x6d, 0x6b, 0xba, 0xa7, 0x96, 0x5c, 0x78, 0x08, 0xbb, 0xff, 0x1a, 0x91 };
    //unsigned char expectedDigest2[20] = { 0x4c, 0x1a, 0x03, 0x42, 0x4b, 0x55, 0xe0, 0x7f, 0xe7, 0xf2, 0x7b, 0xe1, 0xd5, 0x8b, 0xb9, 0x32, 0x4a, 0x9a, 0x5a, 0x04 };  // We don't calculate the 2nd block in this test.
    char *data;
    size_t dataLen;
    Hmac dohmac(new Sha512Hash(), true);
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

    free(key);
    free(data);
}
