#include "hmacsha1tests.h"

#include "../otpimpl/hmac.h"
#include "../otpimpl/sha1hash.h"

#include <sstream>
#include <iomanip>
#include <iostream>

// Test vectors taken from RFC 2202 at https://tools.ietf.org/html/rfc2202

void HmacSha1Tests::hmacTestCase1()
{
    char *key;
    size_t keyLength = 20;
    unsigned char expectedDigest[20] = { 0xb6, 0x17, 0x31, 0x86, 0x55, 0x05, 0x72, 0x64, 0xe2, 0x8b, 0xc0, 0xb6, 0xfb, 0x37, 0x8c, 0x8e, 0xf1, 0x46, 0xbe, 0x00 };
    char *data;
    size_t dataLen;
    //Sha1Hash hashmethod;
    Hmac dohmac(new Sha1Hash(), true);
    unsigned char *result;
    size_t resultSize;

    // Build the key, which is 20 bytes of 0x0b for test 1.
    key = reinterpret_cast<char *>(calloc(1, keyLength));
    for (size_t i = 0; i < keyLength; i++) {
        key[i] = 0x0b;
    }

    std::cout << "Key generated!\n";
    data = strdup("Hi There");
    dataLen = strlen(data);

    std::cout << "Calculate HMAC.\n";
    // Calculate the HMAC.
    result = dohmac.calculate(reinterpret_cast<unsigned char *>(&key), keyLength, reinterpret_cast<unsigned char *>(data), dataLen, resultSize);

    // Make sure the length is what we expect.
    QCOMPARE(resultSize, 20);
    if (result != nullptr) {
        std::cout << "Expected : " << binaryToString(expectedDigest, 20) << "\n";
        std::cout << "Got      : " << binaryToString(result, resultSize) << "\n";
        QVERIFY(memcmp(result, expectedDigest, 20) == 0);
    }
}
/*
void HmacSha1Tests::hmacTestCase2()
{
    test_case =     2
    key =           "Jefe"
    key_len =       4
    data =          "what do ya want for nothing?"
    data_len =      28
    digest =        0xeffcdf6ae5eb2fa2d27416d5f184df9c259a7c79

}

void HmacSha1Tests::hmacTestCase3()
{
    test_case =     3
    key =           0xaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
    key_len =       20
    data =          0xdd repeated 50 times
    data_len =      50
    digest =        0x125d7342b9ac11cd91a39af48aa17b4f63f175d3

}

void HmacSha1Tests::hmacTestCase4()
{
    test_case =     4
    key =           0x0102030405060708090a0b0c0d0e0f10111213141516171819
    key_len =       25
    data =          0xcd repeated 50 times
    data_len =      50
    digest =        0x4c9007f4026250c6bc8414f9bf50c86c2d7235da

}

void HmacSha1Tests::hmacTestCase5()
{
    test_case =     5
    key =           0x0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c
    key_len =       20
    data =          "Test With Truncation"
    data_len =      20
    digest =        0x4c1a03424b55e07fe7f27be1d58bb9324a9a5a04
    digest-96 =     0x4c1a03424b55e07fe7f27be1

}

void HmacSha1Tests::hmacTestCase6()
{
    test_case =     6
    key =           0xaa repeated 80 times
    key_len =       80
    data =          "Test Using Larger Than Block-Size Key - Hash Key First"
    data_len =      54
    digest =        0xaa4ae5e15272d00e95705637ce8a3b55ed402112

}

void HmacSha1Tests::hmacTestCase7()
{
    test_case =     7
    key =           0xaa repeated 80 times
    key_len =       80
    data =          "Test Using Larger Than Block-Size Key and Larger
                    Than One Block-Size Data"
    data_len =      73
    digest =        0xe8e99d0f45237d786d6bbaa7965c7808bbff1a91
    data_len =      20
    digest =        0x4c1a03424b55e07fe7f27be1d58bb9324a9a5a04
    digest-96 =     0x4c1a03424b55e07fe7f27be1


}
*/

std::string HmacSha1Tests::binaryToString(unsigned char *bytes, size_t bytesLength)
{
    std::stringstream result;

    result.clear();

    for (size_t i = 0; i < bytesLength; i++) {
        result << std::setfill('0') << std::setw(2) << std::hex << (int)bytes[i] << " ";
    }

    return result.str();
}
