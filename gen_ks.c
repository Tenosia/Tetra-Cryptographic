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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "taa1.h"
#include "tea1.h"
#include "tea2.h"
#include "tea3.h"

#define KS_LEN 54

static void print_hex(const uint8_t *buf, int len)
{
    int i;
    for (i = 0; i < len; i++) {
        printf("%02X", buf[i]);
    }
}

int main(int argc, const char *argv[])
{
    int dwTeaType;
    FrameNumbers f;
    uint8_t abCk[10];
    int dwCn, dwLa, dwCc;
    int bTea1Reduced = 0;
    int dwKeyLen;
    uint8_t abKs[KS_LEN];
    uint32_t dwIv;

    if (argc != 3 && argc < 7) {
        printf("[+] TEA keystream generation\n");
        printf("    Usage: %s tea_type  hn mn fn sn dir  eck\n", argv[0]);
        printf("    Usage: %s tea_type  hn mn fn sn dir  ck  cn la cc\n", argv[0]);
        printf("    TEA1 reduced key is supported\n");
        printf("    direction 0 = downlink, 1 = uplink\n");
        printf("    Example vector:\n");
        printf("      %s 2  110 30 06 1  0  11111111111111111111  1001 2 1\n", argv[0]);
        printf("      %s 1  110 30 06 1  0  11111111\n", argv[0]);
        return 1;
    }

    if (sscanf(argv[1], "%d", &dwTeaType) != 1 || dwTeaType < 1 || dwTeaType > 3) {
        printf("[-] TEA type not supported\n");
        return 1;
    }

    if (    sscanf(argv[2], "%hd", &f.hn) != 1 ||
            sscanf(argv[3], "%hhd", &f.mn) != 1 ||
            sscanf(argv[4], "%hhd", &f.fn) != 1 ||
            sscanf(argv[5], "%hhd", &f.tn) != 1 ||
            sscanf(argv[6], "%hhd", &f.dir) != 1) {
        printf("[-] Can't parse hn/mn/fn/sn\n");
        return 1;
    }

    memset(abCk, 0, sizeof(abCk));

    if (dwTeaType == 1 && strlen(argv[7]) == 8) {
        bTea1Reduced = 1;
    } else if (strlen(argv[7]) != 20) {
        printf("[-] Invalid length key\n");
        return 1;
    }

    dwKeyLen = tetra_hex_decode(argv[7], abCk, sizeof(abCk));
    if (dwKeyLen < 0) {
        printf("[-] Can't parse key\n");
        return 1;
    }

    if (!bTea1Reduced) {
        if (    argc != 11 ||
                sscanf(argv[8], "%d", &dwCn) != 1 ||
                sscanf(argv[9], "%d", &dwLa) != 1 ||
                sscanf(argv[10], "%d", &dwCc) != 1) {
            printf("[-] Can't parse cn/la/cc\n");
            return 1;
        }
    }

    dwIv = build_iv(&f);

    if (bTea1Reduced) {
        uint64_t expIv = tea1_expand_iv(dwIv);
        uint32_t dwKeyReg = ((uint32_t)abCk[0] << 24) | ((uint32_t)abCk[1] << 16)
                          | ((uint32_t)abCk[2] << 8) | abCk[3];
        printf("TEA1_reduced hn %d mn %d fn %d tn %d eck %02X%02X%02X%02X\n",
            f.hn, f.mn, f.fn, f.tn, abCk[0], abCk[1], abCk[2], abCk[3]);
        tea1_inner(expIv, dwKeyReg, KS_LEN, abKs);
    } else {
        uint8_t abEck[10];
        uint8_t abCn[2] = {(uint8_t)((dwCn >> 8) & 0xFF), (uint8_t)(dwCn & 0xFF)};
        uint8_t abLa[2] = {(uint8_t)((dwLa >> 8) & 0xFF), (uint8_t)(dwLa & 0xFF)};
        uint8_t abCc[1] = {(uint8_t)(dwCc & 0xFF)};

        tb5(abCn, abLa, abCc, abCk, abEck);

        printf("TEA%d hn %d mn %d fn %d tn %d ", dwTeaType, f.hn, f.mn, f.fn, f.tn);
        printf("     cn %d la %d cc %d ck ", dwCn, dwLa, dwCc);
        print_hex(abCk, 10);
        printf("\n     eck ");
        print_hex(abEck, 10);
        printf("     iv 0x%08X\n     ks ", dwIv);

        switch (dwTeaType) {
        case 1:
            tea1(dwIv, abEck, KS_LEN, abKs);
            break;
        case 2:
            tea2(dwIv, abEck, KS_LEN, abKs);
            break;
        case 3:
            tea3(dwIv, abEck, KS_LEN, abKs);
            break;
        default:
            break;
        }
    }

    print_hex(abKs, KS_LEN);
    printf("\n");
    return 0;
}
