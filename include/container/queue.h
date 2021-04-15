#ifndef QUEUE_H
#define QUEUE_H

#include "iter.h"
#include "snode.h"
#include <stdbool.h>

struct queue {
  struct snode head;
  struct snode *tail;
};

#define QUEUE_INIT()                                                           \
  { SNODE_INIT(), NULL }

static inline bool queue_empty(struct queue const *queue) {
  return queue->head.next == NULL;
}

static inline void queue_init(struct queue *queue) {
  snode_init(&queue->head);
  queue->tail = NULL;
}

static inline struct snode *queue_pop(struct queue *queue) {
  struct snode *node = queue->head.next;
  snode_del(&queue->head, node);
  return node;
}

static inline void queue_put(struct queue *queue, struct snode *novel) {
  if (queue_empty(queue))
    queue->tail = novel;

  snode_add_next(&queue->head, novel);
}

#endif
