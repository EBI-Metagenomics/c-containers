#ifndef CONTAINERS_HASH_H
#define CONTAINERS_HASH_H

#include "containers/container_of.h"
#include "containers/hash_support.h"
#include "containers/hlist.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define HASH_DECLARE(name, bits) struct hlist name[1 << (bits)]

/**
 * hash_init - initialize a hash table
 * @hashtable: hashtable to be initialized
 *
 * Calculates the size of the hashtable from the given parameter, otherwise
 * same as hash_init_size.
 *
 * This has to be a macro since HASH_BITS() will not work on pointers since
 * it calculates the size during preprocessing.
 */
static inline void __hash_init(struct hlist *ht, unsigned sz) {
  for (unsigned i = 0; i < sz; i++)
    HLIST_INIT(&ht[i]);
}

#define hash_init(hashtable) __hash_init(hashtable, HASH_SIZE(hashtable))

/**
 * hash_add - add an object to a hashtable
 * @hashtable: hashtable to add to
 * @node: the &struct hnode of the object to be added
 * @key: the key of the object to be added
 */
#define hash_add(hashtable, node, key)                                         \
  __hlist_add(node, &hashtable[hash_min(key, HASH_BITS(hashtable))])

static inline bool __hash_empty(struct hlist *ht, unsigned sz) {
  for (unsigned i = 0; i < sz; i++)
    if (!hlist_empty(&ht[i]))
      return false;

  return true;
}

/**
 * hash_empty - check whether a hashtable is empty
 * @hashtable: hashtable to check
 *
 * This has to be a macro since HASH_BITS() will not work on pointers since
 * it calculates the size during preprocessing.
 */
#define hash_empty(hashtable) __hash_empty(hashtable, HASH_SIZE(hashtable))

/**
 * hash_del - remove an object from a hashtable
 * @node: &struct hnode of the object to remove
 */
static inline void hash_del(struct hnode *node) { hnode_del_init(node); }

/**
 * hash_for_each_safe - iterate over a hashtable safe against removal of
 * hash entry
 * @name: hashtable to iterate
 * @bkt: integer to use as bucket loop cursor
 * @tmp: a &struct hnode used for temporary storage
 * @obj: the type * to use as a loop cursor for each entry
 * @member: the name of the hnode within the struct
 */
#define hash_for_each_safe(name, bkt, tmp, obj, member)                        \
  for ((bkt) = 0, obj = NULL; obj == NULL && (bkt) < HASH_SIZE(name); (bkt)++) \
  __hlist_for_each_entry_safe(obj, tmp, &name[bkt], member)

/**
 * hash_for_each_possible - iterate over all possible objects hashing to the
 * same bucket
 * @name: hashtable to iterate
 * @obj: the type * to use as a loop cursor for each entry
 * @member: the name of the hnode within the struct
 * @key: the key of the objects to iterate over
 */
#define hash_for_each_possible(name, obj, member, key)                         \
  __hlist_for_each_entry(obj, &name[hash_min(key, HASH_BITS(name))], member)

/**
 * hash_for_each_possible_safe - iterate over all possible objects hashing to
 * the same bucket safe against removals
 * @name: hashtable to iterate
 * @obj: the type * to use as a loop cursor for each entry
 * @tmp: a &struct hnode used for temporary storage
 * @member: the name of the hnode within the struct
 * @key: the key of the objects to iterate over
 */
#define hash_for_each_possible_safe(name, obj, tmp, member, key)               \
  __hlist_for_each_entry_safe(obj, tmp, &name[hash_min(key, HASH_BITS(name))], \
                              member)

/**
 * hash_for_each - iterate over a hashtable
 * @name: hashtable to iterate
 * @bkt: integer to use as bucket loop cursor
 * @obj: the type * to use as a loop cursor for each entry
 * @member: the name of the hnode within the struct
 */
#define hash_for_each(name, bkt, obj, member)                                  \
  for ((bkt) = 0, obj = NULL; obj == NULL && (bkt) < HASH_SIZE(name); (bkt)++) \
  __hlist_for_each_entry(obj, &name[bkt], member)

#endif
