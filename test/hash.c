#include "containers/containers.h"
#include <limits.h>
#include <stdlib.h>

static int errors = 0;

struct foo {
  int key;
  int val;
  struct cco_hnode node;
};

CCO_HASH_DECLARE(tbl_1, 1);
CCO_HASH_DECLARE(tbl_2, 2);
CCO_HASH_DECLARE(tbl_5, 5);
CCO_HASH_DECLARE(tbl_8, 8);
CCO_HASH_DECLARE(tbl_16, 16);

static void simple_tests(void);

static void size_test_1(unsigned nitems);
static void size_test_2(unsigned nitems);
static void size_test_5(unsigned nitems);
static void size_test_8(unsigned nitems);
static void size_test_16(unsigned nitems);

int main(void) {
  simple_tests();
  unsigned nitems[] = {0, 1, 2, 512, 1024, 32768, 65536};

  for (unsigned i = 0; i < 4; ++i) {
    size_test_1(nitems[i]);
    size_test_2(nitems[i]);
    size_test_5(nitems[i]);
  }

  for (unsigned i = 0; i < 5; ++i) {
    size_test_8(nitems[i]);
    size_test_16(nitems[i]);
  }

  for (unsigned i = 5; i < 7; ++i) {
    size_test_8(nitems[i]);
    size_test_16(nitems[i]);
  }
  return errors;
}

static void simple_tests(void) {

  CCO_HASH_DECLARE(tbl, 3);
  cco_hash_init(tbl);

  struct foo foo0 = {0, 0, CCO_HNODE_INIT()};
  struct foo foo1 = {1, 1, CCO_HNODE_INIT()};
  struct foo foo2 = {2, 2, CCO_HNODE_INIT()};
  struct foo foo3 = {-1, 3, CCO_HNODE_INIT()};

  errors += !cco_hash_empty(tbl);
  cco_hash_add(tbl, &foo0.node, foo0.key);
  cco_hash_add(tbl, &foo1.node, foo1.key);
  cco_hash_add(tbl, &foo2.node, foo2.key);
  cco_hash_add(tbl, &foo3.node, foo3.key);
  errors += cco_hash_empty(tbl);

  struct foo *foo = NULL;
  cco_hash_for_each_possible(tbl, foo, node, 2) {
    errors += foo->key != 2;
    errors += foo->val != 2;
  }

  cco_hash_for_each_possible(tbl, foo, node, 1) {
    errors += foo->key != 1;
    errors += foo->val != 1;
  }

  cco_hash_for_each_possible(tbl, foo, node, 0) {
    errors += foo->key != 0;
    errors += foo->val != 0;
  }

  struct foo foo4 = {-1, 4, CCO_HNODE_INIT()};
  cco_hash_add(tbl, &foo4.node, foo4.key);

  int count = 0;
  cco_hash_for_each_possible(tbl, foo, node, -1) {
    errors += foo->key != -1;
    errors += foo->val != 3 && foo->val != 4;
    ++count;
  }
  errors += count != 2;

  struct cco_hnode *tmp = NULL;
  cco_hash_for_each_possible_safe(tbl, foo, tmp, node, -1) {
    errors += foo->key != -1;
    if (foo->val == 4)
      cco_hash_del(&foo->node);
  }

  count = 0;
  cco_hash_for_each_possible(tbl, foo, node, -1) {
    errors += foo->key != -1;
    errors += foo->val != 3;
    ++count;
  }
  errors += count != 1;

  cco_hash_for_each_possible_safe(tbl, foo, tmp, node, -1) {
    errors += foo->key != -1;
    if (foo->val == 3)
      cco_hash_del(&foo->node);
  }

  unsigned bkt = 0;
  count = 0;
  cco_hash_for_each(tbl, bkt, foo, node) {
    errors += foo->key != foo->val;
    ++count;
  }
  errors += count != 3;

  cco_hash_for_each_safe(tbl, bkt, tmp, foo, node) { cco_hash_del(&foo->node); }

  count = 0;
  cco_hash_for_each(tbl, bkt, foo, node) { ++count; }
  errors += count != 0;
}

static inline uint32_t next_rand(uint32_t val) {
  static uint32_t const golden_ratio = 0x61C88647;
  return val * golden_ratio;
}

#define MAKE_SIZE_TEST(bits, size)                                             \
  static void size_test_##bits(unsigned nitems) {                              \
    cco_hash_init(tbl_##bits);                                                 \
                                                                               \
    errors += CCO_HASH_SIZE(tbl_##bits) != size;                               \
                                                                               \
    struct foo *foos = malloc(sizeof(*foos) * nitems);                         \
                                                                               \
    uint32_t key = next_rand(1);                                               \
    for (unsigned i = 0; i < nitems; ++i) {                                    \
      foos[i].key = (int)key;                                                  \
      foos[i].val = (int)i;                                                    \
      cco_hash_add(tbl_##bits, &foos[i].node, foos[i].key);                    \
      key = next_rand(key);                                                    \
    }                                                                          \
                                                                               \
    unsigned min_nitems = UINT_MAX;                                            \
    unsigned max_nitems = 0;                                                   \
                                                                               \
    for (unsigned i = 0; i < nitems; ++i) {                                    \
                                                                               \
      unsigned count = 0;                                                      \
      struct foo *foo = NULL;                                                  \
      cco_hash_for_each_possible(tbl_##bits, foo, node, i) { ++count; }        \
                                                                               \
      if (count < min_nitems)                                                  \
        min_nitems = count;                                                    \
                                                                               \
      if (count > max_nitems)                                                  \
        max_nitems = count;                                                    \
    }                                                                          \
    float freq = ((float)nitems) / CCO_HASH_SIZE(tbl_##bits);                  \
    float d = freq * 5 + 2;                                                    \
    unsigned left = (unsigned)(freq < d ? 0 : freq - d);                       \
    unsigned right = (unsigned)(freq + d);                                     \
                                                                               \
    errors += min_nitems < left;                                               \
    errors += max_nitems > right;                                              \
                                                                               \
    free(foos);                                                                \
  }

MAKE_SIZE_TEST(1, 2)
MAKE_SIZE_TEST(2, 4)
MAKE_SIZE_TEST(5, 32)
MAKE_SIZE_TEST(8, 256)
MAKE_SIZE_TEST(16, 65536)
