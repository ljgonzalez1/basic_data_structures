#pragma once

#include <stddef.h>   // size_t
#include <stdbool.h>  // bool

typedef struct bds_array {
    void **payload;
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
size_t arrayLength(Array *array);
bool arrayIsEmpty(Array *array);

/// Access (read-only to `void **payload[i]`)
void *arrayGet(Array *array, size_t index);
const void *arrayCGet(const Array *array, size_t index);
void *arrayFirst(Array *array);
const void *arrayCFirst(const Array *array);
void *arrayLast(Array *array);
const void *arrayCLast(const Array *array);

// Change
bool arraySet(Array *array, size_t index, void *data);

/// Search and count
size_t arrayIndexOf(const Array *array, filter_func key);
size_t arrayCount(const Array *array, filter_func key);
size_t arrayMinIdx(const Array *array, key_val_func key);
size_t arrayMaxIdx(const Array *array, key_val_func key);


/// Sorting (return NEW sorted array)
// AVG: O(n²)
Array *arrayBubbleSorted (const Array *array, key_val_func key);
Array *arrayInsertionSorted(const Array *array, key_val_func key);
Array *arraySelectionSorted(const Array *array, key_val_func key);
Array *arrayCocktailShakerSorted(const Array *array, key_val_func key);
Array *arrayGnomeSorted(const Array *array, key_val_func key);
// AVG: O(n log n)
Array *arrayMergeSorted  (const Array *array, key_val_func key);
Array *arrayHeapSorted(const Array *array, key_val_func key);
Array *arrayTimSorted(const Array *array, key_val_func key);
Array *arrayIntroSorted(const Array *array, key_val_func key);
// AVG: O(n log² n)
Array *arrayShellSorted(const Array *array, key_val_func key);
// AVG: O(n log n) ; WORST O(n²)
Array *arrayQuickSorted  (const Array *array, key_val_func key);

