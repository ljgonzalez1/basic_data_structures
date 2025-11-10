#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>

typedef struct QueueNode {
    void *data;
    struct QueueNode *next;
} QueueNode;

typedef struct {
    QueueNode *front;
    QueueNode *rear;
    size_t size;
} Queue;

/* Create a new queue */
Queue *queue_create(void);

/* Destroy queue and free memory */
void queue_destroy(Queue *queue);

/* Add element to rear of queue */
int queue_enqueue(Queue *queue, void *data);

/* Remove and return front element */
void *queue_dequeue(Queue *queue);

/* Peek at front element without removing */
void *queue_front(Queue *queue);

/* Peek at rear element without removing */
void *queue_rear(Queue *queue);

/* Get size of queue */
size_t queue_size(Queue *queue);

/* Check if queue is empty */
int queue_is_empty(Queue *queue);

/* Clear all elements */
void queue_clear(Queue *queue);

#endif /* QUEUE_H */
