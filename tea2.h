#ifndef HAVE_TEA2_H
#define HAVE_TEA2_H

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

uint64_t tea2_expand_iv(uint32_t dwFrameNumbers);

void tea2(uint32_t dwFrameNumbers, uint8_t *lpKey, uint32_t dwNumKsBytes, uint8_t *lpKsOut);

extern const uint16_t g_abTea2LutA[8];
extern const uint16_t g_abTea2LutB[8];
extern const uint8_t g_abTea2Sbox[256];

#ifdef __cplusplus
}
#endif

#endif /* HAVE_TEA2_H */
