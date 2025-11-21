#include "../../include/bds/array/bds_array_core.h"
#include "../../include/bds/array/bds_array_sort.h"

static inline void swap(Array *array, size_t idx1, size_t idx2) {
    void *temp = arrayGet(array, idx1);
    arraySet(array, idx1, arrayGet(array, idx2));
    arraySet(array, idx2, temp);
}

/// Sorting (return NEW sorted array)
Array *arrayBubbleSorted (const Array *array, key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    size_t length = arrayLength(sorted_array);

    if (length < 2) return sorted_array;

    bool swapped;

    do {
        swapped = false;

        for (size_t idx = 0; idx < length - 1; idx++) {
            size_t next_idx = idx + 1;

            int this_val = key(arrayGet(sorted_array, idx));
            int next_val = key(arrayGet(sorted_array, next_idx));

            if (this_val > next_val) {
                swap(sorted_array, idx, next_idx);
                swapped = true;
            }
        }

        length--;  // Last element is in place, no need to check again.
    } while (swapped);

    return sorted_array;
}


Array *arrayInsertionSorted(const Array *array, key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    // TODO: ===================
    // TODO: SORT ALGORITHM HERE
    // TODO: ===================

    return sorted_array;
}


Array *arraySelectionSorted(const Array *array, key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    // TODO: ===================
    // TODO: SORT ALGORITHM HERE
    // TODO: ===================

    return sorted_array;
}


Array *arrayCocktailShakerSorted(const Array *array, key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    // TODO: ===================
    // TODO: SORT ALGORITHM HERE
    // TODO: ===================

    return sorted_array;
}


Array *arrayGnomeSorted(const Array *array, key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    // TODO: ===================
    // TODO: SORT ALGORITHM HERE
    // TODO: ===================

    return sorted_array;
}

Array *arrayMergeSorted  (const Array *array, key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    // TODO: ===================
    // TODO: SORT ALGORITHM HERE
    // TODO: ===================

    return sorted_array;
}


Array *arrayHeapSorted(const Array *array, key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    // TODO: ===================
    // TODO: SORT ALGORITHM HERE
    // TODO: ===================

    return sorted_array;
}


Array *arrayTimSorted(const Array *array, key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    // TODO: ===================
    // TODO: SORT ALGORITHM HERE
    // TODO: ===================

    return sorted_array;
}


Array *arrayIntroSorted(const Array *array, key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    // TODO: ===================
    // TODO: SORT ALGORITHM HERE
    // TODO: ===================

    return sorted_array;
}


// AVG: O(n log² n)
Array *arrayShellSorted(const Array *array, key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    // TODO: ===================
    // TODO: SORT ALGORITHM HERE
    // TODO: ===================

    return sorted_array;
}


// AVG: O(n log n) ; WORST O(n²)
Array *arrayQuickSorted  (const Array *array, key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    // TODO: ===================
    // TODO: SORT ALGORITHM HERE
    // TODO: ===================

    return sorted_array;
}

