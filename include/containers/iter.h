#ifndef CONTAINERS_ITER_H
#define CONTAINERS_ITER_H

#include "containers/container_of.h"
#include "containers/node.h"

struct iter {
  struct node *curr;
  struct node const *end;
};

static inline struct node *iter_next(struct iter *iter) {
  if (!iter || iter->curr == iter->end)
    return NULL;
  struct node *node = iter->curr;
  iter->curr = node->next;
  return node;
}

#define iter_entry(pos, type, member) container_of_safe(pos, type, member)

#define iter_next_entry(iter, entry, member)                                   \
  container_of_safe(iter_next(iter), __typeof__(*entry), member)

#define iter_for_each(pos, iter, member)                                       \
  for (pos = iter_next(iter); pos; pos = iter_next(iter))

#define iter_for_each_safe(pos, tmp, iter, member)                             \
  for (pos = iter_next(iter), tmp = iter_next(iter); pos;                      \
       pos = tmp, tmp = iter_next(iter))

#define iter_for_each_entry(entry, iter, member)                               \
  for (entry = iter_next_entry(iter, entry, member); entry;                    \
       entry = iter_next_entry(iter, entry, member))

#define iter_for_each_entry_safe(entry, tmp, iter, member)                     \
  for (entry = iter_next_entry(iter, entry, member),                           \
      tmp = iter_next_entry(iter, entry, member);                              \
       entry; entry = tmp, tmp = iter_next_entry(iter, entry, member))

#endif
