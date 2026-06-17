#ifndef HAVE_HURDLE_H
#define HAVE_HURDLE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HURDLE_KEY_BYTES   16u
#define HURDLE_BLOCK_BYTES 8u
#define HURDLE_ROUNDS      16u

#define HURDLE_ENCRYPT 0u
#define HURDLE_DECRYPT 1u

typedef struct {
    uint8_t abRoundKeys[HURDLE_ROUNDS * HURDLE_KEY_BYTES];
} HURDLE_CTX;

void HURDLE_set_key(const uint8_t k[HURDLE_KEY_BYTES], HURDLE_CTX *lpContextOut);
void HURDLE_set_key_fw(const uint8_t k[HURDLE_KEY_BYTES], HURDLE_CTX *lpContextOut);

void HURDLE_encrypt(
    uint8_t abOutput[HURDLE_BLOCK_BYTES],
    const uint8_t abInput[HURDLE_BLOCK_BYTES],
    const HURDLE_CTX *lpKey,
    uint8_t eEncryptMode);

void HURDLE_enc_cbc(
    uint8_t abCiphertext[2 * HURDLE_BLOCK_BYTES],
    const uint8_t abPlaintext[2 * HURDLE_BLOCK_BYTES],
    const uint8_t abKey[HURDLE_KEY_BYTES]);

void HURDLE_dec_cts(
    uint8_t abPlaintext[15],
    const uint8_t abCiphertext[15],
    const uint8_t abKey[HURDLE_KEY_BYTES]);

void HURDLE_f(uint8_t abOutput[4], const uint8_t abRhs[4], const uint8_t *lpRoundKey);

extern const uint8_t g_abHurdleSbox[256];
extern const uint8_t g_abHurdleInvSbox[256];

#ifdef __cplusplus
}
#endif

#endif /* HAVE_HURDLE_H */
