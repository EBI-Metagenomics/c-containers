#ifndef CONTAINERS_HASH_SUPPORT_H
#define CONTAINERS_HASH_SUPPORT_H

#include <stdint.h>

/*
 * __has_builtin is supported on gcc >= 10, clang >= 3 and icc >= 21.
 * In the meantime, to support gcc < 10, we implement __has_builtin
 * by hand.
 */
#ifndef __has_builtin
#define __has_builtin(x) (0)
#endif

/**
 * __cco_fls32 - find last (most-significant) bit set
 * @x: the word to search
 *
 * This is defined the same way as ffs.
 * Note __cco_fls32(1) = 1, __cco_fls32(0x80000000) = 32.
 *
 * Undefined if no set bit exists, so code should check against 0 first.
 */
static inline unsigned __cco_fls32(uint32_t x) {

#if __has_builtin(__builtin_clz)
  return (unsigned)((int)sizeof(int) * 8 - __builtin_clz(x));
#else
  unsigned r = 32;

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
#endif
}

/**
 * __cco_fls64 - find last (most-significant) set bit in a long word
 * @x: the word to search
 *
 * Undefined if no set bit exists, so code should check against 0 first.
 */
static inline unsigned __cco_fls64(uint64_t x) {

#if __has_builtin(__builtin_clzl)
  uint32_t h = x >> 32;
  return h ? __cco_fls32(h) + 32 : __cco_fls32((uint32_t)x);
#else
  return (unsigned)((int)sizeof(long) * 8 - __builtin_clzl(x));
#endif
}

#define __CCO_ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

#define __CCO_UNSIGNED(x)                                                      \
  _Generic((x), char                                                           \
           : (unsigned char)(x), signed char                                   \
           : (unsigned char)(x), short                                         \
           : (unsigned short)(x), int                                          \
           : (unsigned int)(x), long                                           \
           : (unsigned long)(x), long long                                     \
           : (unsigned long long)(x), default                                  \
           : (unsigned int)(x))

/**
 * __cco_ilog2 - log base 2 of 32-bit or a 64-bit unsigned value
 * @n: parameter
 *
 * constant-capable log of base 2 calculation
 * - this can be used to initialise global variables from constant data, hence
 * the massive ternary operator construction
 *
 * selects the appropriately-sized optimised version depending on sizeof(n)
 */
#define __cco_ilog2(x)                                                         \
  (sizeof(x) <= 4 ? __cco_ilog2_u32(x) : __cco_ilog2_u64(x))

static inline unsigned __cco_ilog2_u32(uint32_t n) {
  return __cco_fls32(n) - 1;
}

static inline unsigned __cco_ilog2_u64(uint64_t n) {
  return __cco_fls64(n) - 1;
}

#define __CCO_GOLDEN_RATIO_32 0x61C88647
#define __CCO_GOLDEN_RATIO_64 0x61C8864680B583EBull

static inline uint32_t cco_hash_32(uint32_t val, unsigned bits) {
  /* High bits are more random, so use them. */
  return val * __CCO_GOLDEN_RATIO_32 >> (32 - bits);
}

static inline uint32_t cco_hash_64(uint64_t val, unsigned bits) {
  /* High bits are more random, so use them. */
  return (uint32_t)(val * __CCO_GOLDEN_RATIO_64 >> (64 - bits));
}

/* Use cco_hash_32 when possible to allow for fast 32bit hashing in 64bit
 * kernels.
 */
#define cco_hash_min(x, bits)                                                  \
  (sizeof(x) <= 4 ? cco_hash_32(__CCO_UNSIGNED(x), bits)                       \
                  : cco_hash_64(__CCO_UNSIGNED(x), bits))

#define CCO_HASH_BITS(name) __cco_ilog2(CCO_HASH_SIZE(name))
#define CCO_HASH_SIZE(name) (__CCO_ARRAY_SIZE(name))

#endif
