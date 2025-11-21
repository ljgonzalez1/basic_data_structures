#pragma once

#include "../bds_config.h"

#include <stddef.h>   // size_t
#include <stdbool.h>  // bool

typedef struct bds_array {
    void **data;
    size_t length;
} Array;

// Key functions
typedef int (*key_val_func)(const void *elem);
typedef void (*deleter_func)(void *elem);
typedef bool (*filter_func)(const void *elem);  // for counters

/// Lifecycle
Array *arrayNew(size_t length);
Array *arrayShallowCopy(const Array *array);
void arrayFreeWith(Array *array, deleter_func deleter);  // Frees payloads according to func
void arrayFree(Array *array);  // Just frees itself

/// Info
static inline size_t arrayLength(const Array *array) {
    return array->length;
}

static inline bool arrayIsEmpty(const Array *array) {
    return array->length == 0;
}

/// Access (read-only to `void **data[i]`)
static inline void *arrayGet(const Array *array, const size_t index) {
    return array->data[index];
}

static inline void *arrayFirst(const Array *array) {
    if (arrayIsEmpty(array)) return NULL;
    return array->data[0];
}

static inline void *arrayLast(const Array *array) {
    if (arrayIsEmpty(array)) return NULL;
    return array->data[array->length - 1];
}

/// Change
// sets array[index] = data
static inline void arraySet(Array *array, const size_t index, void *data) {
    array->data[index] = data;
}

