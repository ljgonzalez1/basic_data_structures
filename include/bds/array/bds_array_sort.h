#pragma once

#include "bds_array_core.h"

/// Sorting (SAME sorted array)

void arrayOddEvenSort(Array *array, key_val_func key);
void arrayCombSort(Array *array, key_val_func key);

//////////////////////////// AVG: O(n²) ////////////////////////////
void arrayBubbleSort(Array *array, key_val_func key);
void arrayInsertionSort(Array *array, key_val_func key);
void arraySelectionSort(Array *array, key_val_func key);
void arrayCocktailShakerSort(Array *array, key_val_func key);
void arrayGnomeSort(Array *array, key_val_func key);

//////////////////////////// AVG: O(n log n) ////////////////////////////
void arrayMergeSort(Array *array, key_val_func key);
void arrayTimSort(Array *array, key_val_func key);
void arrayIntroSort(Array *array, key_val_func key);

//////////////////////////// AVG: O(n log² n) ///////////////////////////
void arrayShellSort(Array *array, key_val_func key);

//////////////////////////// AVG: O(n log n) ; WORST O(n²) ////////////////////////////
void arrayQuickSort(Array *array, key_val_func key);

/// Sorting (returns NEW sorted array)

Array *arrayOddEvenSorted(const Array *array, key_val_func key);
Array *arrayCombSorted(const Array *array, key_val_func key);

// AVG: O(n²)
Array *arrayBubbleSorted (const Array *array, key_val_func key);
Array *arrayInsertionSorted(const Array *array, key_val_func key);
Array *arraySelectionSorted(const Array *array, key_val_func key);
Array *arrayCocktailShakerSorted(const Array *array, key_val_func key);
Array *arrayGnomeSorted(const Array *array, key_val_func key);

// AVG: O(n log n)
Array *arrayMergeSorted(const Array *array, key_val_func key);
Array *arrayTimSorted(const Array *array, key_val_func key);
Array *arrayIntroSorted(const Array *array, key_val_func key);

// AVG: O(n log² n)
Array *arrayShellSorted(const Array *array, key_val_func key);

// AVG: O(n log n) ; WORST O(n²)
Array *arrayQuickSorted(const Array *array, key_val_func key);
