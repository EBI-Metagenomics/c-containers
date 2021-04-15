#ifndef QUEUE_H
#define QUEUE_H

#include "iter.h"
#include "snode.h"
#include <stdbool.h>

typedef struct snode queue_node_t;
typedef struct iter_snode queue_iter_t;

struct queue {
  struct snode head;
  struct snode *tail;
};

#define QUEUE_NODE_INIT() SNODE_INIT()

#define QUEUE_INIT(name)                                                       \
  { (struct snode){&name.head}, &name.head }

static inline bool queue_empty(struct queue const *queue) {
  return &queue->head == queue->tail;
}

static inline void queue_init(struct queue *queue) {
  queue->tail = queue->head.next = &queue->head;
}

static inline queue_iter_t queue_iter(struct queue *queue) {
  return (queue_iter_t){queue->tail, &queue->head};
}

static inline struct snode *queue_pop(struct queue *queue) {
  struct snode *node = queue->tail;
  queue->tail = queue->tail->next;
  return node;
}

static inline void queue_put(struct queue *queue, struct snode *novel) {
  if (queue_empty(queue))
    queue->tail = novel;

  struct snode *next = queue->head.next;
  next->next = novel;
  novel->next = &queue->head;
  queue->head.next = novel;
}

#endif
