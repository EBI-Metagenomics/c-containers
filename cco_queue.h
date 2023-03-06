#ifndef CCO_QUEUE_H
#define CCO_QUEUE_H

#include "cco_iter.h"
#include "cco_node.h"
#include <stdbool.h>

struct cco_queue {
  struct cco_node head;
  struct cco_node *tail;
};

#define CCO_QUEUE_INIT(name)                                                   \
  { (struct cco_node){&name.head}, &name.head }

static inline void cco_queue_init(struct cco_queue *x) {
  x->tail = x->head.next = &x->head;
}

static inline struct cco_iter cco_queue_iter(struct cco_queue *x) {
  return (struct cco_iter){&x->head, x->tail, &x->head};
}

static inline bool cco_queue_empty(struct cco_queue const *x) {
  struct cco_iter it = cco_queue_iter((struct cco_queue *)x);
  return it.prev == it.curr;
}

static inline void cco_queue_put(struct cco_queue *x, struct cco_node *novel) {
  struct cco_node *a = x->tail;
  struct cco_node *b = x->tail->next;
  struct cco_node *c = novel;
  struct cco_node *tmp = b->next;

  c->next = b;
  b->next = a;
  a->next = c;

  x->tail = a->next->next->next;

  b->next = tmp;
  a->next = c->next;

  struct cco_node *next = x->head.next;
  next->next = novel;
  novel->next = &x->head;
  x->head.next = novel;
}

static inline struct cco_node *cco_queue_pop(struct cco_queue *queue) {
  struct cco_node *node = queue->tail;
  queue->tail = queue->tail->next;
  if (queue->tail == &queue->head)
    queue->head.next = &queue->head;
  return node;
}

static inline void cco_queue_put_first(struct cco_queue *queue,
                                       struct cco_node *novel) {
  struct cco_node *tail = queue->tail;
  novel->next = tail;
  queue->tail = novel;
  if (queue->head.next == &queue->head)
    queue->head.next = novel;
}

#endif
