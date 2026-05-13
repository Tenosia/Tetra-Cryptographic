#ifndef HAVE_TEA3_H
#define HAVE_TEA3_H

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

void tea3(uint32_t dwFrameNumbers, uint8_t *lpKey, uint32_t dwNumKsBytes, uint8_t *lpKsOut);

extern const uint16_t g_awTea3LutA[8];
extern const uint16_t g_awTea3LutB[8];
extern const uint8_t g_abTea3Sbox[256];

#ifdef __cplusplus
}
#endif

#endif /* HAVE_TEA3_H */
