#ifndef TESTUTILS_H
#define TESTUTILS_H

#include <string>

class TestUtils
{
public:
    static std::string binaryToString(unsigned char *bytes, size_t bytesLength);
};

#endif // TESTUTILS_H
