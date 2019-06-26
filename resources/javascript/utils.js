.pragma library

.import Rollin.Logger 1.0 as Logger

// Convert the secret encoding type of a string to the integer value used in the C++
// code.
function secretTypeToInt(secretEncoding) {
    if (secretEncoding === "HEX") {
        return 0;
    } else if (secretEncoding === "Base32") {
        return 1;
    }

    Logger.Log.logError("Unknown/unexpected secret encoding type : " + secretEncoding);

    return -1;
}

// Convert the secret coding from the integer value used in the C++ code to a string.
function secretTypeIntToString(secretEncodingInt) {
    switch (secretEncodingInt) {
    case 0:
        // HEX
        return "HEX";

    case 1:
        // Base32
        return "Base32";
    }

    Logger.Log.logError("Unknown/unexpected secret encoding type : " + secretEncodingInt);

    return "<UNKNOWN>";
}

// Convert the OTP type to an integer used to identify the OTP type in the C++
// code.
function otpTypeToInt(encodingType) {
    if (encodingType === "TOTP") {
        return 0;
    } else if (encodingType === "HOTP") {
        return 1;
    }

    Logger.Log.logError("Unknown/unexpected encoding type : " + encodingType);

    return -1;
}

// Convert the OTP type string to the int value used to represent it in the
// C++ code.
function otpTypeIntToString(otpTypeInt) {
    switch (otpTypeInt) {
    case 0:
        // TOTP
        return "TOTP";

    case 1:
        // HOTP
        return "HOTP";
    }

    Logger.Log.logError("Unknown/unexpected OTP type : " + otpTypeInt);

    return "<UNKNOWN>";
}

// Convert a string that indiciates one of the hashing algorithms we can use, to the
// integer value used in the C++ code.
function hashAlgToInt(hashAlg) {
    if (hashAlg === "SHA1") {
        return 0;
    } else if (hashAlg === "SHA256") {
        return 1;
    } else if (hashAlg === "SHA512") {
        return 2;
    }

    Logger.Log.logError("Unknown/unexpected hash algorithm of '" + hashAlg + "'!");

    return -1;
}

// Convert an integer value to the hash algorithm string to display.
function hashAlgIntToString(hashAlg) {
    switch (hashAlg) {
    case 0:
        return "SHA1";

    case 1:
        return "SHA256";

    case 2:
        return "SHA512";
    }

    Logger.Log.logError("Unknown/unexpected hash algorithm int : " + hashAlg);

    return "<UNKNOWN>";
}
