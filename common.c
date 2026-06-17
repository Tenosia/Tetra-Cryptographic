// Copyright 2023, Midnight Blue.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <assert.h>
#include <string.h>

#include "common.h"

int frame_numbers_valid(const FrameNumbers *f)
{
    if (!f) {
        return 0;
    }
    if (f->tn < TETRA_TN_MIN || f->tn > TETRA_TN_MAX) {
        return 0;
    }
    if (f->fn < TETRA_FN_MIN || f->fn > TETRA_FN_MAX) {
        return 0;
    }
    if (f->mn < TETRA_MN_MIN || f->mn > TETRA_MN_MAX) {
        return 0;
    }
    if (f->dir > TETRA_DIR_UPLINK) {
        return 0;
    }
    return 1;
}

uint32_t build_iv(const FrameNumbers *f)
{
#ifndef NDEBUG
    assert(frame_numbers_valid(f));
#else
    if (!frame_numbers_valid(f)) {
        return 0;
    }
#endif
    return ((uint32_t)(f->tn - 1)
            | ((uint32_t)f->fn << 2)
            | ((uint32_t)f->mn << 7)
            | ((uint32_t)(f->hn & 0x7FFF) << 13)
            | ((uint32_t)f->dir << 28));
}

static int tetra_hex_nibble(char c)
{
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    }
    if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    }
    return -1;
}

int tetra_hex_decode(const char *hex, uint8_t *buf, unsigned buf_cap)
{
    size_t len;
    unsigned i;
    int hi;
    int lo;

    if (!hex || !buf) {
        return -1;
    }
    len = strlen(hex);
    if (len == 0 || (len & 1u) != 0) {
        return -1;
    }
    if (len / 2 > buf_cap) {
        return -1;
    }

    for (i = 0; i < len; i += 2) {
        hi = tetra_hex_nibble(hex[i]);
        lo = tetra_hex_nibble(hex[i + 1]);
        if (hi < 0 || lo < 0) {
            return -1;
        }
        buf[i / 2] = (uint8_t)((hi << 4) | lo);
    }
    return (int)(len / 2);
}
