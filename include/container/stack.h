#ifndef STACK_H
#define STACK_H

#include "iter.h"
#include "snode.h"
#include <stdbool.h>

typedef struct snode stack_node_t;
typedef struct iter_snode stack_iter_t;

struct stack {
  struct snode head;
};

#define STACK_NODE_INIT() SNODE_INIT()

#define STACK_INIT()                                                           \
  { STACK_NODE_INIT() }

static inline bool stack_empty(struct stack const *stack) {
  return stack->head.next == NULL;
}

static inline void stack_init(struct stack *stack) { snode_init(&stack->head); }

static inline stack_iter_t stack_iter(struct stack *stack) {
  return (stack_iter_t){stack->head.next, NULL};
}

static inline struct snode *stack_pop(struct stack *stack) {
  struct snode *node = stack->head.next;
  snode_del(&stack->head, node);
  return node;
}

static inline void stack_put(struct stack *stack, struct snode *novel) {
  snode_add_next(&stack->head, novel);
}

#endif
