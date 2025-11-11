#pragma once

#include "bds_array_core.h"

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