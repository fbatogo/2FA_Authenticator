#ifndef HASHTYPEBASE_H
#define HASHTYPEBASE_H

#include <cstdlib>

/**
 * @brief The HashTypeBase class provides the template for implementing hashing methods
 *      that can be used with the associated HMAC implementation.
 */
class HashTypeBase
{
public:
    HashTypeBase() = default;
    virtual ~HashTypeBase() = default;

    virtual unsigned char *hash(unsigned char *bytes, size_t bytesLength) { return nullptr; }
    virtual size_t hashResultLength() { return 0; }
    virtual size_t hashBlockLength() { return 0; }
};

#endif // HASHTYPEBASE_H
