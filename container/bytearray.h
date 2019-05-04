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
    ByteArray(bool zeroOnFree = false);
    ByteArray(size_t extraAllocationSize, bool zeroOnFree = false);
    ByteArray(const char *arrayToCopy, size_t length = 0, bool zeroOnFree = false);
    ByteArray(const std::string &stringToCopy, bool zeroOnFree = false);
    ByteArray(const ByteArray &toCopy);
    ~ByteArray();

    void clear();
    bool empty() const;

    void setZeroOnFree(bool newValue);
    void setExtraAllocation(size_t extraBytes);

    unsigned char at(size_t idx) const;
    bool setAt(size_t idx, unsigned char newValue);

    size_t size() const;

    bool fromStdString(const std::string &stringToCopy);
    bool fromCharArray(const char *arrayToCopy, size_t length = 0);
    bool fromUCharArray(const unsigned char *arrayToCopy, size_t length = 0);

    bool append(const std::string &stringToAppend);
    bool append(const char *arrayToAppend, size_t length = 0);
    bool append(const unsigned char *arrayToAppend, size_t length = 0);
    bool append(const char charToAppend);

    std::string toString() const;
    const char *toCharArrayPtr() const;
    const unsigned char *toUCharArrayPtr() const;

    // Assignment operators.
    ByteArray &operator=(const ByteArray &toCopy);
    ByteArray &operator=(const std::string &toCopy);

    // Comparison operators.
    bool operator==(const ByteArray &toCompare);
    bool operator!=(const ByteArray &toCompare);

private:
    void init();
    bool allocateBuffer(size_t newSize);
    bool expandBuffer(size_t newSize);

    unsigned char *mByteArray;
    size_t mBufferAllocated;
    size_t mByteArrayLength;
    size_t mExtraAllocationSize;
    bool mZeroOnFree;
};

#endif // BYTEARRAY_H
