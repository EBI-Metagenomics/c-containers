#ifndef DNODE_H
#define DNODE_H

#include <stddef.h>

struct dnode {
  struct dnode *next;
  struct dnode *prev;
};

#define NODE_INIT(name)                                                        \
  { &(name), &(name) }

static inline void dnode_add(struct dnode *prev, struct dnode *next,
                             struct dnode *novel) {
  next->prev = novel;
  novel->next = next;
  novel->prev = prev;
  prev->next = novel;
}

static inline void dnode_add_next(struct dnode *where, struct dnode *novel) {
  dnode_add(where, where->next, novel);
}

static inline void dnode_add_prev(struct dnode *where, struct dnode *novel) {
  dnode_add(where->prev, where, novel);
}

static inline void dnode_del(struct dnode *node) {
  node->prev->next = node->next;
  node->next->prev = node->prev;
}

static inline void dnode_init(struct dnode *node) {
  node->next = node;
  node->prev = node;
}

#endif
