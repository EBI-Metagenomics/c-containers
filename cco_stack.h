#ifndef CCO_STACK_H
#define CCO_STACK_H

#include "cco_iter.h"
#include "cco_node.h"
#include <stdbool.h>

struct cco_stack {
  struct cco_node head;
  struct cco_node *tail;
};

#define CCO_STACK_INIT(name)                                                   \
  { (struct cco_node){&name.head}, &name.head }

static inline void cco_stack_init(struct cco_stack *x) {
  x->tail = x->head.next = &x->head;
}

static inline struct cco_iter cco_stack_iter(struct cco_stack *x) {
  return (struct cco_iter){&x->head, x->head.next, &x->head};
}

static inline bool cco_stack_empty(struct cco_stack const *x) {
  struct cco_iter it = cco_stack_iter((struct cco_stack *)x);
  return it.prev == it.curr;
}

static inline void cco_stack_put(struct cco_stack *x, struct cco_node *novel) {
  cco_node_add_next(&x->head, novel);
}

static inline struct cco_node *cco_stack_pop(struct cco_stack *stack) {
  struct cco_node *node = stack->head.next;
  cco_node_del(&stack->head, node);
  return node;
}

#endif
