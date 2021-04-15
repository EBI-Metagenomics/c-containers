#ifndef HASH_H
#define HASH_H

#include "misc.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define HASH_BITS(name) ilog2(HASH_SIZE(name))

struct hlist_head
{
    struct hlist_node* first;
};

struct hlist_node
{
    struct hlist_node *next, **pprev;
};

/* Use hash_32 when possible to allow for fast 32bit hashing in 64bit kernels. */
#define hash_min(val, bits) (sizeof(val) <= 4 ? hash_32(val, bits) : hash_64(val, bits))

#define HASH_SIZE(name) (ARRAY_SIZE(name))

#define INIT_HLIST_HEAD(ptr) ((ptr)->first = NULL)

#define DECLARE_HASHTABLE(name, bits) struct hlist_head name[1 << (bits)]

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
static inline void __hash_init(struct hlist_head* ht, unsigned sz)
{
    unsigned i;

    for (i = 0; i < sz; i++)
        INIT_HLIST_HEAD(&ht[i]);
}

/**
 * hlist_empty - Is the specified hlist_head structure an empty hlist?
 * @h: Structure to check.
 */
static inline int hlist_empty(const struct hlist_head* h) { return !h->first; }

#define hash_init(hashtable) __hash_init(hashtable, HASH_SIZE(hashtable))

/**
 * hash_add - add an object to a hashtable
 * @hashtable: hashtable to add to
 * @node: the &struct hlist_node of the object to be added
 * @key: the key of the object to be added
 */
#define hash_add(hashtable, node, key) hlist_add_head(node, &hashtable[hash_min(key, HASH_BITS(hashtable))])

/**
 * hlist_add_head - add a new entry at the beginning of the hlist
 * @n: new entry to be added
 * @h: hlist head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void hlist_add_head(struct hlist_node* n, struct hlist_head* h)
{
    struct hlist_node* first = h->first;
    n->next = first;
    if (first)
        first->pprev = &n->next;
    h->first = n;
    n->pprev = &h->first;
}

static inline bool __hash_empty(struct hlist_head* ht, unsigned sz)
{
    unsigned i;

    for (i = 0; i < sz; i++)
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
 * hlist_unhashed - Has node been removed from list and reinitialized?
 * @h: Node to be checked
 *
 * Not that not all removal functions will leave a node in unhashed
 * state.  For example, hlist_nulls_del_init_rcu() does leave the
 * node in unhashed state, but hlist_nulls_del() does not.
 */
static inline int hlist_unhashed(const struct hlist_node* h) { return !h->pprev; }

static inline void __hlist_del(struct hlist_node* n)
{
    struct hlist_node*  next = n->next;
    struct hlist_node** pprev = n->pprev;

    *pprev = next;
    if (next)
        next->pprev = pprev;
}

static inline void INIT_HLIST_NODE(struct hlist_node* h)
{
    h->next = NULL;
    h->pprev = NULL;
}

/**
 * hlist_del_init - Delete the specified hlist_node from its list and initialize
 * @n: Node to delete.
 *
 * Note that this function leaves the node in unhashed state.
 */
static inline void hlist_del_init(struct hlist_node* n)
{
    if (!hlist_unhashed(n)) {
        __hlist_del(n);
        INIT_HLIST_NODE(n);
    }
}

/**
 * hash_del - remove an object from a hashtable
 * @node: &struct hlist_node of the object to remove
 */
static inline void hash_del(struct hlist_node* node) { hlist_del_init(node); }

#define hlist_entry(ptr, type, member) CONTAINER_OF(ptr, type, member)

#define hlist_entry_safe(ptr, type, member)                                                                            \
    ({                                                                                                                 \
        __typeof__(ptr) ____ptr = (ptr);                                                                               \
        ____ptr ? hlist_entry(____ptr, type, member) : NULL;                                                           \
    })

/**
 * hlist_for_each_entry	- iterate over list of given type
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the hlist_node within the struct.
 */
#define hlist_for_each_entry(pos, head, member)                                                                        \
    for (pos = hlist_entry_safe((head)->first, __typeof__(*(pos)), member); pos;                                       \
         pos = hlist_entry_safe((pos)->member.next, __typeof__(*(pos)), member))

/**
 * hash_for_each_safe - iterate over a hashtable safe against removal of
 * hash entry
 * @name: hashtable to iterate
 * @bkt: integer to use as bucket loop cursor
 * @tmp: a &struct hlist_node used for temporary storage
 * @obj: the type * to use as a loop cursor for each entry
 * @member: the name of the hlist_node within the struct
 */
#define hash_for_each_safe(name, bkt, tmp, obj, member)                                                                \
    for ((bkt) = 0, obj = NULL; obj == NULL && (bkt) < HASH_SIZE(name); (bkt)++)                                       \
    hlist_for_each_entry_safe(obj, tmp, &name[bkt], member)

/**
 * hash_for_each_possible - iterate over all possible objects hashing to the
 * same bucket
 * @name: hashtable to iterate
 * @obj: the type * to use as a loop cursor for each entry
 * @member: the name of the hlist_node within the struct
 * @key: the key of the objects to iterate over
 */
#define hash_for_each_possible(name, obj, member, key)                                                                 \
    hlist_for_each_entry(obj, &name[hash_min(key, HASH_BITS(name))], member)

/**
 * hash_for_each_possible_safe - iterate over all possible objects hashing to the
 * same bucket safe against removals
 * @name: hashtable to iterate
 * @obj: the type * to use as a loop cursor for each entry
 * @tmp: a &struct hlist_node used for temporary storage
 * @member: the name of the hlist_node within the struct
 * @key: the key of the objects to iterate over
 */
#define hash_for_each_possible_safe(name, obj, tmp, member, key)                                                       \
    hlist_for_each_entry_safe(obj, tmp, &name[hash_min(key, HASH_BITS(name))], member)

/**
 * hlist_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:	the type * to use as a loop cursor.
 * @n:		a &struct hlist_node to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the hlist_node within the struct.
 */
#define hlist_for_each_entry_safe(pos, n, head, member)                                                                \
    for (pos = hlist_entry_safe((head)->first, __typeof__(*pos), member); pos && ({                                    \
                                                                              n = pos->member.next;                    \
                                                                              1;                                       \
                                                                          });                                          \
         pos = hlist_entry_safe(n, __typeof__(*pos), member))

/**
 * hash_for_each - iterate over a hashtable
 * @name: hashtable to iterate
 * @bkt: integer to use as bucket loop cursor
 * @obj: the type * to use as a loop cursor for each entry
 * @member: the name of the hlist_node within the struct
 */
#define hash_for_each(name, bkt, obj, member)                                                                          \
    for ((bkt) = 0, obj = NULL; obj == NULL && (bkt) < HASH_SIZE(name); (bkt)++)                                       \
    hlist_for_each_entry(obj, &name[bkt], member)

#endif
