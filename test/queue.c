#include "cco/cco.h"

static int errors = 0;

struct foo {
  int val;
  struct cco_node node;
};

int main(void) {
  struct cco_queue queue = CCO_QUEUE_INIT(queue);

  struct foo foo1 = {1, CCO_NODE_INIT()};
  struct foo foo2 = {2, CCO_NODE_INIT()};
  struct foo foo3 = {3, CCO_NODE_INIT()};
  struct foo foo4 = {4, CCO_NODE_INIT()};
  struct foo foo5 = {5, CCO_NODE_INIT()};

  errors += !cco_queue_empty(&queue);
  cco_queue_put(&queue, &foo1.node);
  cco_queue_put(&queue, &foo2.node);
  cco_queue_put(&queue, &foo3.node);

  struct foo *entry = NULL;
  struct cco_iter iter = cco_queue_iter(&queue);
  int const vals[] = {1, 2, 3};
  int const *val = vals;
  cco_iter_for_each_entry(entry, &iter, node) {
    errors += entry->val != *val;
    val++;
  }

  errors += cco_of(cco_queue_pop(&queue), struct foo, node)->val != 1;
  errors += cco_queue_empty(&queue);
  errors += cco_of(cco_queue_pop(&queue), struct foo, node)->val != 2;
  errors += cco_queue_empty(&queue);

  cco_queue_put(&queue, &foo2.node);
  cco_queue_put(&queue, &foo1.node);

  errors += cco_of(cco_queue_pop(&queue), struct foo, node)->val != 3;
  errors += cco_queue_empty(&queue);
  errors += cco_of(cco_queue_pop(&queue), struct foo, node)->val != 2;
  errors += cco_queue_empty(&queue);

  iter = cco_queue_iter(&queue);
  errors += cco_of(cco_iter_next(&iter), struct foo, node)->val != 1;
  errors += cco_queue_empty(&queue);
  errors += cco_of_safe(cco_iter_next(&iter), struct foo, node) != NULL;

  cco_queue_pop(&queue);
  errors += !cco_queue_empty(&queue);

  cco_queue_put(&queue, &foo3.node);
  errors += cco_of(cco_queue_pop(&queue), struct foo, node)->val != 3;
  errors += !cco_queue_empty(&queue);

  cco_queue_put(&queue, &foo1.node);
  cco_queue_put(&queue, &foo2.node);
  cco_queue_put(&queue, &foo3.node);
  cco_queue_put_first(&queue, &foo4.node);
  cco_queue_put_first(&queue, &foo5.node);

  int const vals2[] = {5, 4, 1, 2, 3};
  val = vals2;
  cco_iter_for_each_entry(entry, &iter, node) {
    errors += entry->val != *val;
    val++;
  }

  return errors;
}
