#ifndef CONTAINERS_HNODE_H
#define CONTAINERS_HNODE_H

#include <stddef.h>

struct cco_hnode {
  struct cco_hnode *next, **pprev;
};

#define CCO_HNODE_INIT()                                                       \
  { NULL, NULL }

/**
 * cco_hnode_unhashed - Has node been removed from list and reinitialized?
 * @h: Node to be checked
 *
 * Not that not all removal functions will leave a node in unhashed
 * state.
 */
static inline int cco_hnode_unhashed(const struct cco_hnode *h) {
  return !h->pprev;
}

static inline void __cco_hnode_del(struct cco_hnode *n) {
  struct cco_hnode *next = n->next;
  struct cco_hnode **pprev = n->pprev;

  *pprev = next;
  if (next)
    next->pprev = pprev;
}

static inline void cco_hnode_init(struct cco_hnode *h) {
  h->next = NULL;
  h->pprev = NULL;
}

/**
 * cco_hnode_del_init - Delete the specified hnode from its list and
 * initialize
 * @n: Node to delete.
 *
 * Note that this function leaves the node in unhashed state.
 */
static inline void cco_hnode_del_init(struct cco_hnode *n) {
  if (!cco_hnode_unhashed(n)) {
    __cco_hnode_del(n);
    cco_hnode_init(n);
  }
}

#endif
