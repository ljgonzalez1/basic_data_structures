#ifndef STACK_H
#define STACK_H

#include <stddef.h>

typedef struct StackNode {
    void *data;
    struct StackNode *next;
} StackNode;

typedef struct {
    StackNode *top;
    size_t size;
} Stack;

/* Create a new stack */
Stack *stack_create(void);

/* Destroy stack and free memory */
void stack_destroy(Stack *stack);

/* Push element onto stack */
int stack_push(Stack *stack, void *data);

/* Pop element from stack */
void *stack_pop(Stack *stack);

/* Peek at top element without removing */
void *stack_peek(Stack *stack);

/* Get size of stack */
size_t stack_size(Stack *stack);

/* Check if stack is empty */
int stack_is_empty(Stack *stack);

/* Clear all elements */
void stack_clear(Stack *stack);

#endif /* STACK_H */
