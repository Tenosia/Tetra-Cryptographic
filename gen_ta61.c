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

#include "common.h"
#include "taa1.h"

int main(int argc, const char *argv[])
{
    char bOp;
    uint8_t abCck[10];
    int dwKeyLen;
    int dwSsi;
    int dwSsiOut;
    uint8_t abSsi[3];
    uint8_t abInt[8];

    if (argc != 4) {
        printf("[+] TA61 identity encryption/decryption\n");
        printf("    Usage: %s e cck ssi\n", argv[0]);
        printf("    Usage: %s d cck esi\n", argv[0]);
        printf("    TA61 intermediate secret is supported\n");
        printf("    Example vectors:\n");
        printf("      %s e 11111111111111111111 100\n", argv[0]);
        printf("      %s d 11111111111111111111 12845063\n", argv[0]);
        return 1;
    }

    if (sscanf(argv[1], "%c", &bOp) != 1 || (bOp != 'e' && bOp != 'd')) {
        printf("[-] Can't parse operation type, should be e for encrypt or d for decrypt\n");
        return 1;
    }

    if (strlen(argv[2]) == 20) {
        dwKeyLen = 10;
    } else if (strlen(argv[2]) == 16) {
        dwKeyLen = 8;
    } else {
        printf("[-] Unexpected cck length, should be 8 or 10 bytes\n");
        return 1;
    }

    if (tetra_hex_decode(argv[2], abCck, sizeof(abCck)) != dwKeyLen) {
        printf("[-] Can't parse key\n");
        return 1;
    }

    if (sscanf(argv[3], "%d", &dwSsi) != 1 || dwSsi > 0xFFFFFF) {
        printf("[-] Can't parse SSI\n");
        return 1;
    }

    abSsi[0] = (uint8_t)((dwSsi >> 16) & 0xFF);
    abSsi[1] = (uint8_t)((dwSsi >> 8) & 0xFF);
    abSsi[2] = (uint8_t)(dwSsi & 0xFF);

    printf("TA61 ");

    if (bOp == 'e') {
        if (dwKeyLen == 10) {
            ta61_compute_c(abCck, abInt);
            ta61_inner(abInt, abSsi, abSsi);
            printf("intermediate: %02X%02X%02X%02X%02X%02X%02X%02X: ",
                abInt[0], abInt[1], abInt[2], abInt[3],
                abInt[4], abInt[5], abInt[6], abInt[7]);
        } else {
            ta61_inner(abCck, abSsi, abSsi);
        }
    } else {
        if (dwKeyLen == 10) {
            ta61_compute_c(abCck, abInt);
            ta61_inner_inv(abInt, abSsi, abSsi);
            printf("intermediate: %02X%02X%02X%02X%02X%02X%02X%02X: ",
                abInt[0], abInt[1], abInt[2], abInt[3],
                abInt[4], abInt[5], abInt[6], abInt[7]);
        } else {
            ta61_inner_inv(abCck, abSsi, abSsi);
        }
    }

    dwSsiOut = (abSsi[0] << 16) | (abSsi[1] << 8) | abSsi[2];
    printf("%d -> %d\n", dwSsi, dwSsiOut);
    return 0;
}
