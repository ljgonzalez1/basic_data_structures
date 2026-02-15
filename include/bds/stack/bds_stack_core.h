#pragma once

#include "../bds_config.h"
#include "../bds_types.h"
#include "../array/bds_array.h"

#include <stddef.h>   // size_t
#include <stdbool.h>  // bool
#include <stdlib.h>   // realloc

typedef struct bds_stack {
    void **data;
    size_t max_length;
    size_t top_idx;
} Stack;

//// Lifecycle ////

Stack *stackNew(size_t length);
Stack *stackShallowCopy(const Stack *stack);
void stackFreeWith(Stack *stack, deleter_func deleter);  // Frees payloads according to func
void stackFree(Stack *stack);  // Just frees itself
Stack *stackNewFromArray(const Array *array);  // Copies array data into stack; stack capacity >= array length

//// Helper ////

static inline bool stackExists(const Stack *stack) {
    return !!stack;
}

static inline bool stackNeedsExpansion(const Stack *stack) {
    return (stackExists(stack) && stack->top_idx >= stack->max_length);
}

/// Info ///

static inline size_t stackLength(const Stack *stack) {
    return stackExists(stack) ? stack->top_idx : 0;
}

static inline size_t stackCapacity(const Stack *stack) {
    return stackExists(stack) ? stack->max_length : 0;
}

static inline bool stackIsEmpty(const Stack *stack) {
    return stackLength(stack) == 0;
}

//// Access (read-only to `void **data[i]`) ////

static inline void *stackGet(const Stack *stack, const size_t index) {
    return index < stackLength(stack) ? stack->data[index] : NULL;
}

static inline void *stackPeek(const Stack *stack) {
    return stackIsEmpty(stack) ? NULL : stack->data[stack->top_idx - 1];
}

//// Change ////

static inline Stack *stackAutoExpand(Stack *stack) {
    if (!stackNeedsExpansion(stack)) return stack;
    size_t new_cap = (size_t)((double)stack->max_length * (STACK_AUTO_EXPAND_FACTOR + 1.0)) + 1;
    void **new_data = realloc(stack->data, sizeof(void *) * new_cap);
    return new_data ? (stack->data = new_data, stack->max_length = new_cap, stack) : stack;
}

static inline void *stackPop(Stack *stack) {
    return stackIsEmpty(stack) ? NULL : stack->data[--stack->top_idx];
}

static inline void *stackPush(Stack *stack, void *data) {
    stack = stackAutoExpand(stack);
    return stackExists(stack) ? (stack->data[stack->top_idx++] = data) : NULL;
}