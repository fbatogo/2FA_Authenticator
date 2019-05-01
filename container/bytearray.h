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
    explicit ByteArray(const std::string &stringToCopy);
    ByteArray(ByteArray &toCopy);
    ~ByteArray();

    void clear();

    char at(size_t idx);
    size_t size();

    void fromStdString(const std::string &stringToCopy);
    void fromCharArray(const char *arrayToCopy, size_t length = 0);

    std::string toString();
    char *toCharArrayPtr() const;
    size_t toCharArraySize() const;

    // Assignment operators.
    ByteArray &operator=(ByteArray &toCopy);
    ByteArray &operator=(const std::string &toCopy);

    // Comparison operators.
    bool operator==(ByteArray &toCompare);
    bool operator!=(ByteArray &toCompare);

private:

    char *mByteArray;
    size_t mByteArrayLength;
};

#endif // BYTEARRAY_H
