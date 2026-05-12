# TETRA_crypto

C reference implementation of cryptographic primitives used in TETRA digital radio systems. The code builds a static library plus small command-line tools and a test harness. Source files are copyright 2023 Midnight Blue and licensed under the Apache License, Version 2.0; see the LICENSE file in this repository.

## Scope

The project focuses on air-interface style algorithms: stream ciphers for keystream generation, a compact block cipher used in some modes, and TA or TB style transformations and primitives described in ETSI documentation for TETRA security (for example ETSI EN 300 392-7 for authentication and key-handling building blocks).

## Components

**Static library `libtetracrypto.a`** is built from:

- `common.c` / `common.h` — frame numbering (`FrameNumbers`), endian helpers, and `build_iv` to form a 32-bit IV from timeslot, frame, multiframe, hyperframe, and uplink or downlink direction.
- `hurdle.c` / `hurdle.h` — HURDLE 64-bit block cipher: key schedule, encrypt and decrypt, CBC encryption helper, CTS decryption helper, and published S-box tables.
- `tea1.c` / `tea1.h` — TEA1 keystream generator: `tea1` API, IV expansion, LUTs and S-box, and lower-level helpers exposed for analysis or tests.
- `tea2.c` / `tea2.h` — TEA2 keystream generator and IV expansion.
- `tea3.c` / `tea3.h` — TEA3 keystream generator.
- `taa1.c` / `taa1.h` — TA and TB primitives: bit-width transforms (80 to 120, 88 to 120, identity, and related variants), TA11 through TA92 style procedures, TB4 through TB7, plus TA61 internals (`ta61_compute_c`, `ta61_inner`, `ta61_inner_inv`) for cryptanalytic or tooling use.

**`tests`** — links against `libtetracrypto.a` and runs vector checks for HURDLE, TEA1, TEA2, TEA3, and the TA/TB routines in `tests.c`.

**`gen_ks`** — prints a TEA1, TEA2, or TEA3 keystream (54 bytes) for given air-interface frame parameters and keys. It can derive the encryption class key with `tb5` from CK and CN, LA, CC when using a full key path, or use a reduced four-byte TEA1 key mode. Source includes headers for Libsodium and some POSIX types; the provided Makefile only links the local static library, so those includes are not required at link time for the current code paths.

**`gen_ta61`** — encrypts or decrypts a 24-bit subscriber identity (SSI) under TA61, with optional use of the TA61 intermediate derived from a 10-byte CCK.

## Build

Requirements: a C compiler (the Makefile uses `gcc`), `make`, and `ar` for the static library. The code uses GCC builtins such as `__builtin_bswap32` and assumes a little-endian or big-endian host via `__BYTE_ORDER` when including `common.h`.

From the project root:

```
make
```

This produces `libtetracrypto.a`, `tests`, `gen_ks`, and `gen_ta61`.

Clean object files and built artifacts:

```
make clean
```

Compiler flags are set in the Makefile (`-Wall -O3 -g`). To use another compiler, override `CC` on the make command line.

## Run tests

After a successful build:

```
./tests
```

The program prints per-suite lines and marks each vector group as passed or failed.

## Command-line tools (summary)

**`gen_ks`** expects either a short TEA1 key form or the full parameter set. Direction is `0` for downlink and `1` for uplink. Consult the usage text printed when the program is run with the wrong number of arguments for the exact argument order (TEA type, hyperframe, multiframe, frame, slot, direction, key material, and when applicable CN, LA, CC as decimal values).

**`gen_ta61`** usage: first argument `e` to encrypt or `d` to decrypt; second argument is the CCK as hex (20 hex digits for 10 bytes or 16 for 8 bytes); third argument is the SSI as a decimal value up to 24 bits. Example forms are shown in the program’s own usage message.

Note: `gen_ks.c` and `gen_ta61.c` include POSIX headers (`unistd.h`, `sys/sysinfo.h`). Building those programs on Windows may require a POSIX-compatible toolchain or adjusting includes; the core library sources do not depend on those headers.

## Integrating the library

Link with `libtetracrypto.a` and add the project directory to the include path. Include only the headers you need: `hurdle.h`, `tea1.h`, `tea2.h`, `tea3.h`, `taa1.h`, and `common.h` as appropriate. Many symbols in the headers are documented as internal or for historical compatibility; prefer the primary APIs (`HURDLE_*`, `tea1`, `tea2`, `tea3`, and the `ta*` / `tb*` functions) unless you are extending tests or analysis code.

## Security and compliance note

These algorithms are specified for TETRA infrastructure and terminals. Export, import, and use of cryptographic software may be regulated in your jurisdiction. This repository is a software implementation for interoperability testing, research, and education; operators remain responsible for lawful use and for meeting applicable standards and certifications.
