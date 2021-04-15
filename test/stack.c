#include "container/stack.h"

static int errors = 0;

struct foo {
  int val;
  struct snode node;
};

int main(void) {
  struct stack stack = STACK_INIT(stack);

  struct foo foo1 = {1, SNODE_INIT()};
  struct foo foo2 = {2, SNODE_INIT()};
  struct foo foo3 = {3, SNODE_INIT()};

  errors += !stack_empty(&stack);
  stack_put(&stack, &foo1.node);
  stack_put(&stack, &foo2.node);
  stack_put(&stack, &foo3.node);

  struct foo *entry = NULL;
  stack_iter_t iter = stack_iter(&stack);
  int const vals[] = {3, 2, 1};
  int const *val = vals;
  ITER_FOREACH(entry, &iter, node) {
    errors += entry->val != *val;
    val++;
  }

  errors += container_of(stack_pop(&stack), struct foo, node)->val != 3;
  errors += stack_empty(&stack);
  errors += container_of(stack_pop(&stack), struct foo, node)->val != 2;
  errors += stack_empty(&stack);

  stack_put(&stack, &foo3.node);
  stack_put(&stack, &foo2.node);

  errors += container_of(stack_pop(&stack), struct foo, node)->val != 2;
  errors += stack_empty(&stack);
  errors += container_of(stack_pop(&stack), struct foo, node)->val != 3;
  errors += stack_empty(&stack);

  iter = stack_iter(&stack);
  errors += container_of(iter_next(&iter), struct foo, node)->val != 1;
  errors += stack_empty(&stack);
  errors += container_of_safe(iter_next(&iter), struct foo, node) != NULL;

  stack_pop(&stack);
  errors += !stack_empty(&stack);

  stack_put(&stack, &foo3.node);
  errors += container_of(stack_pop(&stack), struct foo, node)->val != 3;
  errors += !stack_empty(&stack);

  return errors;
}
