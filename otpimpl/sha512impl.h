#ifndef SHA512IMPL_H
#define SHA512IMPL_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdint.h>

/* Structure to save state of computation between the single steps.  */
typedef struct
{
    uint64_t H[8];

    uint64_t total[2];
    uint64_t buflen;
    char buffer[256];	/* NB: always correctly aligned for uint64_t.  */
} SHA512_CTX;

void SHA512Init(SHA512_CTX *ctx);
void SHA512Update(SHA512_CTX *ctx, const unsigned char *data, uint32_t len);
void SHA512Final(unsigned char digest[32], SHA512_CTX *ctx);

void SHA512(unsigned char *hash_out, const char *str, uint32_t len);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // SHA512IMPL_H
