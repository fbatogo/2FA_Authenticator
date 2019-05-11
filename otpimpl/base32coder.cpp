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
 * @param toEncode - The ByteArray to encode.
 *
 * @return ByteArray containing the base32 encoded string.
 */
ByteArray Base32Coder::encode(const ByteArray &toEncode)
{
    ByteArray result;
    size_t blocks;
    size_t offset;
    unsigned char b;

    // If we have nothing to encode, return an empty string.
    if (toEncode.empty()) {
        return ByteArray();
    }

    result.clear();

    blocks = (toEncode.size() / 5);

    if (blocks == 0) {
        blocks = 1;
    } else {
        if ((toEncode.size() % 5) != 0) {
            blocks++;
        }
    }

    // Set the result to pre-allocate enough space for our data.
    result.setExtraAllocation(blocks * 10);

    for (size_t i = 0; i < blocks; i++) {
        // Calculate the 32bit offset in to the byte string.
        offset = (i * 5);

        // Pull out the values that we need to look up.

        // Take the first 5 bits.
        result.append(base32chars[toEncode.at(offset) >> 3]);

        // Take the last 3 bits in the first byte, and the first 2 of the 2nd byte.
        b = ((toEncode.at(offset) & 0x07)  << 2);
        if ((offset + 1) < toEncode.size()) {
            b |= (toEncode.at(offset+1) >> 6);
        }
        result.append(base32chars[b]);

        if ((offset + 1) >= toEncode.size()) {
            result.append("======");
        } else {
            // Then, the 5 bits in the middle of the 2nd byte.
            result.append(base32chars[(toEncode.at(offset+1) & 0x3f) >> 1]);

            // Then, the last bit of the 2nd byte, and the high nibble of the 3rd.
            b = ((toEncode.at(offset+1) & 0x01) << 4);
            if ((offset + 2) < toEncode.size()) {
                b |= (toEncode.at(offset+2) >> 4);
            }
            result.append(base32chars[b]);

            if ((offset + 2) >= toEncode.size()) {
                result.append("====");
            } else {
                // Then, the low nibble of the 3rd byte, and 1 bit from the 4th.
                b = ((toEncode.at(offset+2) & 0x0f) << 1);
                if ((offset + 3) < toEncode.size()) {
                    b |= (toEncode.at(offset+3) >> 7);
                }
                result.append(base32chars[b]);

                if ((offset + 3) >= toEncode.size()) {
                    result.append("===");
                } else {
                    // Then the 5 middle-ish bits from the 4th byte.
                    result.append(base32chars[(toEncode.at(offset+3) & 0x7f) >> 2]);

                    // Then the low 2 bits of the 4th byte and the high 3 of the 5th.
                    b = ((toEncode.at(offset+3) & 0x03) << 3);
                    if ((offset + 4) < toEncode.size()) {
                        b |= (toEncode.at(offset+4) >> 5);
                    }
                    result.append(base32chars[b]);

                    if ((offset + 4) >= toEncode.size()) {
                        result.append('=');
                    } else {
                        // Then, the last 5 bits of the 5th byte.
                        result.append(base32chars[toEncode.at(offset+4) & 0x1f]);
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

    if (toDecode.empty()) {
        // Return an empty ByteArray.
        return ByteArray();
    }

    result.clear();

    // It should be evenly divisible by 8, if not, it isn't a valid string.
    if ((toDecode.size() % 8) != 0) {
        // Return a null pointer.
        return ByteArray();
    }

    // Set our result to pre-allocate enough memory to store everything decoded, and then some.
    result.setExtraAllocation(toDecode.size());

    // Figure out how many blocks we have.
    blocks = toDecode.size() / 8;

    // Iterate each block, converting it.
    for (size_t i = 0; i < blocks; i++) {
        if (decode8Chars(toDecode, (i * 8), result) == false) {
            LOG_ERROR("Failed to decode 8 bytes!");
            return ByteArray();     // Return an empty object.
        }
    }

   return result;
}

/**
 * @brief Base32Coder::isBase32Encoded - Check to see if the provided string is base32
 *      encoded.
 *
 * @param toValidate - The ByteArray to validate the base32 encoding on.
 *
 * @return true if the string appears to be base32 encoded.  false otherwise.
 */
bool Base32Coder::isBase32Encoded(const ByteArray &toValidate)
{
    QString temp;

    if (toValidate.empty()) {
        LOG_ERROR("The string to validate for base32 encoding was empty!");
        return false;
    }

    // The string length needs to be evenly divisible by 8.
    if ((toValidate.size() % 8) != 0) {
        LOG_ERROR("The string provided isn't base32 encoded.  The length is incorrect!");
        return false;
    }

    // And, each character needs to be one of the characters allowed in the
    // array above.
    for (size_t i = 0; i < toValidate.size(); i++) {
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
bool Base32Coder::decode8Chars(const ByteArray &data, size_t dataOffset, ByteArray &target)
{
    unsigned char decoded1;
    unsigned char decoded2;
    unsigned char decoded3;

    // If the data is empty, or we don't have 8 bytes to decode, it is a failure.
    if ((data.empty()) || ((dataOffset + 8) > data.size())) {
        LOG_ERROR("Attempted to decode data beyond the end of the input data!");
        return false;
    }

    // Decode the first byte.
    decoded1 = decodeChar(data.at(dataOffset));
    decoded2 = decodeChar(data.at(dataOffset + 1));
    if ((decoded1 == 0xff) || (decoded2 == 0xff)) {
        LOG_ERROR("Unable to decode the 1st set of bytes!");
        return false;
    }

    if (!target.append(static_cast<char>((decoded1 << 3) | (decoded2 >> 2)))) {
        return false;
    }

    if (data.at(dataOffset + 2) != '=') {
            decoded1 = decodeChar(data.at(dataOffset + 1));
            decoded2 = decodeChar(data.at(dataOffset + 2));
            decoded3 = decodeChar(data.at(dataOffset + 3));
            if ((decoded1 == 0xff) || (decoded2 == 0xff) || (decoded3 == 0xff)) {
                LOG_ERROR("Unable to decode the 2nd set of bytes!");
                return false;
            }

            if (!target.append(static_cast<char>((decoded1 << 6) | (decoded2 << 1) | (decoded3 >> 4)))) {
                return false;
            }
    }

    if (data.at(dataOffset + 4) != '=') {
        decoded1 = decodeChar(data.at(dataOffset + 3));
        decoded2 = decodeChar(data.at(dataOffset + 4));
        if ((decoded1 == 0xff) || (decoded2 == 0xff)) {
            LOG_ERROR("Unable to decode the 3rd set of bytes!");
            return false;
        }

        if (!target.append(static_cast<char>((decoded1 << 4) | (decoded2 >> 1)))) {
            return false;
        }
    }

    if (data.at(dataOffset + 5) != '=') {
        decoded1 = decodeChar(data.at(dataOffset + 4));
        decoded2 = decodeChar(data.at(dataOffset + 5));
        decoded3 = decodeChar(data.at(dataOffset + 6));
        if ((decoded1 == 0xff) || (decoded2 == 0xff) || (decoded3 == 0xff)) {
            LOG_ERROR("Unable to decode the 4th set of bytes!");
            return false;
        }

        if (!target.append(static_cast<char>((decoded1 << 7) | (decoded2 << 2) | (decoded3 >> 3)))) {
            return false;
        }
    }

    if (data.at(dataOffset + 7) != '=') {
        decoded1 = decodeChar(data.at(dataOffset + 6));
        decoded2 = decodeChar(data.at(dataOffset + 7));
        if ((decoded1 == 0xff) || (decoded2 == 0xff)) {
            LOG_ERROR("Unable to decode the 5th set of bytes!");
            return false;
        }

        if (!target.append(static_cast<char>((decoded1 << 5) | (decoded2)))) {
            return false;
        }
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
