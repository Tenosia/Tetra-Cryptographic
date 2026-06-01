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
#include <ctype.h>
#include <string.h>

#include "common.h"

uint32_t build_iv(const FrameNumbers *f)
{
#ifndef NDEBUG
    assert(1 <= f->tn  && f->tn  <= 4);
    assert(1 <= f->fn  && f->fn  <= 18);
    assert(1 <= f->mn  && f->mn  <= 60);
    assert(0 <= f->hn  && f->hn  <= 0xFFFF);
    assert(0 <= f->dir && f->dir <= 1);
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
    int hi, lo;

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
