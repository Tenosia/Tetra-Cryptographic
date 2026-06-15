#ifndef HAVE_TEA2_H
#define HAVE_TEA2_H

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TEA2_INITIAL_SKIP_ROUNDS 51u
#define TEA2_OUTPUT_SKIP_ROUNDS  19u

void tea2(uint32_t dwFrameNumbers, const uint8_t *lpKey, uint32_t dwNumKsBytes, uint8_t *lpKsOut);

extern const uint16_t g_awTea2LutA[8];
extern const uint16_t g_awTea2LutB[8];
extern const uint8_t g_abTea2Sbox[256];

uint64_t tea2_expand_iv(uint32_t dwFrameNumbers);
void tea2_inner(uint64_t qwIvReg, uint8_t abKeyReg[10], uint32_t dwNumKsBytes, uint8_t *lpKsOut);

#ifdef __cplusplus
}
#endif

#endif /* HAVE_TEA2_H */
