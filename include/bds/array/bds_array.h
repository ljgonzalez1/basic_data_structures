#pragma once

#include <stddef.h>   // size_t
#include <stdbool.h>  // bool

typedef struct bds_array {
    void **payload;
    size_t length;
} Array;

typedef const Array *ConstArray;

// Key functions
typedef int (*key_val_func)(const void *elem);
typedef void (*deleter_func)(void *elem);
typedef bool (*filter_func)(const void *elem);  // for counters

/// Lifecycle
Array *arrayNew(size_t length);
Array *arrayShallowCopy(ConstArray *array);
void arrayFreeWith(Array *array, deleter_func deleter);  // Frees payloads according to func
void arrayFree(Array *array);  // Just frees itself

/// Info
size_t arrayLength(Array *array);
bool arrayIsEmpty(Array *array);

/// Access (read-only to `void **payload[i]`)
ConstArray *arrayConstIfy(Array *array);
void *arrayGet(Array *array, size_t index);
const void *arrayCGet(ConstArray *array, size_t index);
void *arrayFirst(Array *array);
const void *arrayCFirst(ConstArray *array);
void *arrayLast(Array *array);
const void *arrayCLast(ConstArray *array);

// Change
bool arraySet(Array *array, size_t index, void *data);

/// Search and count
size_t arrayIndexOf(ConstArray *array, filter_func key);
size_t arrayCount(ConstArray *array, filter_func key);
size_t arrayMinIdx(ConstArray *array, key_val_func key);
size_t arrayMaxIdx(ConstArray *array, key_val_func key);


/// Sorting (return NEW sorted array)
// AVG: O(n²)
Array *arrayBubbleSorted (ConstArray *array, key_val_func key);
Array *arrayInsertionSorted(ConstArray *array, key_val_func key);
Array *arraySelectionSorted(ConstArray *array, key_val_func key);
Array *arrayCocktailShakerSorted(ConstArray *array, key_val_func key);
Array *arrayGnomeSorted(ConstArray *array, key_val_func key);
// AVG: O(n log n)
Array *arrayMergeSorted  (ConstArray *array, key_val_func key);
Array *arrayHeapSorted(ConstArray *array, key_val_func key);
Array *arrayTimSorted(ConstArray *array, key_val_func key);
Array *arrayIntroSorted(ConstArray *array, key_val_func key);
// AVG: O(n log² n)
Array *arrayShellSorted(ConstArray *array, key_val_func key);
// AVG: O(n log n) ; WORST O(n²)
Array *arrayQuickSorted  (ConstArray *array, key_val_func key);

