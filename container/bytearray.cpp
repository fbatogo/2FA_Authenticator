#include "bytearray.h"

#include <cstring>

#ifdef _WIN32
#define strdup  _strdup
#endif // _WIN32

ByteArray::ByteArray()
{
    mByteArray = nullptr;
    mByteArrayLength = 0;
}

/**
 * @brief ByteArray::ByteArray - Create a new ByteArray object, using an existing
 *      character array, and (optionally) a character array length.
 *
 * @param arrayToCopy - A pointer to a character array to copy in to this object.
 * @param length - (Optional) The length of the character array pointed to by the
 *      \c arrayToCopy variable.
 */
ByteArray::ByteArray(const char *arrayToCopy, size_t length)
{
    mByteArray = nullptr;
    mByteArrayLength = 0;

    fromCharArray(arrayToCopy, length);
}

ByteArray::ByteArray(const std::string &stringToCopy)
{
    mByteArray = nullptr;
    mByteArrayLength = 0;

    // Copy the data, and length from the string.
    fromStdString(stringToCopy);
}

ByteArray::ByteArray(const ByteArray &toCopy)
{
    (*this) = toCopy;
}

ByteArray::~ByteArray()
{
    clear();
}

/**
 * @brief ByteArray::clear - Free any memory, and clear out any values that are stored
 *      in the current object.
 */
void ByteArray::clear()
{
    if (nullptr != mByteArray) {
        // Free the memory.
        delete mByteArray;
        mByteArray = nullptr;
    }

    mByteArrayLength = 0;
}

/**
 * @brief ByteArray::empty - Return true if the data contained in this object is "empty".
 *
 * @return true if this object is empty.  false if not.
 */
bool ByteArray::empty() const
{
    return (nullptr == mByteArray);
}

/**
 * @brief ByteArray::at - Return the character at the specified index.
 *
 * @param idx - The index to get the character from.
 *
 * @return char containing the character at the specified index.  If the index is beyond
 *      the length of the array, or if the array isn't defined, a null character (0x00)
 *      is returned.
 */
char ByteArray::at(size_t idx) const
{
    if ((nullptr == mByteArray) || (idx > mByteArrayLength)) {
        // Return the null byte.
        return 0x00;
    }

    // Otherwise, return the byte at the specified index.
    return mByteArray[idx];
}

/**
 * @brief ByteArray::size - Return the size of the byte array that is currently stored
 *      in this object.
 *
 * @return size_t containing the length of the byte array currently stored in this
 *      object.
 */
size_t ByteArray::size() const
{
    return mByteArrayLength;
}

/**
 * @brief ByteArray::fromStdString - Convert a standard string to a byte array.
 *
 * @param stringToCopy - The string that we want to copy to our byte array.
 */
void ByteArray::fromStdString(const std::string &stringToCopy)
{
    // Clean out anything already in memory.
    clear();

    // Copy the data, and length from the string.
    fromCharArray(stringToCopy.c_str(), stringToCopy.length());
}

/**
 * @brief ByteArray::fromCharArray - Copy a char pointer full of data to our
 *      byte array.
 *
 * @param arrayToCopy - A pointer to the data to copy.
 * @param length - (Optional) If arrayToCopy is a null terminated string, then
 *      this value can be 0.  Otherwise, it should be the length of the data that
 *      we want to copy from arrayToCopy.
 */
void ByteArray::fromCharArray(const char *arrayToCopy, size_t length)
{
    // Clean out anything that might be hanging around.
    clear();

    mByteArrayLength = length;

    // If the length is 0, then we are dealing with a null terminated string.  So, figure
    // out how long the string is.
    if (0 == mByteArrayLength) {
        mByteArrayLength = strlen(arrayToCopy);
    }

    // Allocate the memory we need to store the array, plus one extra character to be
    // sure we pick up a null character.
    mByteArray = new unsigned char[mByteArrayLength + 1];
    if (nullptr == mByteArray) {
        // Couldn't allocate memory, set everything to 0 and null.
        mByteArray = nullptr;
        mByteArrayLength = 0;
        return;
    }

    // Zero out the array.
    memset(mByteArray, 0x00, (mByteArrayLength + 1));

    // Then, copy the data from arrayToCopy in to our target array.
    memcpy(mByteArray, arrayToCopy, mByteArrayLength);
}

/**
 * @brief ByteArray::toString - Return the byte array as a standard string.
 *
 * @return std::string containing the data from the byte array stored in this object.
 */
std::string ByteArray::toString() const
{
    if (nullptr == mByteArray) {
        // Return an empty string.
        return "";
    }

    return std::string(toCharArrayPtr(), size());
}

/**
 * @brief ByteArray::toCharArrayPtr - Return the internal pointer to the byte array data stored
 *      in this object.
 *
 * @return char pointer to the data stored in this object.
 */
const char *ByteArray::toCharArrayPtr() const
{
    return reinterpret_cast<char *>(mByteArray);
}

/**
 * @brief ByteArray::toUCharArrayPtr - Return a pointer to an unsigned char byte array using the
 *      data stored in this object.
 *
 * @return unsigned char pointer to the data stored in this object.
 */
const unsigned char *ByteArray::toUCharArrayPtr() const
{
    return mByteArray;
}

/**
 * @brief ByteArray::operator = - Copy the contents of another ByteArray object in to
 *      this object.
 *
 * @param toCopy - The other object to copy the contents from.
 *
 * @return ByteArray with the contents copied.
 */
ByteArray &ByteArray::operator=(const ByteArray &toCopy)
{
    if (this == &toCopy) {
        return (*this);
    }

    fromUCharArray(toCopy.mByteArray, toCopy.mByteArrayLength);

    return (*this);
}

/**
 * @brief ByteArray::operator = - Copy the contents of a standard string to this ByteArray
 *      object.
 *
 * @param toCopy - The string to copy the data from.
 *
 * @return ByteArray with the string contents copied.
 */
ByteArray &ByteArray::operator=(const std::string &toCopy)
{
    this->fromStdString(toCopy);

    return (*this);
}

/**
 * @brief ByteArray::operator == - Compare the contents of this object to the contents
 *      of another object of the same type.
 *
 * @param toCompare - The object whose content we want to compare with this one.
 *
 * @return true if the contents match.  false otherwise.
 */
bool ByteArray::operator==(ByteArray &toCompare)
{
    // Are the lengths the same?
    if (mByteArrayLength != toCompare.mByteArrayLength) {
        // They don't match.
        return false;
    }

    // The lengths are the same, so are the byte arrays the same?
    if (memcmp(mByteArray, toCompare.mByteArray, mByteArrayLength) == 0) {
        // Its a match!
        return true;
    }

    // It isn't a match!
    return false;
}

/**
 * @brief ByteArray::operator != - Compare the contents of this object to the contents
 *      of another object of the same type, and return true if they are different.
 *
 * @param toCompare - The object to compare this one with.
 *
 * @return true if they don't match.  false if they do.
 */
bool ByteArray::operator!=(ByteArray &toCompare)
{
    return !((*this) == toCompare);
}

