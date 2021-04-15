#ifndef ITER_H
#define ITER_H

#include "container.h"
#include "dnode.h"
#include "snode.h"

struct iter_dnode {
  struct dnode *curr;
  struct dnode const *end;
};

struct iter_snode {
  struct snode *curr;
  struct snode const *end;
};

static inline struct dnode *iter_dnode_next(struct iter_dnode *iter) {
  if (iter->curr == iter->end)
    return NULL;
  struct dnode *node = iter->curr;
  iter->curr = node->next;
  return node;
}

static inline struct snode *iter_snode_next(struct iter_snode *iter) {
  if (iter->curr == iter->end)
    return NULL;
  struct snode *node = iter->curr;
  iter->curr = node->next;
  return node;
}

#define iter_next(x)                                                           \
  _Generic((x), struct iter_snode *                                            \
           : iter_snode_next, struct iter_dnode *                              \
           : iter_dnode_next)(x)

#define ITER_FOREACH(entry, iter, member)                                      \
  for (entry = container_of_safe(iter_next(iter), __typeof__(*entry), member); \
       entry;                                                                  \
       entry = container_of_safe(iter_next(iter), __typeof__(*entry), member))

#endif
