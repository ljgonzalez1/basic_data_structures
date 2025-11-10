#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stack.h"

void test_stack_create_destroy(void) {
    Stack *stack = stack_create();
    assert(stack != NULL);
    assert(stack_size(stack) == 0);
    assert(stack_is_empty(stack) == 1);
    stack_destroy(stack);
    printf("✓ test_stack_create_destroy passed\n");
}

void test_stack_push_pop(void) {
    Stack *stack = stack_create();
    int nums[] = {1, 2, 3, 4, 5};
    
    for (int i = 0; i < 5; i++) {
        assert(stack_push(stack, &nums[i]) == 0);
    }
    
    assert(stack_size(stack) == 5);
    assert(stack_is_empty(stack) == 0);
    
    // Stack is LIFO - last in, first out
    int *val = (int *)stack_pop(stack);
    assert(*val == 5);
    
    val = (int *)stack_pop(stack);
    assert(*val == 4);
    
    assert(stack_size(stack) == 3);
    
    stack_destroy(stack);
    printf("✓ test_stack_push_pop passed\n");
}

void test_stack_peek(void) {
    Stack *stack = stack_create();
    int nums[] = {10, 20, 30};
    
    for (int i = 0; i < 3; i++) {
        stack_push(stack, &nums[i]);
    }
    
    int *val = (int *)stack_peek(stack);
    assert(*val == 30);
    assert(stack_size(stack) == 3); // Size shouldn't change
    
    stack_destroy(stack);
    printf("✓ test_stack_peek passed\n");
}

void test_stack_clear(void) {
    Stack *stack = stack_create();
    int nums[] = {1, 2, 3};
    
    for (int i = 0; i < 3; i++) {
        stack_push(stack, &nums[i]);
    }
    
    assert(stack_size(stack) == 3);
    stack_clear(stack);
    assert(stack_size(stack) == 0);
    assert(stack_is_empty(stack) == 1);
    
    stack_destroy(stack);
    printf("✓ test_stack_clear passed\n");
}

int main(void) {
    printf("Running stack tests...\n");
    test_stack_create_destroy();
    test_stack_push_pop();
    test_stack_peek();
    test_stack_clear();
    printf("\nAll stack tests passed!\n");
    return 0;
}
