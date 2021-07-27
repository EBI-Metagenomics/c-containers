#ifndef CONTAINERS_QUEUE_H
#define CONTAINERS_QUEUE_H

#include "containers/iter.h"
#include "containers/node.h"
#include <stdbool.h>

struct cco_queue {
  struct cco_node head;
  struct cco_node *tail;
};

#define CCO_QUEUE_INIT(name)                                                   \
  { (struct cco_node){&name.head}, &name.head }

static inline bool cco_queue_empty(struct cco_queue const *queue) {
  return &queue->head == queue->tail;
}

static inline void cco_queue_init(struct cco_queue *queue) {
  queue->tail = queue->head.next = &queue->head;
}

static inline struct cco_iter cco_queue_iter(struct cco_queue *queue) {
  return (struct cco_iter){queue->tail, &queue->head};
}

static inline struct cco_node *cco_queue_pop(struct cco_queue *queue) {
  struct cco_node *node = queue->tail;
  queue->tail = queue->tail->next;
  return node;
}

static inline void cco_queue_put(struct cco_queue *queue,
                                 struct cco_node *novel) {
  if (cco_queue_empty(queue))
    queue->tail = novel;

  struct cco_node *next = queue->head.next;
  next->next = novel;
  novel->next = &queue->head;
  queue->head.next = novel;
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
