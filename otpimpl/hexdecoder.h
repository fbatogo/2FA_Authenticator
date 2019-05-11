#ifndef HEXDECODER_H
#define HEXDECODER_H

#include <string>
#include "container/bytearray.h"

class HexDecoder
{
public:
    HexDecoder();

    ByteArray decode(const ByteArray &hexData);

    static bool isHexEncoded(const std::string &toTest);

protected:
    unsigned char decodeOneByte(const std::string &oneByte);
    unsigned char decodeOneNibble(char oneNibble);

private:
    static std::string cleanup(const std::string &toClean);
    static std::string replaceInString(const std::string &sourceString, const std::string &toReplace, const std::string &replaceWith);
    static bool replaceSingleInstance(const std::string &sourceString, const std::string &toReplace, const std::string &replaceWith, std::string &result);
};

#endif // HEXDECODER_H
