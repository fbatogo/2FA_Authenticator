#include "hexdecoder.h"

#include <cctype>
#include <logger.h>
#include <sstream>

HexDecoder::HexDecoder()
{

}

/**
 * @brief HexDecoder::decode - Decode a hex string that may contain spaces, colons, dashes, periods, or a 0x prefix
 *      between the octets.
 *
 * @param hexData - A string that contains the string representation of the hex we want to convert.
 *
 * @return ByteArray containing the decoded value.  On error, the ByteArray will be empty.
 */
ByteArray HexDecoder::decode(const ByteArray &hexData)
{
    std::string smashed;
    ByteArray result;
    size_t resultSize;

    // If the string is empty, return nullptr.
    if (hexData.empty()) {
        return ByteArray();
    }

    // Start clean.
    result.clear();

    // Need to replace the spaces, colons, dashes, or periods in the string to get a single, undecorated, hex
    // string.
    smashed = cleanup(hexData.toString());

    // We should now have a string that is all hex characters.  So, the length should be divisible by 2.  Make
    // sure it is.
    if ((smashed.length() % 2) != 0) {
        LOG_ERROR("HEX string to decode wasn't an even number of characters!");
        return ByteArray();
    }

    // Calculate our result length.
    resultSize = (smashed.length() / 2);

    // Have our result buffer allocate extra space to speed things up.
    result.setExtraAllocation((resultSize + 2));

    // Iterate the string passing 2 characters at a time in to decodeOneByte, and stashing the
    // result in our return buffer.
    for (size_t i = 0; i < resultSize; i++) {
        result.append(decodeOneByte(smashed.substr((i * 2), 2)));
    }

    return result;
}

/**
 * @brief HexDecoder::isHexEncoded - Verify that the string provided appears to be a
 *      hex encoded string.
 *
 * @param toTest - The string that we want to test the encoding on.
 *
 * @return true if the string appears to be hex encoded.  false otherwise.
 */
bool HexDecoder::isHexEncoded(const std::string &toTest)
{
    QString temp;
    std::string toValidate;

    if (toTest.empty()) {
        LOG_ERROR("The string to validate for hex encoding was empty!");
        return false;
    }

    // Strip any excess, but allowed, characters from the string.
    toValidate = cleanup(toTest);

    // The string length needs to be evenly divisible by 2.
    if ((toValidate.length() % 2) != 0) {
        LOG_ERROR("The string provided isn't hex encoded.  The length is incorrect!");
        return false;
    }

    // And, each character needs to be one of the characters allowed in the
    // array above.
    for (size_t i = 0; i < toValidate.length(); i++) {
        if (((toValidate.at(i) < '0') || (toValidate.at(i) > '9')) &&
                ((toValidate.at(i) < 'A') || (toValidate.at(i) > 'F'))) {
            // The character is invalid.
            temp.clear();
            temp = toValidate.at(i);
            LOG_DEBUG("The character '" + temp + "' at index " + QString::number(i) + " is not a valid hex encoded character!");
            return false;
        }
    }

    // If we get here, then the string appears to be a valid hex string.
    return true;
}

/**
 * @brief HexDecoder::decodeOneByte - Decode a string containing 2 characters to an unsigned char that represents
 *      the binary of those two characters.
 *
 * @param oneByte - A string, containing only 2 characters, that we want to convert to an unsigned char value.
 *
 * @return unsigned char version of the 2 character string provided.  On error, an error will be logged, and
 *      0x00 will be returned.
 */
unsigned char HexDecoder::decodeOneByte(const std::string &oneByte)
{
    if (oneByte.length() != 2) {
        LOG_ERROR("The value to convert to hex wasn't one byte!  It was " + QString::number(oneByte.length()) + " byte(s) long!");
        return 0x00;
    }

    // Convert the two characters to a hex value.
    return static_cast<unsigned char>((decodeOneNibble(oneByte.at(0)) << 4) | (decodeOneNibble(oneByte.at(1))));
}

