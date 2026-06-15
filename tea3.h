#ifndef HAVE_TEA3_H
#define HAVE_TEA3_H

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TEA3_INITIAL_SKIP_ROUNDS 51u
#define TEA3_OUTPUT_SKIP_ROUNDS  19u

void tea3(uint32_t dwFrameNumbers, const uint8_t *lpKey, uint32_t dwNumKsBytes, uint8_t *lpKsOut);

extern const uint16_t g_awTea3LutA[8];
extern const uint16_t g_awTea3LutB[8];
extern const uint8_t g_abTea3Sbox[256];

uint64_t tea3_expand_iv(uint32_t dwFrameNumbers);
void tea3_inner(uint64_t qwIvReg, uint8_t abKeyReg[10], uint32_t dwNumKsBytes, uint8_t *lpKsOut);

#ifdef __cplusplus
}
#endif

#endif /* HAVE_TEA3_H */
