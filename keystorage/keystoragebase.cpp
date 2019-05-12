#include "keystorage.h"

/**
 * @brief KeyStorageBase::freeKeyStorage - Take whatever steps are necessary to free
 *      the key storage method that is in use.
 *
 * @return true if the key storage was freed.  false otherwise.
 */
bool KeyStorageBase::freeKeyStorage()
{
    // By default, do nothing.
    return true;
}
