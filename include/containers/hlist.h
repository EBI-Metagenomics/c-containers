#ifndef CONTAINERS_HLIST_H
#define CONTAINERS_HLIST_H

#include "containers/hnode.h"

struct hlist {
  struct hnode *first;
};

#define HLIST_INIT(ptr) ((ptr)->first = NULL)

/**
 * hlist_empty - Is the specified hlist structure an empty hlist?
 * @h: Structure to check.
 */
static inline int hlist_empty(const struct hlist *h) { return !h->first; }

/**
 * __hlist_add - add a new entry at the beginning of the hlist
 * @n: new entry to be added
 * @h: hlist head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void __hlist_add(struct hnode *n, struct hlist *h) {
  struct hnode *first = h->first;
  n->next = first;
  if (first)
    first->pprev = &n->next;
  h->first = n;
  n->pprev = &h->first;
}

/**
 * hlist_unhashed - Has node been removed from list and reinitialized?
 * @h: Node to be checked
 *
 * Not that not all removal functions will leave a node in unhashed
 * state.  For example, hlist_nulls_del_init_rcu() does leave the
 * node in unhashed state, but hlist_nulls_del() does not.
 */
static inline int hlist_unhashed(struct hnode const *h) { return !h->pprev; }

#define hlist_entry(ptr, type, member) container_of(ptr, type, member)

#define hlist_entry_safe(ptr, type, member)                                    \
  ({                                                                           \
    __typeof__(ptr) ____ptr = (ptr);                                           \
    ____ptr ? hlist_entry(____ptr, type, member) : NULL;                       \
  })

/**
 * __hlist_for_each_entry	- iterate over list of given type
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the hnode within the struct.
 */
#define __hlist_for_each_entry(pos, head, member)                              \
  for (pos = hlist_entry_safe((head)->first, __typeof__(*(pos)), member); pos; \
       pos = hlist_entry_safe((pos)->member.next, __typeof__(*(pos)), member))

/**
 * __hlist_for_each_entry_safe - iterate over list of given type safe against
 * removal of list entry
 * @pos:	the type * to use as a loop cursor.
 * @n:		a &struct hnode to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the hnode within the struct.
 */
#define __hlist_for_each_entry_safe(pos, n, head, member)                      \
  for (pos = hlist_entry_safe((head)->first, __typeof__(*pos), member);        \
       pos && ({                                                               \
         n = pos->member.next;                                                 \
         1;                                                                    \
       });                                                                     \
       pos = hlist_entry_safe(n, __typeof__(*pos), member))

#endif
