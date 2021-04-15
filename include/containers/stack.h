#ifndef CONTAINERS_STACK_H
#define CONTAINERS_STACK_H

#include "containers/iter.h"
#include "containers/node.h"
#include <stdbool.h>

struct stack {
  struct node head;
};

#define STACK_INIT()                                                           \
  { NODE_INIT() }

static inline bool stack_empty(struct stack const *stack) {
  return stack->head.next == NULL;
}

static inline void stack_init(struct stack *stack) { node_init(&stack->head); }

static inline struct iter stack_iter(struct stack *stack) {
  return (struct iter){stack->head.next, NULL};
}

static inline struct node *stack_pop(struct stack *stack) {
  struct node *node = stack->head.next;
  node_del(&stack->head, node);
  return node;
}

static inline void stack_put(struct stack *stack, struct node *novel) {
  node_add_next(&stack->head, novel);
}

#endif
