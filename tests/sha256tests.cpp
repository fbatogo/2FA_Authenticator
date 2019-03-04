#include "sha256tests.h"

#include <QDebug>

extern "C" {
#include "otpimpl/sha2.h"
}
#include "testutils.h"

void Sha256Tests::sha256ImplTests()
{
    unsigned char abctest[] = { 0xba, 0x78, 0x16, 0xbf, 0x8f, 0x01, 0xcf, 0xea, 0x41, 0x41, 0x40, 0xde, 0x5d, 0xae, 0x22, 0x23, 0xb0, 0x03, 0x61, 0xa3, 0x96, 0x17, 0x7a, 0x9c, 0xb4, 0x10, 0xff, 0x61, 0xf2, 0x00, 0x15, 0xad };
    unsigned char result[32];
    sha256_ctx ctx;

    // Test string is "abc".
    sha256_init(&ctx);
    sha256_update(&ctx, (unsigned char *)"abc", 3);
    sha256_final(&ctx, (unsigned char *)&result);

    qDebug("Expected   : %s", TestUtils::binaryToString(abctest, 32).c_str());
    qDebug("Calculated : %s", TestUtils::binaryToString(result, 32).c_str());

    QVERIFY(memcmp(&result, &abctest, 32) == 0);
}
