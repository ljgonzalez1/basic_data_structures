#pragma once

#include "../bds_types.h"
#include "../list/bds_list.h"
#include "../bds_utils.h"

#include <stddef.h>   // size_t
#include <stdbool.h>  // bool

typedef List Queue;

//// Lifecycle ////

Queue *queueNew(void);
void queueFreeWith(Queue *queue, deleter_func deleter);  // Frees payloads according to func
void queueFree(Queue *queue);  // Just frees itself
Queue *queueNewFromList(const List *list);
Queue *queueShallowCopy(const Queue *queue);

//// Helper ////

static inline bool queueExists(const Queue *stack) {
    return this_struct_exists((void *)stack);
}

/// Info ///

static inline bool queueLength(const Queue *queue) {
    return listLength((List *)queue);
}

static inline size_t queueIsEmpty(const Queue *queue) {
    return listIsEmpty((List *)queue);
}

//// Access (read-only to `void **data[i]`) ////


void *queuePeek(const Queue *queue);
void *queueGetLast(const Queue *queue);

//// Change ////

bool queueEnqueue(Queue *queue, void *data);
void *queueDequeue(Queue *queue);

