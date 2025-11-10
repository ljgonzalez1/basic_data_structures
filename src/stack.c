#include "stack.h"
#include <stdlib.h>

Stack *stack_create(void) {
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    if (stack == NULL) {
        return NULL;
    }
    
    stack->top = NULL;
    stack->size = 0;
    return stack;
}

void stack_destroy(Stack *stack) {
    if (stack == NULL) {
        return;
    }
    
    stack_clear(stack);
    free(stack);
}

int stack_push(Stack *stack, void *data) {
    if (stack == NULL) {
        return -1;
    }
    
    StackNode *node = (StackNode *)malloc(sizeof(StackNode));
    if (node == NULL) {
        return -1;
    }
    
    node->data = data;
    node->next = stack->top;
    stack->top = node;
    stack->size++;
    
    return 0;
}

void *stack_pop(Stack *stack) {
    if (stack == NULL || stack->top == NULL) {
        return NULL;
    }
    
    StackNode *node = stack->top;
    void *data = node->data;
    
    stack->top = node->next;
    free(node);
    stack->size--;
    
    return data;
}

void *stack_peek(Stack *stack) {
    if (stack == NULL || stack->top == NULL) {
        return NULL;
    }
    return stack->top->data;
}

size_t stack_size(Stack *stack) {
    if (stack == NULL) {
        return 0;
    }
    return stack->size;
}

int stack_is_empty(Stack *stack) {
    return (stack == NULL || stack->top == NULL);
}

void stack_clear(Stack *stack) {
    if (stack == NULL) {
        return;
    }
    
    StackNode *current = stack->top;
    while (current != NULL) {
        StackNode *next = current->next;
        free(current);
        current = next;
    }
    
    stack->top = NULL;
    stack->size = 0;
}
