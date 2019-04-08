#ifndef HEXDECODER_H
#define HEXDECODER_H

#include <string>

class HexDecoder
{
public:
    HexDecoder();

    unsigned char *decode(std::string hexString, size_t &resultSize);

    static bool isHexEncoded(std::string toTest);

private:
    unsigned char decodeOneByte(std::string oneByte);
    unsigned char decodeOneNibble(char oneNibble);

    static std::string cleanup(std::string toClean);
    static std::string replaceInString(std::string sourceString, std::string toReplace, std::string replaceWith, bool global = true);
    static bool replaceSingleInstance(std::string sourceString, std::string toReplace, std::string replaceWith, std::string &result);
};

#endif // HEXDECODER_H
