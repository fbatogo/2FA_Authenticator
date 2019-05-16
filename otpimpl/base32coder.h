#ifndef BASE32CODER_H
#define BASE32CODER_H

#include <string>
#include "container/bytearray.h"

class Base32Coder
{
public:
    Base32Coder();

    ByteArray encode(const ByteArray &toEncode);
    ByteArray decode(const ByteArray &toDecode);

    static bool isBase32Encoded(const ByteArray &toValidate);

protected:
    size_t getBlocksFromSize(size_t dataSize);
    ByteArray encodeOneBlock(size_t blockCount, const ByteArray &toEncode);
    bool decode8Chars(const ByteArray &data, size_t dataOffset, ByteArray &target);
    unsigned char decodeChar(unsigned char toDecode);
};

#endif // BASE32CODER_H
