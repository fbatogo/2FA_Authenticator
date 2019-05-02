#include "base32coder.h"

#include <cstring>
#include <stdint.h>
#include <logger.h>

const char base32chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";


Base32Coder::Base32Coder()
{

}

/**
 * @brief Base32Coder::encode - Given a binary array of bytes, encode them using base32
 *      encoding.
 *
 * @param toEncode - The byte array to encode.
 * @param toEncodeSize - The length of the byte array.
 *
 * @return std::string containing the base32 encoded string.
 */
std::string Base32Coder::encode(unsigned char *toEncode, size_t toEncodeSize)
{
    std::string result;
    size_t blocks;
    size_t offset;
    unsigned char b;

    // If we have nothing to encode, return an empty string.
    if (toEncodeSize == 0) {
        return "";
    }

    result.clear();

    blocks = (toEncodeSize / 5);

    if (blocks == 0) {
        blocks = 1;
    } else {
        if ((toEncodeSize % 5) != 0) {
            blocks++;
        }
    }

    for (size_t i = 0; i < blocks; i++) {
        // Calculate the 32bit offset in to the byte string.
        offset = (i * 5);

        // Pull out the values that we need to look up.

        // Take the first 5 bits.
        result += base32chars[(toEncode[offset] >> 3)];

        // Take the last 3 bits in the first byte, and the first 2 of the 2nd byte.
        b = ((toEncode[offset] & 0x07)  << 2);
        if ((offset + 1) < toEncodeSize) {
            b |= (toEncode[offset+1] >> 6);
        }
        result += base32chars[b];

        if ((offset + 1) >= toEncodeSize) {
            result += "======";
        } else {
            // Then, the 5 bits in the middle of the 2nd byte.
            result += base32chars[(((toEncode[offset+1] & 0x3f) >> 1))];

            // Then, the last bit of the 2nd byte, and the high nibble of the 3rd.
            b = ((toEncode[offset+1] & 0x01) << 4);
            if ((offset + 2) < toEncodeSize) {
                b |= (toEncode[offset+2] >> 4);
            }
            result += base32chars[b];

            if ((offset + 2) >= toEncodeSize) {
                result += "====";
            } else {
                // Then, the low nibble of the 3rd byte, and 1 bit from the 4th.
                b = ((toEncode[offset+2] & 0x0f) << 1);
                if ((offset + 3) < toEncodeSize) {
                    b |= (toEncode[offset+3] >> 7);
                }
                result += base32chars[b];

                if ((offset + 3) >= toEncodeSize) {
                    result += "===";
                } else {
                    // Then the 5 middle-ish bits from the 4th byte.
                    result += base32chars[(((toEncode[offset+3] & 0x7f) >> 2))];

                    // Then the low 2 bits of the 4th byte and the high 3 of the 5th.
                    b = ((toEncode[offset+3] & 0x03) << 3);
                    if ((offset + 4) < toEncodeSize) {
                        b |= (toEncode[offset+4] >> 5);
                    }
                    result += base32chars[b];

                    if ((offset + 4) >= toEncodeSize) {
                        result += "=";
                    } else {
                        // Then, the last 5 bits of the 5th byte.
                        result += base32chars[(toEncode[offset+4] & 0x1f)];
                    }
                }
            }
        }
    }

    return result;
}

/**
 * @brief Base32Coder::decode - Decode a base 32 encoded string.
 *
 * @param toDecode - The string that we want to decode.
 *
 * @return unsigned char pointer to the decoded data.
 */
ByteArray Base32Coder::decode(const ByteArray &toDecode)
{
    ByteArray result;
    size_t blocks;
    size_t decodedSize;

    if (toDecode.empty()) {
        // Return an empty ByteArray.
        return ByteArray();
    }

    result.clear();

    // It should be evenly divisible by 8, if not, it isn't a valid string.
    if ((toDecode.size() % 8) != 0) {
        // Return a null pointer.
        return nullptr;
    }

    // Figure out how many blocks we have.
    blocks = toDecode.size() / 8;

    decodedSize = 0;

    // Iterate each block, converting it.
    for (size_t i = 0; i < blocks; i++) {
        if (decode8Chars(bytesToDecode, (i * 8), result, decodedSize) == false) {
            LOG_ERROR("Failed to decode 8 bytes!");

            // Free the memory we used.
            free(bytesToDecode);
            bytesToDecode = nullptr;

            return nullptr;
        }
    }

   return result;
}

