#ifndef HAVE_COMMON_H
#define HAVE_COMMON_H

#include <stdint.h>
#include <string.h>

#if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && defined(__ORDER_BIG_ENDIAN__)
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define TETRA_COMMON_IS_LE 1
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define TETRA_COMMON_IS_LE 0
#else
#error "common.h: unsupported __BYTE_ORDER__"
#endif
#elif defined(_WIN32) || defined(__CYGWIN__)
#define TETRA_COMMON_IS_LE 1
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

#define TETRA_DIR_DOWNLINK 0u
#define TETRA_DIR_UPLINK   1u

#define TETRA_TN_MIN 1u
#define TETRA_TN_MAX 4u
#define TETRA_FN_MIN 1u
#define TETRA_FN_MAX 18u
#define TETRA_MN_MIN 1u
#define TETRA_MN_MAX 60u

static inline uint64_t tetra_expand_iv64(uint32_t iv, uint32_t xor_const)
{
    uint32_t x = iv ^ xor_const;
    x = TETRA_ROTL32(x, 8);
    uint64_t qw = ((uint64_t)iv << 32) | x;
    return TETRA_ROTR64(qw, 8);
}

static inline uint32_t tetra_read_u32(const uint8_t *p)
{
    uint32_t v;
    memcpy(&v, p, sizeof(v));
    return v;
}

static inline void tetra_write_u32(uint8_t *p, uint32_t v)
{
    memcpy(p, &v, sizeof(v));
}

static inline uint16_t tetra_read_u16(const uint8_t *p)
{
    uint16_t v;
    memcpy(&v, p, sizeof(v));
    return v;
}

static inline void tetra_write_u16(uint8_t *p, uint16_t v)
{
    memcpy(p, &v, sizeof(v));
}

static inline void tetra_write_u8(uint8_t *p, uint8_t v)
{
    *p = v;
}

#define BYTE0(x) ((x) & 0xff)
#define BYTE1(x) (((x) >> 8) & 0xff)
#define BYTE2(x) (((x) >> 16) & 0xff)
#define BYTE3(x) (((x) >> 24) & 0xff)
#define BYTE4(x) (((x) >> 32) & 0xff)
#define BYTE5(x) (((x) >> 40) & 0xff)
#define BYTE6(x) (((x) >> 48) & 0xff)
#define BYTE7(x) (((x) >> 56) & 0xff)

#define BYTEN(x, n) (((x) >> ((n) * 8)) & 0xff)

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
    uint8_t  tn; /* timeslot, 1 to 4 */
    uint8_t  fn; /* frame, 1 to 18 */
    uint8_t  mn; /* multiframe, 1 to 60 */
    uint16_t hn; /* hyperframe, 0 to 0xFFFF; only 15 bits used in IV */
    uint8_t  dir; /* TETRA_DIR_DOWNLINK or TETRA_DIR_UPLINK */
} FrameNumbers;

int frame_numbers_valid(const FrameNumbers *f);
uint32_t build_iv(const FrameNumbers *f);

/* Decode even-length hex into buf; returns byte count or -1 on error. */
int tetra_hex_decode(const char *hex, uint8_t *buf, unsigned buf_cap);

#ifdef __cplusplus
}
#endif

#endif /* HAVE_COMMON_H */
