#ifndef CONTAINERS_NODE_H
#define CONTAINERS_NODE_H

#include <stddef.h>

struct cco_node {
  struct cco_node *next;
};

#define CCO_NODE_INIT()                                                        \
  { NULL }

static inline void cco_node_add_next(struct cco_node *where,
                                     struct cco_node *novel) {
  novel->next = where->next;
  where->next = novel;
}

static inline void cco_node_del(struct cco_node *prev, struct cco_node *node) {
  prev->next = node->next;
  node->next = NULL;
}

static inline void cco_node_init(struct cco_node *node) { node->next = NULL; }

#endif
