#ifndef HEXDECODER_H
#define HEXDECODER_H

#include <string>

class HexDecoder
{
public:
    HexDecoder();

    unsigned char *decode(std::string hexString, size_t &resultSize);

private:
    unsigned char decodeOneByte(std::string oneByte);
    unsigned char decodeOneNibble(char oneNibble);

    std::string replaceInString(std::string sourceString, std::string toReplace, std::string replaceWith, bool global = true);
    bool replaceSingleInstance(std::string sourceString, std::string toReplace, std::string replaceWith, std::string &result);
};

#endif // HEXDECODER_H
