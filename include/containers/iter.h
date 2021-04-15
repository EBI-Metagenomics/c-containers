#ifndef CONTAINERS_ITER_H
#define CONTAINERS_ITER_H

#include "containers/container_of.h"
#include "containers/node.h"

struct iter {
  struct node *curr;
  struct node const *end;
};

static inline struct node *iter_node_next(struct iter *iter) {
  if (iter->curr == iter->end)
    return NULL;
  struct node *node = iter->curr;
  iter->curr = node->next;
  return node;
}

#define iter_next(x) _Generic((x), struct iter * : iter_node_next)(x)

#define iter_for_each(pos, iter, member)                                       \
  for (pos = iter_next(iter); pos; pos = iter_next(iter))

#define iter_for_each_entry(entry, iter, member)                               \
  for (entry = container_of_safe(iter_next(iter), __typeof__(*entry), member); \
       entry;                                                                  \
       entry = container_of_safe(iter_next(iter), __typeof__(*entry), member))

#endif
