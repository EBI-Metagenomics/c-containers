#ifndef CONTAINERS_HNODE_H
#define CONTAINERS_HNODE_H

#include <stddef.h>

struct hnode {
  struct hnode *next, **pprev;
};

#define HNODE_INIT()                                                           \
  { NULL, NULL }

/**
 * hnode_unhashed - Has node been removed from list and reinitialized?
 * @h: Node to be checked
 *
 * Not that not all removal functions will leave a node in unhashed
 * state.
 */
static inline int hnode_unhashed(const struct hnode *h) { return !h->pprev; }

static inline void __hnode_del(struct hnode *n) {
  struct hnode *next = n->next;
  struct hnode **pprev = n->pprev;

  *pprev = next;
  if (next)
    next->pprev = pprev;
}

static inline void hnode_init(struct hnode *h) {
  h->next = NULL;
  h->pprev = NULL;
}

/**
 * hnode_del_init - Delete the specified hnode from its list and
 * initialize
 * @n: Node to delete.
 *
 * Note that this function leaves the node in unhashed state.
 */
static inline void hnode_del_init(struct hnode *n) {
  if (!hnode_unhashed(n)) {
    __hnode_del(n);
    hnode_init(n);
  }
}

#endif
