#ifndef CCO_HLIST_H
#define CCO_HLIST_H

#include "cco/hnode.h"

struct cco_hlist {
  struct cco_hnode *first;
};

#define CCO_HLIST_INIT(ptr) ((ptr)->first = NULL)

/**
 * cco_hlist_empty - Is the specified hlist structure an empty hlist?
 * @h: Structure to check.
 */
static inline int cco_hlist_empty(const struct cco_hlist *h) {
  return !h->first;
}

/**
 * __cco_hlist_add - add a new entry at the beginning of the hlist
 * @n: new entry to be added
 * @h: hlist head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void __cco_hlist_add(struct cco_hnode *n, struct cco_hlist *h) {
  struct cco_hnode *first = h->first;
  n->next = first;
  if (first)
    first->pprev = &n->next;
  h->first = n;
  n->pprev = &h->first;
}

/**
 * cco_hlist_unhashed - Has node been removed from list and reinitialized?
 * @h: Node to be checked
 *
 * Not that not all removal functions will leave a node in unhashed
 * state.  For example, hlist_nulls_del_init_rcu() does leave the
 * node in unhashed state, but hlist_nulls_del() does not.
 */
static inline int cco_hlist_unhashed(struct cco_hnode const *h) {
  return !h->pprev;
}

#define cco_hlist_entry(ptr, type, member) cco_of(ptr, type, member)

#define cco_hlist_entry_safe(ptr, type, member)                                \
  ({                                                                           \
    __typeof__(ptr) ____ptr = (ptr);                                           \
    ____ptr ? cco_hlist_entry(____ptr, type, member) : NULL;                   \
  })

/**
 * __cco_hlist_for_each_entry	- iterate over list of given type
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the hnode within the struct.
 */
#define __cco_hlist_for_each_entry(pos, head, member)                          \
  for (pos = cco_hlist_entry_safe((head)->first, __typeof__(*(pos)), member);  \
       pos; pos = cco_hlist_entry_safe((pos)->member.next, __typeof__(*(pos)), \
                                       member))

/**
 * __cco_hlist_for_each_entry_safe - iterate over list of given type safe
 * against removal of list entry
 * @pos:	the type * to use as a loop cursor.
 * @n:		a &struct hnode to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the hnode within the struct.
 */
#define __cco_hlist_for_each_entry_safe(pos, n, head, member)                  \
  for (pos = cco_hlist_entry_safe((head)->first, __typeof__(*pos), member);    \
       pos && ({                                                               \
         n = pos->member.next;                                                 \
         1;                                                                    \
       });                                                                     \
       pos = cco_hlist_entry_safe(n, __typeof__(*pos), member))

#endif
