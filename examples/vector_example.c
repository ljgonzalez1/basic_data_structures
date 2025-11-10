#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

int main(void) {
    printf("=== Vector Example ===\n\n");
    
    // Create a vector
    Vector *vec = vector_create(4);
    if (vec == NULL) {
        fprintf(stderr, "Failed to create vector\n");
        return 1;
    }
    
    // Add some integers
    printf("Adding elements: 10, 20, 30, 40, 50\n");
    int nums[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        vector_push_back(vec, &nums[i]);
    }
    
    // Display vector contents
    printf("Vector size: %zu\n", vector_size(vec));
    printf("Vector contents: ");
    for (size_t i = 0; i < vector_size(vec); i++) {
        int *val = (int *)vector_get(vec, i);
        printf("%d ", *val);
    }
    printf("\n\n");
    
    // Pop last element
    int *popped = (int *)vector_pop_back(vec);
    printf("Popped element: %d\n", *popped);
    printf("Vector size after pop: %zu\n", vector_size(vec));
    
    printf("Vector contents: ");
    for (size_t i = 0; i < vector_size(vec); i++) {
        int *val = (int *)vector_get(vec, i);
        printf("%d ", *val);
    }
    printf("\n\n");
    
    // Check if empty
    printf("Is empty: %s\n", vector_is_empty(vec) ? "yes" : "no");
    
    // Clear vector
    vector_clear(vec);
    printf("After clear, is empty: %s\n", vector_is_empty(vec) ? "yes" : "no");
    
    // Clean up
    vector_destroy(vec);
    
    return 0;
}
