#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

int main(void) {
    printf("=== Linked List Example ===\n\n");
    
    // Create a linked list
    LinkedList *list = list_create();
    if (list == NULL) {
        fprintf(stderr, "Failed to create linked list\n");
        return 1;
    }
    
    // Add elements to the back
    printf("Adding elements to back: 1, 2, 3\n");
    int nums[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 3; i++) {
        list_push_back(list, &nums[i]);
    }
    
    // Add elements to the front
    printf("Adding elements to front: 4, 5\n");
    list_push_front(list, &nums[3]);
    list_push_front(list, &nums[4]);
    
    printf("List size: %zu\n", list_size(list));
    
    // Display front and back
    int *front = (int *)list_front(list);
    int *back = (int *)list_back(list);
    printf("Front element: %d\n", *front);
    printf("Back element: %d\n", *back);
    printf("\n");
    
    // Pop from front
    printf("Popping elements from front:\n");
    while (!list_is_empty(list)) {
        int *val = (int *)list_pop_front(list);
        printf("  Popped: %d (size now: %zu)\n", *val, list_size(list));
    }
    
    printf("\nIs empty: %s\n", list_is_empty(list) ? "yes" : "no");
    
    // Clean up
    list_destroy(list);
    
    return 0;
}
