#ifndef HAVE_HURDLE_H
#define HAVE_HURDLE_H

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t abRoundKeys[256];
} HURDLE_CTX;

#define HURDLE_ENCRYPT 0
#define HURDLE_DECRYPT 1

void HURDLE_set_key(const uint8_t *k, HURDLE_CTX *lpContextOut);
void HURDLE_encrypt(uint8_t abOutput[8], const uint8_t abInput[8], HURDLE_CTX *lpKey, uint8_t eEncryptMode);
void HURDLE_enc_cbc(uint8_t abCiphertext[16], const uint8_t abPlaintext[16], uint8_t abKey[16]);
void HURDLE_dec_cts(uint8_t abPlaintext[15], const uint8_t abCiphertext[15], uint8_t abKey[16]);

void HURDLE_set_key_fw(const uint8_t *k, HURDLE_CTX *lpContextOut);

void HURDLE_f(uint8_t abOutput[4], const uint8_t abRhs[4], const uint8_t *lpRoundKey);

extern const uint8_t g_abHurdleSbox[256];
extern const uint8_t g_abHurdleInvSbox[256];

#ifdef __cplusplus
}
#endif

#endif /* HAVE_HURDLE_H */
