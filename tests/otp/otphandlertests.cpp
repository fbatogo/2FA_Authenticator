#include "otphandlertests.h"

#include <QtTest>

#include "otp/otphandler.h"

void OtpHandlerTests::calculateOtpForKeyEntry()
{
    OtpHandler handler;
    KeyEntry kEntry;

    // Try to calculate using a null pointer.
    handler.calculateOtpForKeyEntry(nullptr);

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

void OtpHandlerTests::calculateTotpForKeyEntry()
{
    // XXX
}
