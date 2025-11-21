#pragma once

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
size_t arrayLength(const Array *array);
bool arrayIsEmpty(const Array *array);

/// Access (read-only to `void **data[i]`)
void *arrayGet(const Array *array, size_t index);
void *arrayFirst(const Array *array);
void *arrayLast(const Array *array);

/// Change
// sets array[index] = data
bool arraySet(Array *array, size_t index, void *data);

