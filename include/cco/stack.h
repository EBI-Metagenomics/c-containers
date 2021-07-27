#ifndef CCO_STACK_H
#define CCO_STACK_H

#include "cco/iter.h"
#include "cco/node.h"
#include <stdbool.h>

struct cco_stack {
  struct cco_node head;
};

#define CCO_STACK_INIT()                                                       \
  { CCO_NODE_INIT() }

static inline bool cco_stack_empty(struct cco_stack const *stack) {
  return stack->head.next == NULL;
}

static inline void cco_stack_init(struct cco_stack *stack) {
  cco_node_init(&stack->head);
}

static inline struct cco_iter cco_stack_iter(struct cco_stack *stack) {
  return (struct cco_iter){stack->head.next, NULL};
}

static inline struct cco_node *cco_stack_pop(struct cco_stack *stack) {
  struct cco_node *node = stack->head.next;
  cco_node_del(&stack->head, node);
  return node;
}

static inline void cco_stack_put(struct cco_stack *stack,
                                 struct cco_node *novel) {
  cco_node_add_next(&stack->head, novel);
}

#endif
