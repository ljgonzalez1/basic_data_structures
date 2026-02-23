#include "../../include/bds/stack/bds_stack_core.h"

#include <stdlib.h>


/// Lifecycle
Stack *stackNew(void) {
    Stack *stk = (Stack *)malloc(sizeof(Stack));
    if (!stk) return NULL;

    stk->max_length = ARRAY_MINIMUM_CAPACITY;
    stk->top_idx = 0;
    *stk->data = (void **)calloc(stk->max_length, sizeof(void*));

	if (!stk->data) {
        free(stk);
        return NULL;
    }

	return stk;
}


void stackFreeWith(Stack *stack, deleter_func deleter) {
	for (size_t i = 0; i < stackLength(stack); i++) {
        void *datapoint = stackGet(stack, i);
        deleter(datapoint);
    }

	stackFree(stack);
}


void stackFree(Stack *stack) {
	if (!stackExists(stack)) return;

	free(stack->data);
	free(stack);
}


Stack *stackNewFromArray(const Array *array) {  // Copies array data into stack; stack capacity >= array length
	Stack *new_stack = stackNew();

	for (size_t i = 0; i < arrayLength(array); i++) {
        void *datapoint = arrayGet(array, i);
        stackPush(new_stack, datapoint);
    }

	return new_stack;
}

