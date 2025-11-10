#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int main(void) {
    printf("=== Queue Example ===\n\n");
    
    // Create a queue
    Queue *queue = queue_create();
    if (queue == NULL) {
        fprintf(stderr, "Failed to create queue\n");
        return 1;
    }
    
    // Enqueue elements
    printf("Enqueuing elements: 10, 20, 30, 40, 50\n");
    int nums[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        queue_enqueue(queue, &nums[i]);
    }
    
    printf("Queue size: %zu\n", queue_size(queue));
    
    // Peek at front and rear
    int *front = (int *)queue_front(queue);
    int *rear = (int *)queue_rear(queue);
    printf("Front element: %d\n", *front);
    printf("Rear element: %d\n\n", *rear);
    
    // Dequeue elements
    printf("Dequeuing elements:\n");
    while (!queue_is_empty(queue)) {
        int *val = (int *)queue_dequeue(queue);
        printf("  Dequeued: %d (size now: %zu)\n", *val, queue_size(queue));
    }
    
    printf("\nIs empty: %s\n", queue_is_empty(queue) ? "yes" : "no");
    
    // Clean up
    queue_destroy(queue);
    
    return 0;
}
