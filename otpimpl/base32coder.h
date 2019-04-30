#ifndef BASE32CODER_H
#define BASE32CODER_H

#include <string>

class Base32Coder
{
public:
    Base32Coder();

    std::string encode(unsigned char *toEncode, size_t toEncodeSize);
    unsigned char *decode(const std::string &toDecode, size_t &decodedSize);

    static bool isBase32Encoded(const std::string &toValidate);

private:
    bool decode8Chars(unsigned char *data, size_t dataOffset, unsigned char *target, size_t &decodedSize);
    unsigned char decodeChar(unsigned char toDecode);
};

#endif // BASE32CODER_H
