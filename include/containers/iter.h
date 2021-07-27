#ifndef CONTAINERS_ITER_H
#define CONTAINERS_ITER_H

#include "containers/container_of.h"
#include "containers/node.h"

struct cco_iter {
  struct cco_node *curr;
  struct cco_node const *end;
};

static inline struct cco_node *cco_iter_next(struct cco_iter *iter) {
  if (!iter || iter->curr == iter->end)
    return NULL;
  struct cco_node *node = iter->curr;
  iter->curr = node->next;
  return node;
}

#define cco_iter_entry(pos, type, member)                                      \
  cco_container_of_safe(pos, type, member)

#define cco_iter_next_entry(iter, entry, member)                               \
  cco_container_of_safe(cco_iter_next(iter), __typeof__(*entry), member)

#define cco_iter_for_each(pos, iter, member)                                   \
  for (pos = cco_iter_next(iter); pos; pos = cco_iter_next(iter))

#define cco_iter_for_each_safe(pos, tmp, iter, member)                         \
  for (pos = cco_iter_next(iter), tmp = cco_iter_next(iter); pos;              \
       pos = tmp, tmp = iter_next(iter))

#define cco_iter_for_each_entry(entry, iter, member)                           \
  for (entry = cco_iter_next_entry(iter, entry, member); entry;                \
       entry = cco_iter_next_entry(iter, entry, member))

#define cco_iter_for_each_entry_safe(entry, tmp, iter, member)                 \
  for (entry = cco_iter_next_entry(iter, entry, member),                       \
      tmp = cco_iter_next_entry(iter, entry, member);                          \
       entry; entry = tmp, tmp = cco_iter_next_entry(iter, entry, member))

#endif
