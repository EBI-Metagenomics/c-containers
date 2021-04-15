#ifndef CONTAINERS_NODE_H
#define CONTAINERS_NODE_H

#include <stddef.h>

struct node {
  struct node *next;
};

#define NODE_INIT()                                                            \
  { NULL }

static inline void node_add_next(struct node *where, struct node *novel) {
  novel->next = where->next;
  where->next = novel;
}

static inline void node_del(struct node *prev, struct node *node) {
  prev->next = node->next;
  node->next = NULL;
}

static inline void node_init(struct node *node) { node->next = NULL; }

#endif
