#include "bytearray.h"

#include <cstring>

#ifdef _WIN32
#define strdup  _strdup
#endif // _WIN32

ByteArray::ByteArray(bool zeroOnFree)
{
    init();
    mZeroOnFree = zeroOnFree;
}

ByteArray::ByteArray(size_t extraAllocationSize, bool zeroOnFree)
{
    init();
    mZeroOnFree = zeroOnFree;
    mExtraAllocationSize = extraAllocationSize;
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
    init();
    mZeroOnFree = zeroOnFree;

    fromCharArray(arrayToCopy, length);
}

ByteArray::ByteArray(const std::string &stringToCopy, bool zeroOnFree)
{
    init();
    mZeroOnFree = zeroOnFree;

    // Copy the data, and length from the string.
    fromStdString(stringToCopy);
}

ByteArray::ByteArray(const ByteArray &toCopy)
{
    init();

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
        // If we are told to zero the memory on free, zero it.
        if (mZeroOnFree) {
            memset(mByteArray, 0x00, mByteArrayLength);
        }

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
 * @brief ByteArray::setExtraAllocation - Set the amount of extra data that we want to
 *      allocate, to make appends more efficient.
 *
 * @param extraBytes - The number of extra bytes to allocate when memory is allocated.
 */
void ByteArray::setExtraAllocation(size_t extraBytes)
{
    mExtraAllocationSize = extraBytes;
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

    return true;
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
 *
 * @return true if the data was copied.  false on error.
 */
bool ByteArray::fromStdString(const std::string &stringToCopy)
{
    if (stringToCopy.empty()) {
        // Make sure we are clear.
        clear();
        return true;
    }

    // Copy the data, and length from the string.
    return fromCharArray(stringToCopy.c_str(), stringToCopy.length());
}

/**
 * @brief ByteArray::fromCharArray - Copy a char pointer full of data to our
 *      byte array.
 *
 * @param arrayToCopy - A pointer to the data to copy.
 * @param length - (Optional) If arrayToCopy is a null terminated string, then
 *      this value can be 0.  Otherwise, it should be the length of the data that
 *      we want to copy from arrayToCopy.
 *
 * @return true if the data was copied from the character array.  false on error.
 */
bool ByteArray::fromCharArray(const char *arrayToCopy, size_t length)
{
    if (nullptr == arrayToCopy) {
        // Clear out the data
        clear();
        return true;
    }

    // If the length specified is 0, we need to figure it out.
    if (0 == length) {
        length = strlen(arrayToCopy);
        if (0 == length) {
            // Clear any existing buffer and return.
            clear();
            return false;
        }
    }

    // Allocate the new buffer.
    if (!allocateBuffer(length)) {
        //  Nothing more we can do.
        return false;
    }

    // Copy in our data.
    memcpy(mByteArray, arrayToCopy, length);
    mByteArrayLength = length;

    return true;
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
 *
 * @return true if the data was copied.  false on error.
 */
bool ByteArray::fromUCharArray(const unsigned char *arrayToCopy, size_t length)
{
    return fromCharArray(reinterpret_cast<const char *>(arrayToCopy), length);
}

/**
 * @brief ByteArray::append - Append the data from a standard string to the existing
 *      data in this object.
 *
 * @param stringToAppend - The string that we want to append to the data already in
 *      this object.
 *
 * @return true if the data was appended.  false on error.
 */
bool ByteArray::append(const std::string &stringToAppend)
{
    return append(stringToAppend.c_str());
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
 *
 * @return true if the data was appended.  false on error.
 */
bool ByteArray::append(const char *arrayToAppend, size_t length)
{
    // See if we need to expand the buffer..
    if ((0 == mBufferAllocated) || ((mByteArrayLength + length) >= (mBufferAllocated - 1))) {
        // Expand the buffer.
        if (!expandBuffer((mByteArrayLength + length))) {
            // Can't do it.
            return false;
        }
    }

    // Copy the data to the buffer.
    memcpy(&mByteArray[mByteArrayLength], arrayToAppend, length);
    mByteArrayLength += length;

    return true;
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
 *
 * @return true if the data was appended.  false on error.
 */
bool ByteArray::append(const unsigned char *arrayToAppend, size_t length)
{
    return append(reinterpret_cast<const char *>(arrayToAppend), length);
}

/**
 * @brief ByteArray::append - Append a single character to the end of our byte array.
 *
 * @param charToAppend - The character to append to the data in this object.
 *
 * @return true if the character was appended.  false on error.
 */
bool ByteArray::append(const char charToAppend)
{
    return append(&charToAppend, 1);
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
bool ByteArray::operator==(const ByteArray &toCompare)
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
bool ByteArray::operator!=(const ByteArray &toCompare)
{
    return !((*this) == toCompare);
}

/**
 * @brief ByteArray::init - Initialize our member variables to their
 *      defaults.
 */
void ByteArray::init()
{
    mByteArray = nullptr;
    mByteArrayLength = 0;
    mBufferAllocated = 0;
    mZeroOnFree = false;
    mExtraAllocationSize = 0;
}

/**
 * @brief ByteArray::allocateBuffer - Free any existing buffer, and allocate a new one
 *      with the specified size, plus any extra allocation size.
 *
 * @param newSize - The new base size for the buffer.
 *
 * @return true if the buffer was cleared and a new buffer allocated.  false on error.
 */
bool ByteArray::allocateBuffer(size_t newSize)
{
    // Clear out the old buffer.
    clear();

    // Allocate the new buffer.
    mBufferAllocated = newSize + mExtraAllocationSize;
    mByteArray = new unsigned char[mBufferAllocated + 1];
    if (nullptr == mByteArray) {
        // Failed to allocate the memory.
        return false;
    }

    // Zero out the (whole) array.
    memset(mByteArray, 0x00, (mBufferAllocated + 1));

    // We are good.
    return true;
}

/**
 * @brief ByteArray::expandBuffer - Expand the existing buffer to a larger size by
 *      allocating a new buffer, and copying the existing data to it.
 *
 * @param newSize - The size of the new buffer.  (The extra allocation size will be
 *      added to this value.
 *
 * @return true if the buffer was expanded, and data copied.  false on error.
 */
bool ByteArray::expandBuffer(size_t newSize)
{
    unsigned char *oldBuffer = nullptr;
    size_t oldBufferSize = 0;

    if (nullptr != mByteArray) {
        // Store the pointer for later use.
        oldBuffer = mByteArray;
        oldBufferSize = mByteArrayLength;
    }

    // Allocate the new buffer.
    mBufferAllocated = newSize + mExtraAllocationSize;
    mByteArray = new unsigned char[mBufferAllocated + 1];
    if (nullptr == mByteArray) {
        // Failed to allocate the memory.
        return false;
    }

    // If we have an old buffer, copy the data to our new buffer, and update our
    // data buffer size.
    if (nullptr != oldBuffer) {
        memcpy(mByteArray, oldBuffer, oldBufferSize);
        mByteArrayLength = oldBufferSize;

        // If we are told to zero on free, do it.
        if (mZeroOnFree) {
            memset(oldBuffer, 0x00, oldBufferSize);
        }

        // Free the old memory.
        delete oldBuffer;
        oldBuffer = nullptr;
    }

    return true;
}
