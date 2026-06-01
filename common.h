#ifndef HAVE_COMMON_H
#define HAVE_COMMON_H

#include <stdint.h>

#if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && defined(__ORDER_BIG_ENDIAN__)
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define TETRA_COMMON_IS_LE 1
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define TETRA_COMMON_IS_LE 0
#else
#error "common.h: unsupported __BYTE_ORDER__"
#endif
#else
#include <endian.h>
#if __BYTE_ORDER == __LITTLE_ENDIAN
#define TETRA_COMMON_IS_LE 1
#elif __BYTE_ORDER == __BIG_ENDIAN
#define TETRA_COMMON_IS_LE 0
#else
#error "common.h: unsupported host byte order"
#endif
#endif

#define TETRA_ROTL32(x, n) \
    (((uint32_t)(x) << (n)) | ((uint32_t)(x) >> (32 - (n))))
#define TETRA_ROTR64(x, n) \
    (((uint64_t)(x) >> (n)) | ((uint64_t)(x) << (64 - (n))))

#define rol32(x, n) TETRA_ROTL32(x, n)

static inline uint64_t tetra_expand_iv64(uint32_t iv, uint32_t xor_const)
{
    uint32_t x = iv ^ xor_const;
    x = TETRA_ROTL32(x, 8);
    uint64_t qw = ((uint64_t)iv << 32) | x;
    return TETRA_ROTR64(qw, 8);
}

#define BYTE0(x) ((x) & 0xff)
#define BYTE1(x) (((x) >> 8) & 0xff)
#define BYTE2(x) (((x) >> 16) & 0xff)
#define BYTE3(x) (((x) >> 24) & 0xff)
#define BYTE4(x) (((x) >> 32) & 0xff)
#define BYTE5(x) (((x) >> 40) & 0xff)
#define BYTE6(x) (((x) >> 48) & 0xff)
#define BYTE7(x) (((x) >> 56) & 0xff)

#define BYTEN(x,n) (((x) >> (n * 8)) & 0xff)

#if TETRA_COMMON_IS_LE
#define be32(x) __builtin_bswap32((uint32_t)(x))
#define be16(x) __builtin_bswap16((uint16_t)(x))
#define le32(x) ((uint32_t)(x))
#define le16(x) ((uint16_t)(x))
#else
#define be32(x) ((uint32_t)(x))
#define be16(x) ((uint16_t)(x))
#define le32(x) __builtin_bswap32((uint32_t)(x))
#define le16(x) __builtin_bswap16((uint16_t)(x))
#endif

#define TETRA_HOST_LITTLE_ENDIAN TETRA_COMMON_IS_LE
#undef TETRA_COMMON_IS_LE

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t  tn; // timeslot, 1 to 4
    uint8_t  fn; // frame, 1 to 18
    uint8_t  mn; // multiframe, 1 to 60
    uint16_t hn; // hyperframe, 0 to 0xFFFF, although only 15 bits are used
    uint8_t  dir; // 0 or 1; 0 = downlink, 1 = uplink
} FrameNumbers;


uint32_t build_iv(const FrameNumbers *f);

/* Decode even-length hex into buf; returns byte count or -1 on error. */
int tetra_hex_decode(const char *hex, uint8_t *buf, unsigned buf_cap);

#ifdef __cplusplus
}
#endif

#endif /* HAVE_COMMON_H */