/**
 * @brief HexDecoder::decodeOneNibble - Convert a single character in to the binary version it represents.
 *
 * @param oneNibble - The character to convert to a nibble.
 *
 * @return unsigned char containing the converted nibble.  If the character isn't a valid hex character,
 *      an error will be logged, and 0xff will be returned.
 */
unsigned char HexDecoder::decodeOneNibble(char oneNibble)
{
    const char nibbles[] = "0123456789ABCDEF";
    char upperCharToSearch;
    std::stringstream errorMessage;

    // First, convert the character to upper case (in case if it A-F, and in lower case).
    upperCharToSearch = toupper(oneNibble);

    // Then, search for the character in our list of known characters.
    for (unsigned char i = 0; i < strlen(nibbles); i++) {
        if (upperCharToSearch == nibbles[i]) {
            return i;
        }
    }

    // If we didn't find it, log an error, and return 0xff.
    errorMessage << "Unable to convert '" << oneNibble << "' to a hex nibble!";

    LOG_ERROR(QString::fromStdString(errorMessage.str()));
    return 0xff;
}

/**
 * @brief HexDecoder::cleanup - Strip the hex encoded string of spaces, colons, dashes, periods of a 0x prefix
 *      and make sure all letters are upper case, then return the result.
 *
 * @param toClean - The string that we want to strip of extra characters.
 *
 * @return std::string stripped of extra characters allowed in a hex string.
 */
std::string HexDecoder::cleanup(const std::string &toClean)
{
    std::string smashed;

    // Need to replace the spaces, colons, dashes, or periods in the string to get a single, undecorated, hex
    // string.
    smashed = replaceInString(toClean, " ", "");
    smashed = replaceInString(smashed, ":", "");
    smashed = replaceInString(smashed, "-", "");
    smashed = replaceInString(smashed, ".", "");
    smashed = replaceInString(smashed, "0x", "");

    // Iterate the newly stripped string, making sure everything is in upper case.
    for (size_t i = 0; i < smashed.length(); i++) {
        smashed[i] = toupper(smashed.at(i));
    }

    return smashed;
}

/**
 * @brief HexDecoder::replaceInString - Replace 'toReplace', if it is found in 'sourceString', with
 *      the 'replaceWith' value.
 *
 * @param sourceString - The string to search for 'toReplace' in.
 * @param toReplace - The string that we want to replace.
 * @param replaceWith - The string to replace 'toReplace' with.
 *
 * @return
 */
std::string HexDecoder::replaceInString(const std::string &sourceString, const std::string &toReplace, const std::string &replaceWith)
{
    std::string result;

    // Replace all instances.
    result = sourceString;

    // Replace all of the instances.
    while (replaceSingleInstance(result, toReplace, replaceWith, result));

    // Return the updated string.
    return result;
}

/**
 * @brief HexDecoder::replaceSingleInstance - Replace a single instance of 'toReplace' with 'replaceWith' when it
 *      is found in 'sourceString'.
 *
 * @param sourceString - The string to replace an instance of 'toReplace' in.
 * @param toReplace - The string to search for in 'sourceString'.
 * @param replaceWith - The string that will replace 'toReplace' in the resulting string.
 * @param result[OUT] - The result string with 0 or 1 replacements in it.
 *
 * @return true if an instance of 'toReplace' was replaced.  false otherwise.
 */
bool HexDecoder::replaceSingleInstance(const std::string &sourceString, const std::string &toReplace, const std::string &replaceWith, std::string &result)
{
    size_t index = 0;

    // Copy the source to our result, then operate on the result string.
    result = sourceString;

    // Find an instance of substring in the string.
    index = sourceString.find(toReplace, index);
    if (index == std::string::npos) {
        // Didn't find anything to replace.
        return false;
    }

    // Found an instance, so replace it.
    result = result.replace(index, toReplace.length(), replaceWith);

    return true;
}
