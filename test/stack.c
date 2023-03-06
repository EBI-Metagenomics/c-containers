#include "cco.h"

static int errors = 0;

struct foo {
  int val;
  struct cco_node node;
};

int main(void) {
  struct cco_stack stack = CCO_STACK_INIT(stack);

  struct foo foo1 = {1, CCO_NODE_INIT()};
  struct foo foo2 = {2, CCO_NODE_INIT()};
  struct foo foo3 = {3, CCO_NODE_INIT()};

  errors += !cco_stack_empty(&stack);
  cco_stack_put(&stack, &foo1.node);
  cco_stack_put(&stack, &foo2.node);
  cco_stack_put(&stack, &foo3.node);

  struct foo *entry = NULL;
  struct cco_iter iter = cco_stack_iter(&stack);
  int const vals[] = {3, 2, 1};
  int const *val = vals;
  cco_iter_for_each_entry(entry, &iter, node) {
    errors += entry->val != *val;
    val++;
  }

  errors += cco_of(cco_stack_pop(&stack), struct foo, node)->val != 3;
  errors += cco_stack_empty(&stack);
  errors += cco_of(cco_stack_pop(&stack), struct foo, node)->val != 2;
  errors += cco_stack_empty(&stack);

  cco_stack_put(&stack, &foo3.node);
  cco_stack_put(&stack, &foo2.node);

  errors += cco_of(cco_stack_pop(&stack), struct foo, node)->val != 2;
  errors += cco_stack_empty(&stack);
  errors += cco_of(cco_stack_pop(&stack), struct foo, node)->val != 3;
  errors += cco_stack_empty(&stack);

  iter = cco_stack_iter(&stack);
  errors += cco_of(cco_iter_next(&iter), struct foo, node)->val != 1;
  errors += cco_stack_empty(&stack);
  errors += cco_of_safe(cco_iter_next(&iter), struct foo, node) != NULL;

  cco_stack_pop(&stack);
  errors += !cco_stack_empty(&stack);

  cco_stack_put(&stack, &foo3.node);
  errors += cco_of(cco_stack_pop(&stack), struct foo, node)->val != 3;
  errors += !cco_stack_empty(&stack);

  return errors;
}
