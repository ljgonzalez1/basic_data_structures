#include "queue.h"
#include <stdlib.h>

Queue *queue_create(void) {
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    if (queue == NULL) {
        return NULL;
    }
    
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    return queue;
}

void queue_destroy(Queue *queue) {
    if (queue == NULL) {
        return;
    }
    
    queue_clear(queue);
    free(queue);
}

int queue_enqueue(Queue *queue, void *data) {
    if (queue == NULL) {
        return -1;
    }
    
    QueueNode *node = (QueueNode *)malloc(sizeof(QueueNode));
    if (node == NULL) {
        return -1;
    }
    
    node->data = data;
    node->next = NULL;
    
    if (queue->rear == NULL) {
        queue->front = node;
        queue->rear = node;
    } else {
        queue->rear->next = node;
        queue->rear = node;
    }
    
    queue->size++;
    return 0;
}

void *queue_dequeue(Queue *queue) {
    if (queue == NULL || queue->front == NULL) {
        return NULL;
    }
    
    QueueNode *node = queue->front;
    void *data = node->data;
    
    queue->front = node->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    
    free(node);
    queue->size--;
    
    return data;
}

void *queue_front(Queue *queue) {
    if (queue == NULL || queue->front == NULL) {
        return NULL;
    }
    return queue->front->data;
}

void *queue_rear(Queue *queue) {
    if (queue == NULL || queue->rear == NULL) {
        return NULL;
    }
    return queue->rear->data;
}

size_t queue_size(Queue *queue) {
    if (queue == NULL) {
        return 0;
    }
    return queue->size;
}

int queue_is_empty(Queue *queue) {
    return (queue == NULL || queue->front == NULL);
}

void queue_clear(Queue *queue) {
    if (queue == NULL) {
        return;
    }
    
    QueueNode *current = queue->front;
    while (current != NULL) {
        QueueNode *next = current->next;
        free(current);
        current = next;
    }
    
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
}
