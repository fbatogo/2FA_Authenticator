#include "bytearray.h"

#include <cstring>

#ifdef _WIN32
#define strdup  _strdup
#endif // _WIN32

ByteArray::ByteArray(bool zeroOnFree)
{
    mByteArray = nullptr;
    mByteArrayLength = 0;
    mBufferAllocated = 0;
    mExtraAllocationSize = 0;
    mZeroOnFree = zeroOnFree;
}

ByteArray::ByteArray(size_t extraAllocationSize, bool zeroOnFree)
{
    mByteArray = nullptr;
    mByteArrayLength = 0;
    mBufferAllocated = 0;
    mExtraAllocationSize = extraAllocationSize;
    mZeroOnFree = zeroOnFree;
}

/**
 * @brief ByteArray::ByteArray - Create a new ByteArray object, using an existing
 *      character array, and (optionally) a character array length.
 *
 * @param arrayToCopy - A pointer to a character array to copy in to this object.
 * @param length - (Optional) The length of the character array pointed to by the
 *      \c arrayToCopy variable.
 * @param zeroOnFree - If set to true, then memory to be freed will be zeroed
 *      before being freed.
 */
ByteArray::ByteArray(const char *arrayToCopy, size_t length, bool zeroOnFree)
{
    mByteArray = nullptr;
    mByteArrayLength = 0;
    mBufferAllocated = 0;
    mZeroOnFree = zeroOnFree;
    mExtraAllocationSize = 0;

    fromCharArray(arrayToCopy, length);
}

ByteArray::ByteArray(const std::string &stringToCopy, bool zeroOnFree)
{
    mByteArray = nullptr;
    mByteArrayLength = 0;
    mZeroOnFree = zeroOnFree;
    mExtraAllocationSize = 0;
    mBufferAllocated = 0;

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
    mBufferAllocated = 0;
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
 * @brief ByteArray::setZeroOnFree - Change the behavior of freeing memory to either
 *      just free it, or zero out the current memory and then freeing it.
 *
 * @param newValue - The new value for the setting.
 */
void ByteArray::setZeroOnFree(bool newValue)
{
    mZeroOnFree = newValue;
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
unsigned char ByteArray::at(size_t idx) const
{
    if ((nullptr == mByteArray) || (idx > mByteArrayLength)) {
        // Return the null byte.
        return 0x00;
    }

    // Otherwise, return the byte at the specified index.
    return mByteArray[idx];
}

/**
 * @brief ByteArray::setAt - Set the byte at the specified index to the new value
 *      provided.
 *
 * @param idx - The index in to the data that we want to update the byte value for.
 * @param newValue - The new value to set the byte at the specified index to.
 *
 * @return true if the byte was updated.  false on error.
 */
bool ByteArray::setAt(size_t idx, unsigned char newValue)
{
    if ((idx >= mByteArrayLength) || (nullptr == mByteArray)) {
        // Can't add.  Only update.
        return false;
    }

    // Update the value.
    mByteArray[idx] = newValue;
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
    // XXX Change to use copyData().
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
 * @brief ByteArray::fromUCharArray - Take the unsigned char pointer to data and set
 *      it as the data stored in this object.
 *
 * @param arrayToCopy - An unsigned char pointer to the data that we want to store in
 *      this object.
 * @param length - (Optional) If arrayToCopy is a null terminated string, then
 *      this value can be 0.  Otherwise, it should be the length of the data that
 *      we want to copy from arrayToCopy.
 */
void ByteArray::fromUCharArray(const unsigned char *arrayToCopy, size_t length)
{
    fromCharArray(reinterpret_cast<const char *>(arrayToCopy), length);
}

/**
 * @brief ByteArray::append - Append the data from a standard string to the existing
 *      data in this object.
 *
 * @param stringToAppend - The string that we want to append to the data already in
 *      this object.
 */
void ByteArray::append(const std::string &stringToAppend)
{
    append(stringToAppend.c_str());
}

/**
 * @brief ByteArray::append - Append the data from a character array (C string) to
 *      the existing data in this object.
 *
 * @param arrayToAppend - The char * data that we want to append to the data already in
 *      this object.
 * @param length - (Optional) If this is set to 0, it is assumed that the data provided
 *      to append is a null terminated string.  If it is set to something else, that
 *      number of bytes will be appended to the existing data.
 */
void ByteArray::append(const char *arrayToAppend, size_t length)
{
    // XXX Change to using appendData().
    unsigned char *oldData;
    size_t oldDataLength;

    // Store our old data and old data length for later use.
    oldData = mByteArray;
    oldDataLength = mByteArrayLength;

    // If the length is 0, then we are dealing with a null terminated string.  So, figure
    // out how long the string is.
    if (0 == length) {
        length = strlen(arrayToAppend);
    }

    // Calculate the new length.
    mByteArrayLength = length + oldDataLength;

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

    // Then, copy the old data in to the beginning of the array.
    memcpy(mByteArray, oldData, oldDataLength);

    // And add the new data right after that.
    memcpy(&mByteArray[oldDataLength], arrayToAppend, length);

    // Then, clean up the memory from the old data.
    delete oldData;
    oldData = nullptr;
}

/**
 * @brief ByteArray::append - Append the data from the unsigned character array to
 *      the existing data in this object.
 *
 * @param arrayToAppend - The unsigned char * data that we want to append to the data
 *      already in this object.
 * @param length - (Optional) If this is set to 0, it is assumed that the data provided
 *      to append is a null terminated string.  If it is set to something else, that
 *      number of bytes will be appended to the existing data.
 */
void ByteArray::append(const unsigned char *arrayToAppend, size_t length)
{
    append(reinterpret_cast<const char *>(arrayToAppend), length);
}

/**
 * @brief ByteArray::append - Append a single character to the end of our byte array.
 *
 * @param charToAppend - The character to append to the data in this object.
 */
void ByteArray::append(const char charToAppend)
{
    // XXX FINISH!
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

    mExtraAllocationSize = toCopy.mExtraAllocationSize;
    mZeroOnFree = toCopy.mZeroOnFree;

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

/**
 * @brief ByteArray::reallocateBuffer - Allocate, or reallocate the mByteArray pointer in
 *      this object that contains the data we are operating on.
 *
 * @param newSize - The new size that is NEEDED in the buffer.  If mExtraAllocationSize is
 *      greater than 0, then the total amount of memory allocated will be this value plus
 *      mExtraAllocationSize.
 * @param zero - If true, and a buffer is currently allocated, the currently allocated
 *      memory will be zeroed out before the memory is freed.  If false, the memory will
 *      simply be freed.
 *
 * @return true if the buffer was allocated.  false on error.
 */
bool ByteArray::reallocateBuffer(size_t newSize)
{

}

bool ByteArray::copyData(unsigned char *toCopyIn, size_t length)
{

}

bool ByteArray::appendData(unsigned char *toAppend, size_t length)
{

}

bool ByteArray::appendByte(unsigned char toAppend)
{

}

