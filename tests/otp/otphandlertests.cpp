#include "otphandlertests.h"

#include <QtTest>

#include "otp/otphandler.h"

void OtpHandlerTests::calculateOtpForKeyEntryTest()
{
    OtpHandler handler;
    KeyEntry kEntry;

    // Try to calculate using a null pointer.
    handler.calculateOtpForKeyEntry(nullptr);

    // Attempt to use an invalid key entry.
    kEntry.clear();
    handler.calculateOtpForKeyEntry(&kEntry);

    // Create an HOTP entry to work with.
    kEntry.clear();
    kEntry.setIdentifier("Test Key");
    kEntry.setIssuer("Test Issuer");
    kEntry.setSecret("3132333435363738393031323334353637383930");
    kEntry.setKeyType(KEYENTRY_KEYTYPE_HEX);
    kEntry.setOtpType(KEYENTRY_OTPTYPE_HOTP);
    kEntry.setTimeStep(30);
    kEntry.setAlgorithm(KEYENTRY_ALG_SHA1);
    kEntry.setCodeValid(true);
    kEntry.setStartTime(0);
    kEntry.setTimeOffset(0);
    kEntry.setHotpCounter(9);
    kEntry.setOutNumberCount(6);

    // Calculate the HOTP code for SHA1.
    handler.calculateOtpForKeyEntry(&kEntry);

    // Then, calculate it with a 256-bit SHA hash.
    kEntry.setAlgorithm(KEYENTRY_ALG_SHA256);

    // Calculate the HOTP code for SHA256.
    handler.calculateOtpForKeyEntry(&kEntry);

    // Then, calculate it with a 512-bit SHA hash.
    kEntry.setAlgorithm(KEYENTRY_ALG_SHA512);

    // Calculate the HOTP code for SHA512.
    handler.calculateOtpForKeyEntry(&kEntry);
}

// Run the test with a base32 encoded value.
void OtpHandlerTests::calculateOtpForKeyEntryTest2()
{
    OtpHandler handler;
    KeyEntry kEntry;

    // Create an HOTP entry to work with.
    kEntry.clear();
    kEntry.setIdentifier("Test Key");
    kEntry.setIssuer("Test Issuer");
    kEntry.setSecret("ORSXG5DWMFWHKZI=");
    kEntry.setKeyType(KEYENTRY_KEYTYPE_BASE32);
    kEntry.setOtpType(KEYENTRY_OTPTYPE_HOTP);
    kEntry.setTimeStep(30);
    kEntry.setAlgorithm(KEYENTRY_ALG_SHA1);
    kEntry.setCodeValid(true);
    kEntry.setStartTime(0);
    kEntry.setTimeOffset(0);
    kEntry.setHotpCounter(9);
    kEntry.setOutNumberCount(6);

    // Calculate the HOTP code for SHA1.
    handler.calculateOtpForKeyEntry(&kEntry);

    // Then, calculate it with a 256-bit SHA hash.
    kEntry.setAlgorithm(KEYENTRY_ALG_SHA256);

    // Calculate the HOTP code for SHA256.
    handler.calculateOtpForKeyEntry(&kEntry);

    // Then, calculate it with a 512-bit SHA hash.
    kEntry.setAlgorithm(KEYENTRY_ALG_SHA512);

    // Calculate the HOTP code for SHA512.
    handler.calculateOtpForKeyEntry(&kEntry);

    // Try to calculate with an invalid algorithm.
    kEntry.setAlgorithm(1234);
    handler.calculateOtpForKeyEntry(&kEntry);
}

