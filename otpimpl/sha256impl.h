#ifndef SHA256IMPL_H
#define SHA256IMPL_H

#include <stdint.h>

#if __cplusplus
extern "C" {
#endif // __cplusplus

/* Structure to save state of computation between the single steps.  */
typedef struct
{
    uint32_t H[8];

    uint32_t total[2];
    uint32_t buflen;
    char buffer[128];	/* NB: always correctly aligned for uint32_t.  */
} SHA256_CTX;

void SHA256Init(SHA256_CTX *ctx);
void SHA256Update(SHA256_CTX *ctx, const unsigned char *data, uint32_t len);
void SHA256Final(unsigned char digest[32], SHA256_CTX *ctx);

void SHA256(unsigned char *hash_out, const char *str, uint32_t len);

#if __cplusplus
}
#endif // __cplusplus

#endif // SHA256IMPL_H
