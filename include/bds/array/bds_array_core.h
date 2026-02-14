#pragma once

#include "../bds_types.h"

#include <stddef.h>   // size_t
#include <stdbool.h>  // bool

typedef struct bds_array {
    void **data;
    size_t length;
} Array;

//// Lifecycle ////

Array *arrayNew(size_t length);

Array *arrayShallowCopy(const Array *array);

void arrayFreeWith(Array *array, deleter_func deleter);  // Frees payloads according to func

void arrayFree(Array *array);  // Just frees itself

//// Helper ////

static inline bool arrayExists(const Array *array) {
    return !!array;
}

//// Info ////

static inline size_t arrayLength(const Array *array) {
    return arrayExists(array) ? array->length : 0;
}

static inline bool arrayIsEmpty(const Array *array) {
    return arrayExists(array) ? array->length == 0 : true;
}

//// Access (read-only to `void **data[i]`) ////

static inline void *arrayGet(const Array *array, const size_t index) {
    return index < arrayLength(array) ? array->data[index] : NULL;
}

static inline void *arrayFirst(const Array *array) {
    if (arrayIsEmpty(array)) return NULL;
    return array->data[0];
}

static inline void *arrayLast(const Array *array) {
    if (arrayIsEmpty(array)) return NULL;
    return array->data[array->length - 1];
}

//// Change ////

// sets array[index] = data
static inline void arraySet(Array *array, const size_t index, void *data) {
    if (!arrayExists(array) || index >= arrayLength(array)) return;
    array->data[index] = data;
}
