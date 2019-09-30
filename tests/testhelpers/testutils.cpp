#include "testutils.h"

#include <sstream>
#include <iomanip>

#ifndef _WIN32
#include <unistd.h>
#endif // _WIN32

std::string TestUtils::binaryToString(const unsigned char *bytes, size_t bytesLength)
{
    std::stringstream result;

    result.clear();

    for (size_t i = 0; i < bytesLength; i++) {
        result << std::setfill('0') << std::setw(2) << std::hex << (int)bytes[i] << " ";
    }

    return result.str();
}

std::string TestUtils::binaryToString(const ByteArray &bytes)
{
    return binaryToString(bytes.toUCharArrayPtr(), bytes.size());
}

std::string TestUtils::binaryToString(const std::shared_ptr<ByteArray> &bytes)
{
    return binaryToString((*bytes.get()));
}

bool TestUtils::fileExists(const std::string &filename)
{
    FILE *fp = nullptr;

    // Try to open the file read only.  If we can, then the file must exist.
    // Though, if we can't, it may still exist.  But, this is good enough for
    // now.
    fp = fopen(filename.c_str(), "r");
    if (fp == nullptr) {
        // It doesn't exist.
        return false;
    }

    // Close it.
    fclose(fp);

    return true;
}

bool TestUtils::deleteFile(const std::string &filename)
{
    return (0 == unlink(filename.c_str()));
}
