#pragma once

#include "../bds_config.h"

#include "../bds_types.h"
#include "../array/bds_array.h"

#include <stddef.h>   // size_t
#include <stdbool.h>  // bool

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


static inline Stack *stackAutoExpand(Stack *stack) {
    // "Greater than" SHOULD BE IMPOSSIBLE. Just overflow prevention + who knows what.
    if (!stackExists(stack) || stackLength(stack) < stackCapacity(stack)) return stack;

    size_t new_cap = (size_t)((double)stackCapacity(stack) * (STACK_AUTO_EXPAND_FACTOR + 1.0)) + 1;
    Stack *expandedStack = realloc(stack, sizeof(void *) * new_cap);

    return expandedStack ? (expandedStack->max_length = new_cap, expandedStack) : stack;
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
    return stackEmpty(stack) ? NULL : stack->data[stack->top_idx - 1];
}


//// Change ////

static inline void *stackPop(Stack *stack) {
    return stackIsEmpty(stack) ? NULL : stack->data[--stack->top_idx];
}

static inline bool stackPush(Stack *stack, void *data) {
    if (!stackExists(stack)) return false;

    if (stackLength(stack) == stackCapacity(stack)) {
        Stack *expanded_stack = stackAutoExpand(stack);
        if (!expanded_stack) return false;
        stack = expanded_stack;
    }

    stack->data[stack->top_idx++] = data;
    return true;
}



//// Change ////

// sets array[index] = data
static inline void arraySet(Array *array, const size_t index, void *data) {
    if (!arrayExists(array) || index >= arrayLength(array)) return;
    array->data[index] = data;
}
