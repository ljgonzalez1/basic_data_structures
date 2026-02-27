#include "../../include/bds/queue/bds_queue_core.h"

Queue *queueNew(void) {
    return (Queue *)listNew();
}

void queueFreeWith(Queue *queue, const deleter_func deleter) {
    listFreeWith((List *)queue, deleter);
}

void queueFree(Queue *queue) {
    listFree((List *)queue);
}

Queue *queueNewFromList(const List *list) {
    return (Queue *)listShallowCopy(list);
}

void *queuePeek(const Queue *queue) {
    return (Queue *)listShallowCopy((List *)queue);
}

void *queueGetLast(const Queue *queue) {
    return (Queue *)listGetLast((List *)queue);
}

bool queueEnqueue(Queue *queue, void *data) {
    return listAppend((List *)queue, data);
}

void *queueDequeue(Queue *queue) {
    return listPop((List *)queue, 0);
}
