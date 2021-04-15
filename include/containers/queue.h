#ifndef CONTAINERS_QUEUE_H
#define CONTAINERS_QUEUE_H

#include "containers/iter.h"
#include "containers/node.h"
#include <stdbool.h>

struct queue {
  struct node head;
  struct node *tail;
};

#define QUEUE_INIT(name)                                                       \
  { (struct node){&name.head}, &name.head }

static inline bool queue_empty(struct queue const *queue) {
  return &queue->head == queue->tail;
}

static inline void queue_init(struct queue *queue) {
  queue->tail = queue->head.next = &queue->head;
}

static inline struct iter queue_iter(struct queue *queue) {
  return (struct iter){queue->tail, &queue->head};
}

static inline struct node *queue_pop(struct queue *queue) {
  struct node *node = queue->tail;
  queue->tail = queue->tail->next;
  return node;
}

static inline void queue_put(struct queue *queue, struct node *novel) {
  if (queue_empty(queue))
    queue->tail = novel;

  struct node *next = queue->head.next;
  next->next = novel;
  novel->next = &queue->head;
  queue->head.next = novel;
}

#endif
