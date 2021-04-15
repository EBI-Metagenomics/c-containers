#ifndef HASH_H
#define HASH_H

#include "container.h"
#include "misc.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct hash_list {
  struct hash_node *first;
};

struct hash_node {
  struct hash_node *next, **pprev;
};

#define HASH_LIST_INIT(ptr) ((ptr)->first = NULL)

#define HASH_DECLARE(name, bits) struct hash_list name[1 << (bits)]

#define HASH_NODE_INIT()                                                       \
  { NULL, NULL }

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
static inline void __hash_init(struct hash_list *ht, unsigned sz) {
  for (unsigned i = 0; i < sz; i++)
    HASH_LIST_INIT(&ht[i]);
}

#define hash_init(hashtable) __hash_init(hashtable, HASH_SIZE(hashtable))

/**
 * hash_list_empty - Is the specified hash_list structure an empty hash_list?
 * @h: Structure to check.
 */
static inline int hash_list_empty(const struct hash_list *h) {
  return !h->first;
}

/**
 * hash_add - add an object to a hashtable
 * @hashtable: hashtable to add to
 * @node: the &struct hash_node of the object to be added
 * @key: the key of the object to be added
 */
#define hash_add(hashtable, node, key)                                         \
  hash_list_add(node, &hashtable[hash_min(key, HASH_BITS(hashtable))])

/**
 * hash_list_add - add a new entry at the beginning of the hlist
 * @n: new entry to be added
 * @h: hlist head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void hash_list_add(struct hash_node *n, struct hash_list *h) {
  struct hash_node *first = h->first;
  n->next = first;
  if (first)
    first->pprev = &n->next;
  h->first = n;
  n->pprev = &h->first;
}

static inline bool __hash_empty(struct hash_list *ht, unsigned sz) {
  for (unsigned i = 0; i < sz; i++)
    if (!hash_list_empty(&ht[i]))
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
 * hash_node_unhashed - Has node been removed from list and reinitialized?
 * @h: Node to be checked
 *
 * Not that not all removal functions will leave a node in unhashed
 * state.
 */
static inline int hash_node_unhashed(const struct hash_node *h) {
  return !h->pprev;
}

static inline void __hash_node_del(struct hash_node *n) {
  struct hash_node *next = n->next;
  struct hash_node **pprev = n->pprev;

  *pprev = next;
  if (next)
    next->pprev = pprev;
}

static inline void INIT_HLIST_NODE(struct hash_node *h) {
  h->next = NULL;
  h->pprev = NULL;
}

/**
 * hash_node_del_init - Delete the specified hash_node from its list and
 * initialize
 * @n: Node to delete.
 *
 * Note that this function leaves the node in unhashed state.
 */
static inline void hash_node_del_init(struct hash_node *n) {
  if (!hash_node_unhashed(n)) {
    __hash_node_del(n);
    INIT_HLIST_NODE(n);
  }
}

/**
 * hash_del - remove an object from a hashtable
 * @node: &struct hash_node of the object to remove
 */
static inline void hash_del(struct hash_node *node) {
  hash_node_del_init(node);
}

#define hlist_entry(ptr, type, member) container_of(ptr, type, member)

#define hlist_entry_safe(ptr, type, member)                                    \
  ({                                                                           \
    __typeof__(ptr) ____ptr = (ptr);                                           \
    ____ptr ? hlist_entry(____ptr, type, member) : NULL;                       \
  })

/**
 * hlist_for_each_entry	- iterate over list of given type
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the hash_node within the struct.
 */
#define hlist_for_each_entry(pos, head, member)                                \
  for (pos = hlist_entry_safe((head)->first, __typeof__(*(pos)), member); pos; \
       pos = hlist_entry_safe((pos)->member.next, __typeof__(*(pos)), member))

/**
 * hash_for_each_safe - iterate over a hashtable safe against removal of
 * hash entry
 * @name: hashtable to iterate
 * @bkt: integer to use as bucket loop cursor
 * @tmp: a &struct hash_node used for temporary storage
 * @obj: the type * to use as a loop cursor for each entry
 * @member: the name of the hash_node within the struct
 */
#define hash_for_each_safe(name, bkt, tmp, obj, member)                        \
  for ((bkt) = 0, obj = NULL; obj == NULL && (bkt) < HASH_SIZE(name); (bkt)++) \
  hlist_for_each_entry_safe(obj, tmp, &name[bkt], member)

/**
 * hash_for_each_possible - iterate over all possible objects hashing to the
 * same bucket
 * @name: hashtable to iterate
 * @obj: the type * to use as a loop cursor for each entry
 * @member: the name of the hash_node within the struct
 * @key: the key of the objects to iterate over
 */
#define hash_for_each_possible(name, obj, member, key)                         \
  hlist_for_each_entry(obj, &name[hash_min(key, HASH_BITS(name))], member)

/**
 * hash_for_each_possible_safe - iterate over all possible objects hashing to
 * the same bucket safe against removals
 * @name: hashtable to iterate
 * @obj: the type * to use as a loop cursor for each entry
 * @tmp: a &struct hash_node used for temporary storage
 * @member: the name of the hash_node within the struct
 * @key: the key of the objects to iterate over
 */
#define hash_for_each_possible_safe(name, obj, tmp, member, key)               \
  hlist_for_each_entry_safe(obj, tmp, &name[hash_min(key, HASH_BITS(name))],   \
                            member)

/**
 * hlist_for_each_entry_safe - iterate over list of given type safe against
 * removal of list entry
 * @pos:	the type * to use as a loop cursor.
 * @n:		a &struct hash_node to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the hash_node within the struct.
 */
#define hlist_for_each_entry_safe(pos, n, head, member)                        \
  for (pos = hlist_entry_safe((head)->first, __typeof__(*pos), member);        \
       pos && ({                                                               \
         n = pos->member.next;                                                 \
         1;                                                                    \
       });                                                                     \
       pos = hlist_entry_safe(n, __typeof__(*pos), member))

/**
 * hash_for_each - iterate over a hashtable
 * @name: hashtable to iterate
 * @bkt: integer to use as bucket loop cursor
 * @obj: the type * to use as a loop cursor for each entry
 * @member: the name of the hash_node within the struct
 */
#define hash_for_each(name, bkt, obj, member)                                  \
  for ((bkt) = 0, obj = NULL; obj == NULL && (bkt) < HASH_SIZE(name); (bkt)++) \
  hlist_for_each_entry(obj, &name[bkt], member)

#endif
