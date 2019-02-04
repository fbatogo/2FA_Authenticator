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
 * @param hexString - A string that contains the string representation of the hex we want to convert.
 * @param resultSize [OUT] - If this function does *NOT* return nullptr, this variable will contain the number of
 *      types that were returned.
 *
 * @return unsigned char pointer to the converted hex data.  If the input string is empty, or an error occurrs,
 *      nullptr will be returned.
 */
unsigned char *HexDecoder::decode(std::string hexString, size_t &resultSize)
{
    std::string smashed;
    unsigned char *result;

    // Start out assuming our result size will be 0.
    resultSize = 0;

    // If the string is empty, return nullptr.
    if (hexString.empty()) {
        return nullptr;
    }

    // Need to replace the spaces, colons, dashes, or periods in the string to get a single, undecorated, hex
    // string.
    smashed = replaceInString(hexString, " ", "");
    smashed = replaceInString(smashed, ":", "");
    smashed = replaceInString(smashed, "-", "");
    smashed = replaceInString(smashed, ".", "");
    smashed = replaceInString(smashed, "0x", "");

    // We should now have a string that is all hex characters.  So, the length should be divisible by 2.  Make
    // sure it is.
    if ((smashed.length() % 2) != 0) {
        LOG_ERROR("HEX string to decode wasn't an even number of characters!");
        return nullptr;
    }

    // We have an even number of characters, so allocate our return buffer, and set the return buffer length.
    resultSize = (smashed.length() / 2);

    result = static_cast<unsigned char *>(calloc(1, resultSize));
    if (result == nullptr) {
        LOG_ERROR("Unable to allocate a buffer for the result of the HEX string conversion!");
        return nullptr;
    }

    // Then, iterate the string passing 2 characters at a time in to decodeOneByte, and stashing the
    // result in our return buffer.
    for (size_t i = 0; i < resultSize; i++) {
        result[i] = decodeOneByte(smashed.substr((i * 2), 2));
    }

    return result;
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
unsigned char HexDecoder::decodeOneByte(std::string oneByte)
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
 * @brief HexDecoder::replaceInString - Replace 'toReplace', if it is found in 'sourceString', with
 *      the 'replaceWith' value.
 *
 * @param sourceString - The string to search for 'toReplace' in.
 * @param toReplace - The string that we want to replace.
 * @param replaceWith - The string to replace 'toReplace' with.
 * @param global - If true, then replace all instances of 'toReplace'.  Otherwise, only replace the
 *      first instance.
 *
 * @return
 */
std::string HexDecoder::replaceInString(std::string sourceString, std::string toReplace, std::string replaceWith, bool global)
{
    std::string result;

    if (!global) {
        // Replace a single instance.
        replaceSingleInstance(sourceString, toReplace, replaceWith, result);
    } else {
        // Replace all instances.
        result = sourceString;

        // Replace all of the instances.
        while (replaceSingleInstance(result, toReplace, replaceWith, result)) {};
    }

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
bool HexDecoder::replaceSingleInstance(std::string sourceString, std::string toReplace, std::string replaceWith, std::string &result)
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