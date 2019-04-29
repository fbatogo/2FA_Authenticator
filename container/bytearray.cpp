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
    mByteArrayLength = length;

    // If the length is 0, then we are dealing with a null terminated string.  So, figure
    // out how long the string is.
    if (0 == mByteArrayLength) {
        mByteArrayLength = strlen(arrayToCopy);
    }

    // Allocate the memory we need to store the array, plus one extra character to be
    // sure we pick up a null character.
    mByteArray = static_cast<char *>(calloc(1, (mByteArrayLength + 1)));
    if (nullptr == mByteArray) {
        // Couldn't allocate memory, set everything to 0 and null.
        mByteArray = nullptr;
        mByteArrayLength = 0;
        return;
    }

    // Then, copy the data from arrayToCopy in to our target array.
    memcpy(mByteArray, arrayToCopy, mByteArrayLength);
}

ByteArray::ByteArray(const std::string &stringToCopy)
{
    // Copy the data, and length from the string.
    mByteArray = strdup(stringToCopy.c_str());
    mByteArrayLength = stringToCopy.length();
}

ByteArray::ByteArray(ByteArray &toCopy)
{
    copy(toCopy);
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
        free(mByteArray);
        mByteArray = nullptr;
    }

    mByteArrayLength = 0;
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
char ByteArray::at(size_t idx)
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
size_t ByteArray::size()
{
    return mByteArrayLength;
}

/**
 * @brief ByteArray::toString - Return the byte array as a standard string.
 *
 * @return std::string containing the data from the byte array stored in this object.
 */
std::string ByteArray::toString()
{
    if (nullptr == mByteArray) {
        // Return an empty string.
        return "";
    }

    return std::string(mByteArray, mByteArrayLength);
}

/**
 * @brief ByteArray::operator = - Copy the contents of another ByteArray object in to
 *      this object.
 *
 * @param toCopy - The other object to copy the contents from.
 *
 * @return ByteArray with the contents copied.
 */
ByteArray &ByteArray::operator=(ByteArray &toCopy)
{
    if (this == &toCopy) {
        return (*this);
    }

    copy(toCopy);

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
 * @brief ByteArray::copy - Copy the byte array data from another object into this one.
 *
 * @param toCopy - The object that we want to copy the byte array data from.
 */
void ByteArray::copy(ByteArray &toCopy)
{
    // If this object already contains data, we need to free it.
    clear();

    // Then, copy the character array from the other object.
    mByteArray = static_cast<char *>(calloc(1, (toCopy.mByteArrayLength + 1)));
    if (nullptr == mByteArray) {
        // Couldn't allocate the memory.  Just return.
        return;
    }

    // Copy the data.
    memcpy(mByteArray, toCopy.mByteArray, toCopy.mByteArrayLength);
    mByteArrayLength = toCopy.mByteArrayLength;
}