/**
 * @brief Base32Coder::isBase32Encoded - Check to see if the provided string is base32
 *      encoded.
 *
 * @param toValidate - The string to validate the base32 encoding on.
 *
 * @return true if the string appears to be base32 encoded.  false otherwise.
 */
bool Base32Coder::isBase32Encoded(const std::string &toValidate)
{
    QString temp;

    if (toValidate.empty()) {
        LOG_ERROR("The string to validate for base32 encoding was empty!");
        return false;
    }

    // The string length needs to be evenly divisible by 8.
    if ((toValidate.length() % 8) != 0) {
        LOG_ERROR("The string provided isn't base32 encoded.  The length is incorrect!");
        return false;
    }

    // And, each character needs to be one of the characters allowed in the
    // array above.
    for (size_t i = 0; i < toValidate.length(); i++) {
        if (((toValidate.at(i) < 'A') || (toValidate.at(i) > 'Z')) &&
                ((toValidate.at(i) < '2') || (toValidate.at(i) > '7')) &&
                (toValidate.at(i) != '=')) {
            // The character is invalid.
            temp.clear();
            temp = toValidate.at(i);
            LOG_DEBUG("The character '" + temp + "' at index " + QString::number(i) + " is not a valid base32 encoded character!");
            return false;
        }
    }

    // If we get here, then the string appears to be a valid base32 string.
    return true;
}

/**
 * @brief Base32Coder::decode8Chars - Base32 decode a black of 8 characters.
 *
 * @param data - The ByteArray object that contains the data that we want to decode.
 * @param dataOffset - The offset in to the ByteArray that we want to decode.
 * @param target[OUT] - The ByteArray object that will have the decoded bytes added to it.
 *
 * @return true if the 8 bytes starting at dataOffset were decoded.  false on error.
 */
bool Base32Coder::decode8Chars(ByteArray &data, size_t dataOffset, ByteArray &target)
{
    if ((data == nullptr) || (target == nullptr)) {
        LOG_ERROR("Invalid inputs to decode8Chars!");
        return false;
    }

    // Decode the first byte.
    target[decodedSize++] = static_cast<unsigned char>((decodeChar(data[dataOffset]) << 3) | ((decodeChar(data[dataOffset + 1]) >> 2)));

    if (data[dataOffset + 2] != '=') {
        // Then the next.
        target[decodedSize++] = static_cast<unsigned char>(((decodeChar(data[dataOffset + 1]) << 6) | ((decodeChar(data[dataOffset + 2]) << 1) | ((decodeChar(data[dataOffset + 3]) >> 4)))));
    }

    if (data[dataOffset + 4] != '=') {
        target[decodedSize++] = static_cast<unsigned char>(((decodeChar(data[dataOffset + 3]) << 4) | ((decodeChar(data[dataOffset + 4]) >> 1))));
    }

    if (data[dataOffset + 5] != '=') {
        target[decodedSize++] = static_cast<unsigned char>(((decodeChar(data[dataOffset + 4]) << 7) | ((decodeChar(data[dataOffset + 5]) << 2) | ((decodeChar(data[dataOffset + 6]) >> 3)))));
    }

    if (data[dataOffset + 7] != '=') {
        target[decodedSize++] = static_cast<unsigned char>(((decodeChar(data[dataOffset + 6]) << 5) | ((decodeChar(data[dataOffset + 7])))));
    }

    return true;
}

/**
 * @brief Base32Coder::decodeChar - Convert a character to a 5 bit value.
 *
 * @param toDecode - The character to decode.
 *
 * @return unsigned char containing the 5 bit value.
 */
unsigned char Base32Coder::decodeChar(unsigned char toDecode)
{
    for (unsigned char i = 0; i < strlen(base32chars); i++) {
        if (toDecode == base32chars[i]) {
            return i;
        }
    }

    // Didn't find it.
    return 0xff;
}
