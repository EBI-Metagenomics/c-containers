#ifndef SNODE_H
#define SNODE_H

#include <stddef.h>

struct snode {
  struct snode *next;
};

#define SNODE_INIT()                                                           \
  { NULL }

static inline void snode_add_next(struct snode *where, struct snode *novel) {
  novel->next = where->next;
  where->next = novel;
}

static inline void snode_del(struct snode *prev, struct snode *node) {
  prev->next = node->next;
  node->next = NULL;
}

static inline void snode_init(struct snode *node) { node->next = NULL; }

#endif
