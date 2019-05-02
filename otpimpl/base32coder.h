#ifndef BASE32CODER_H
#define BASE32CODER_H

#include <string>
#include "container/bytearray.h"

class Base32Coder
{
public:
    Base32Coder();

    std::string encode(unsigned char *toEncode, size_t toEncodeSize);
    ByteArray decode(const ByteArray &toDecode);

    static bool isBase32Encoded(const std::string &toValidate);

private:
    bool decode8Chars(ByteArray &data, size_t dataOffset, ByteArray &target);
    unsigned char decodeChar(unsigned char toDecode);
};

#endif // BASE32CODER_H
