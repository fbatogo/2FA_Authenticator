#ifndef BYTEARRAY_H
#define BYTEARRAY_H

#include <string>

/**
 * @brief The ByteArray class is a container/memory management class for holding
 *          arrays of bytes (basically, char * arrays used in C as strings).
 */
class ByteArray
{
public:
    ByteArray();
    ByteArray(const char *arrayToCopy, size_t length = 0);
    ByteArray(const std::string &stringToCopy);
    ByteArray(ByteArray &toCopy);
    ~ByteArray();

    void clear();

    char at(size_t idx);
    size_t size();

    std::string toString();

    ByteArray &operator=(ByteArray &toCopy);
    bool operator==(ByteArray &toCompare);

private:
    void copy(ByteArray &toCopy);

    char *mByteArray;
    size_t mByteArrayLength;
};

#endif // BYTEARRAY_H
