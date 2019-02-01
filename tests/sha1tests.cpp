#include "sha1tests.h"

#include <cstring>
#include "../otpimpl/sha1hash.h"
#include "../logger.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include "testutils.h"

// We only run two test vectors here, because the base SHA1 code is already NIST certified.
void Sha1Tests::sha1Tests1()
{
    char *toTest;
    unsigned char emptyStringResult[20] = {0xda, 0x39, 0xa3, 0xee, 0x5e, 0x6b, 0x4b, 0x0d, 0x32, 0x55, 0xbf, 0xef, 0x95, 0x60, 0x18, 0x90, 0xaf, 0xd8, 0x07, 0x09};
    unsigned char *result;
    Sha1Hash hashObj;

    // Calculate the empty string test.
    toTest = strdup("");
    result = hashObj.hash(reinterpret_cast<unsigned char *>(toTest), strlen(toTest));
    QVERIFY(memcmp((void *)result, (void *)&emptyStringResult, 20) == 0);

    // Clean up.
    free(toTest);
    toTest = nullptr;
}

void Sha1Tests::sha1Tests2()
{
    char *toTest;
    unsigned char string896bits[20] = {0xa4, 0x9b, 0x24, 0x46, 0xa0, 0x2c, 0x64, 0x5b, 0xf4, 0x19, 0xf9, 0x95, 0xb6, 0x70, 0x91, 0x25, 0x3a, 0x04, 0xa2, 0x59};
    unsigned char *result;
    Sha1Hash hashObj;

    // Calculate 896 bit string test.
    toTest = strdup("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu");
    result = hashObj.hash(reinterpret_cast<unsigned char *>(toTest), strlen(toTest));
    std::cout << "Calculated hash : " << TestUtils::binaryToString(result, 20) << "\n";
    std::cout << "Expected hash   : " << TestUtils::binaryToString(string896bits, 20) << "\n";
    QVERIFY(memcmp((void *)result, (void *)&string896bits, 20) == 0);

    // Clean up.
    free(toTest);
    toTest = nullptr;
}
