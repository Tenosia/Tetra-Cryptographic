# TETRA_crypto build
CC      ?= gcc
AR      ?= ar
ARFLAGS ?= rcs

LIB_SRCS := common.c hurdle.c tea1.c tea2.c tea3.c taa1.c
LIB_OBJS := $(LIB_SRCS:.c=.o)
TARGETS  := libtetracrypto.a tests gen_ks gen_ta61

BASE_CFLAGS := -std=c99 -Wall -Wextra -MMD -MP

ifdef DEBUG
CFLAGS := $(BASE_CFLAGS) -g -O0
else
CFLAGS := $(BASE_CFLAGS) -O3 -DNDEBUG
endif

LDFLAGS  ?=

.PHONY: all clean

all: $(TARGETS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

libtetracrypto.a: $(LIB_OBJS)
	$(AR) $(ARFLAGS) $@ $^

tests: libtetracrypto.a tests.o
	$(CC) $(LDFLAGS) -o $@ tests.o -L. -ltetracrypto

gen_ks: libtetracrypto.a gen_ks.o
	$(CC) $(LDFLAGS) -o $@ gen_ks.o -L. -ltetracrypto

gen_ta61: libtetracrypto.a gen_ta61.o
	$(CC) $(LDFLAGS) -o $@ gen_ta61.o -L. -ltetracrypto

clean:
	rm -f $(LIB_OBJS) tests.o gen_ks.o gen_ta61.o *.d $(TARGETS)

-include $(LIB_OBJS:.o=.d) tests.d gen_ks.d gen_ta61.d
