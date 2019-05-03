#ifndef TESTUTILS_H
#define TESTUTILS_H

#include <string>
#include <memory>
#include "container/bytearray.h"

class TestUtils
{
public:
    static std::string binaryToString(const unsigned char *bytes, size_t bytesLength);
    static std::string binaryToString(const ByteArray &bytes);
    static std::string binaryToString(const std::shared_ptr<ByteArray> &bytes);
};

#endif // TESTUTILS_H
