#include "containers/containers.h"

static int errors = 0;

struct foo {
  int val;
  struct node node;
};

int main(void) {
  struct queue queue = QUEUE_INIT(queue);

  struct foo foo1 = {1, NODE_INIT()};
  struct foo foo2 = {2, NODE_INIT()};
  struct foo foo3 = {3, NODE_INIT()};

  errors += !queue_empty(&queue);
  queue_put(&queue, &foo1.node);
  queue_put(&queue, &foo2.node);
  queue_put(&queue, &foo3.node);

  struct foo *entry = NULL;
  struct iter iter = queue_iter(&queue);
  int const vals[] = {1, 2, 3};
  int const *val = vals;
  ITER_FOREACH(entry, &iter, node) {
    errors += entry->val != *val;
    val++;
  }

  errors += container_of(queue_pop(&queue), struct foo, node)->val != 1;
  errors += queue_empty(&queue);
  errors += container_of(queue_pop(&queue), struct foo, node)->val != 2;
  errors += queue_empty(&queue);

  queue_put(&queue, &foo2.node);
  queue_put(&queue, &foo1.node);

  errors += container_of(queue_pop(&queue), struct foo, node)->val != 3;
  errors += queue_empty(&queue);
  errors += container_of(queue_pop(&queue), struct foo, node)->val != 2;
  errors += queue_empty(&queue);

  iter = queue_iter(&queue);
  errors += container_of(iter_next(&iter), struct foo, node)->val != 1;
  errors += queue_empty(&queue);
  errors += container_of_safe(iter_next(&iter), struct foo, node) != NULL;

  queue_pop(&queue);
  errors += !queue_empty(&queue);

  queue_put(&queue, &foo3.node);
  errors += container_of(queue_pop(&queue), struct foo, node)->val != 3;
  errors += !queue_empty(&queue);

  return errors;
}
