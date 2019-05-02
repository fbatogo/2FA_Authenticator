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
    ByteArray(const ByteArray &toCopy);
    ~ByteArray();

    void clear();
    bool empty() const;

    unsigned char at(size_t idx) const;
    bool setAt(size_t idx, unsigned char newValue);

    size_t size() const;

    void fromStdString(const std::string &stringToCopy);
    void fromCharArray(const char *arrayToCopy, size_t length = 0);
    void fromUCharArray(const unsigned char *arrayToCopy, size_t length = 0);

    void append(const std::string &stringToAppend);
    void append(const char *arrayToAppend, size_t length = 0);
    void append(const unsigned char *arrayToAppend, size_t length = 0);

    std::string toString() const;
    const char *toCharArrayPtr() const;
    const unsigned char *toUCharArrayPtr() const;

    // Assignment operators.
    ByteArray &operator=(const ByteArray &toCopy);
    ByteArray &operator=(const std::string &toCopy);

    // Comparison operators.
    bool operator==(ByteArray &toCompare);
    bool operator!=(ByteArray &toCompare);

private:

    unsigned char *mByteArray;
    size_t mByteArrayLength;
};

#endif // BYTEARRAY_H
