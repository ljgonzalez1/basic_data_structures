#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

int main(void) {
    printf("=== Stack Example ===\n\n");
    
    // Create a stack
    Stack *stack = stack_create();
    if (stack == NULL) {
        fprintf(stderr, "Failed to create stack\n");
        return 1;
    }
    
    // Push elements
    printf("Pushing elements: 100, 200, 300, 400\n");
    int nums[] = {100, 200, 300, 400};
    for (int i = 0; i < 4; i++) {
        stack_push(stack, &nums[i]);
    }
    
    printf("Stack size: %zu\n", stack_size(stack));
    
    // Peek at top
    int *top = (int *)stack_peek(stack);
    printf("Top element (peek): %d\n\n", *top);
    
    // Pop elements
    printf("Popping elements:\n");
    while (!stack_is_empty(stack)) {
        int *val = (int *)stack_pop(stack);
        printf("  Popped: %d (size now: %zu)\n", *val, stack_size(stack));
    }
    
    printf("\nIs empty: %s\n", stack_is_empty(stack) ? "yes" : "no");
    
    // Clean up
    stack_destroy(stack);
    
    return 0;
}
