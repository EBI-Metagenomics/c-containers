#ifndef MISC_H
#define MISC_H

#include <stdint.h>

/**
 * fls32 - find last (most-significant) bit set
 * @x: the word to search
 *
 * This is defined the same way as ffs.
 * Note fls32(0) = 0, fls32(1) = 1, fls32(0x80000000) = 32.
 */
static inline int fls32(uint32_t x)
{
    int r = 32;

    if (!x)
        return 0;
    if (!(x & 0xffff0000u)) {
        x <<= 16;
        r -= 16;
    }
    if (!(x & 0xff000000u)) {
        x <<= 8;
        r -= 8;
    }
    if (!(x & 0xf0000000u)) {
        x <<= 4;
        r -= 4;
    }
    if (!(x & 0xc0000000u)) {
        x <<= 2;
        r -= 2;
    }
    if (!(x & 0x80000000u)) {
        x <<= 1;
        r -= 1;
    }
    return r;
}

static inline int fls64(uint64_t x)
{
    uint32_t h = x >> 32;
    return h ? fls32(h) + 32 : fls32(x);
}

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

/**
 * ilog2 - log base 2 of 32-bit or a 64-bit unsigned value
 * @n: parameter
 *
 * constant-capable log of base 2 calculation
 * - this can be used to initialise global variables from constant data, hence
 * the massive ternary operator construction
 *
 * selects the appropriately-sized optimised version depending on sizeof(n)
 */
#define ilog2(x) (sizeof(x) <= 4 ? ilog2_u32(x) : ilog2_u64(x))

static inline int ilog2_u32(uint32_t n) { return fls32(n) - 1; }

static inline int ilog2_u64(uint64_t n) { return fls64(n) - 1; }

#define GOLDEN_RATIO_32 0x61C88647
#define GOLDEN_RATIO_64 0x61C8864680B583EBull

static inline uint32_t hash_32(uint32_t val, unsigned bits)
{
    /* High bits are more random, so use them. */
    return val * GOLDEN_RATIO_32 >> (32 - bits);
}

static inline uint32_t hash_64(uint64_t val, unsigned bits)
{
    /* High bits are more random, so use them. */
    return val * GOLDEN_RATIO_64 >> (64 - bits);
}

#endif
