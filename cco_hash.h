#ifndef CCO_HASH_H
#define CCO_HASH_H

#include "cco_hash_support.h"
#include "cco_hlist.h"
#include "cco_of.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define CCO_HASH_DECLARE(name, bits) struct cco_hlist name[1 << (bits)]

/**
 * cco_hash_init - initialize a hash table
 * @ht: hashtable to be initialized
 *
 * Calculates the size of the hashtable from the given parameter, otherwise
 * same as hash_init_size.
 *
 * This has to be a macro since CCO_HASH_BITS() will not work on pointers since
 * it calculates the size during preprocessing.
 */
static inline void __cco_hash_init(struct cco_hlist *ht, unsigned sz) {
  for (unsigned i = 0; i < sz; i++)
    CCO_HLIST_INIT(&ht[i]);
}

#define cco_hash_init(ht) __cco_hash_init(ht, CCO_HASH_SIZE(ht))

/**
 * cco_hash_add - add an object to a hashtable
 * @ht: hashtable to add to
 * @node: the &struct hnode of the object to be added
 * @key: the key of the object to be added
 */
#define cco_hash_add(ht, node, key)                                            \
  __cco_hlist_add(node, &ht[cco_hash_min(key, CCO_HASH_BITS(ht))])

static inline bool __cco_hash_empty(struct cco_hlist *ht, unsigned sz) {
  for (unsigned i = 0; i < sz; i++)
    if (!cco_hlist_empty(&ht[i]))
      return false;

  return true;
}

/**
 * cco_hash_empty - check whether a hashtable is empty
 * @ht: hashtable to check
 *
 * This has to be a macro since CCO_HASH_BITS() will not work on pointers since
 * it calculates the size during preprocessing.
 */
#define cco_hash_empty(ht) __cco_hash_empty(ht, CCO_HASH_SIZE(ht))

/**
 * cco_hash_del - remove an object from a hashtable
 * @node: &struct hnode of the object to remove
 */
static inline void cco_hash_del(struct cco_hnode *node) {
  cco_hnode_del_init(node);
}

/**
 * cco_hash_hashed - check whether an object is in any hashtable
 * @node: the &struct hlist_node of the object to be checked
 */
static inline bool cco_hash_hashed(struct cco_hnode const *node) {
  return !cco_hlist_unhashed(node);
}

/**
 * cco_hash_for_each_safe - iterate over a hashtable safe against removal of
 * hash entry
 * @name: hashtable to iterate
 * @bkt: integer to use as bucket loop cursor
 * @tmp: a &struct hnode used for temporary storage
 * @obj: the type * to use as a loop cursor for each entry
 * @member: the name of the hnode within the struct
 */
#define cco_hash_for_each_safe(name, bkt, tmp, obj, member)                    \
  for ((bkt) = 0, obj = NULL; obj == NULL && (bkt) < CCO_HASH_SIZE(name);      \
       (bkt)++)                                                                \
  __cco_hlist_for_each_entry_safe(obj, tmp, &name[bkt], member)

/**
 * cco_hash_for_each_possible - iterate over all possible objects hashing to the
 * same bucket
 * @name: hashtable to iterate
 * @obj: the type * to use as a loop cursor for each entry
 * @member: the name of the hnode within the struct
 * @key: the key of the objects to iterate over
 */
#define cco_hash_for_each_possible(name, obj, member, key)                     \
  __cco_hlist_for_each_entry(                                                  \
      obj, &name[cco_hash_min(key, CCO_HASH_BITS(name))], member)

/**
 * cco_hash_for_each_possible_safe - iterate over all possible objects hashing
 * to the same bucket safe against removals
 * @name: hashtable to iterate
 * @obj: the type * to use as a loop cursor for each entry
 * @tmp: a &struct hnode used for temporary storage
 * @member: the name of the hnode within the struct
 * @key: the key of the objects to iterate over
 */
#define cco_hash_for_each_possible_safe(name, obj, tmp, member, key)           \
  __cco_hlist_for_each_entry_safe(                                             \
      obj, tmp, &name[cco_hash_min(key, CCO_HASH_BITS(name))], member)

/**
 * cco_hash_for_each - iterate over a hashtable
 * @name: hashtable to iterate
 * @bkt: integer to use as bucket loop cursor
 * @obj: the type * to use as a loop cursor for each entry
 * @member: the name of the hnode within the struct
 */
#define cco_hash_for_each(name, bkt, obj, member)                              \
  for ((bkt) = 0, obj = NULL; obj == NULL && (bkt) < CCO_HASH_SIZE(name);      \
       (bkt)++)                                                                \
  __cco_hlist_for_each_entry(obj, &name[bkt], member)

#endif
