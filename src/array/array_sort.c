#include "../../include/bds/array/bds_array_core.h"
#include "../../include/bds/array/bds_array_sort.h"

#include <stdlib.h>

static inline void swap(Array *array, const size_t idx1, const size_t idx2) {
    if (idx1 == idx2) return;

    void *temp = arrayGet(array, idx1);
    arraySet(array, idx1, arrayGet(array, idx2));
    arraySet(array, idx2, temp);
}

void arrayBubbleSort(Array *array, const key_val_func key) {
    size_t length = arrayLength(array);
    if (length < 2) return;

    bool swapped;

    do {
        swapped = false;

        for (size_t idx = 0; idx < length - 1; idx++) {
            const size_t next_idx = idx + 1;

            const int this_val = key(arrayGet(array, idx));
            const int next_val = key(arrayGet(array, next_idx));

            if (this_val > next_val) {
                swap(array, idx, next_idx);
                swapped = true;
            }
        }

        length--;  // Last element is in place, no need to check again.
    } while (swapped);
}

void arrayInsertionSort(Array *array, const key_val_func key) {
    // Subarray sorted at the left
    const size_t length = arrayLength(array);
    if (length < 2) return;

    for (size_t idx = 1; idx < length; idx++) {
        void *to_insert_elem = arrayGet(array, idx);
        const int key_to_insert = key(to_insert_elem);

        size_t shifted_idx = idx;

        while (
            shifted_idx > 0 &&
            key(arrayGet(array, shifted_idx - 1)) > key_to_insert) {

            void *displaced_elem = arrayGet(array, shifted_idx - 1);
            arraySet(array, shifted_idx, displaced_elem);
            shifted_idx--;
        }

        arraySet(array, shifted_idx, to_insert_elem);
    }
}

void arraySelectionSort(Array *array, const key_val_func key) {
    // Swaps minimum to its correct position.
    const size_t length = arrayLength(array);
    if (length < 2) return;

    // Last one is not considered
    for (size_t idx = 0; idx < length - 1; idx++) {
        // We assume the minimum is at idx
        size_t min_idx = idx;
        int min_val = key(arrayGet(array, idx));

        // find minimum in the rest of the array;
        for (size_t idx_right = idx + 1; idx_right < length; idx_right++) {
            const int idx_right_val = key(arrayGet(array, idx_right));

            if (idx_right_val < min_val) {
                min_val = idx_right_val;
                min_idx = idx_right;
            }
        }

        // swap minimum to its correct position
        if (min_idx != idx) {
            swap(array, idx, min_idx);
        }
    }
}

void arrayCocktailShakerSort(Array *array, const key_val_func key) {
    // Bubble sort to the right, then to the left, narrowing the bounds each time.
    // More efficient than bubble sort on average.
    // Though still O(n²) time complexity.
    size_t length = arrayLength(array);
    if (length < 2) return;

    size_t start = 0;
    size_t end = length - 1;
    bool swapped = true;

    do {
        // Bubble up to the right (forward pass; like bubble sort; highest to the right)
        swapped = false;

        for (size_t idx = start; idx < end; idx++) {
            const size_t this_idx = idx;
            const size_t next_idx = idx + 1;

            const int this_val = key(arrayGet(array, this_idx));
            const int next_val = key(arrayGet(array, next_idx));

            if (this_val > next_val) {
                swap(array, this_idx, next_idx);
                swapped = true;
            }
        }

        // If no swap, the array is sorted
        if (!swapped) break;
        end--;  // Last element is in place

        // Bubble down to the left (backward pass; lowest to the left)
        swapped = false;

        for (size_t idx = end; idx > start; idx--) {
            const size_t this_idx = idx;
            const size_t prev_idx = idx - 1;

            const int this_val = key(arrayGet(array, this_idx));
            const int prev_val = key(arrayGet(array, prev_idx));

            if (prev_val > this_val) {
                swap(array, prev_idx, this_idx);
                swapped = true;
            }
        }

        start++; // First element is in place
    } while (swapped);
}

void arrayGnomeSort(Array *array, const key_val_func key) {
    // Similar to insertion sort
    // Simpler
    // If 2 elements are out of order, swap them and step back
    // Else, step forward
    // Repeat until the end of the array is reached
    const size_t length = arrayLength(array);
    if (length < 2) return;

    size_t start = 0;

    while (start < length - 1) {
        const size_t curr_idx = start;
        const size_t next_idx = start + 1;

        const int curr_val = key(arrayGet(array, curr_idx));
        const int next_val = key(arrayGet(array, next_idx));

        // If in order, step forward
        if (curr_val <= next_val) {
            start++;

        // If out of order, swap...
        // ...and step back (if possible)...
        // ...if not possible, we are at the start, just step forward.
        } else {
            swap(array, curr_idx, next_idx);

            if (start > 0) {
                start--;

            } else {
                start ++;
            }
        }
    }
}

// No clue how this one works...
static void merge(
    Array *array,
    void **temp,

    const size_t left,
    const size_t mid,
    const size_t right,

    const key_val_func key
    ) {

    size_t left_half_idx = left;
    size_t right_half_idx = mid;
    size_t temp_idx = left;  // buffer

    while (left_half_idx < mid && right_half_idx < right) {
        void *left_elem  = arrayGet(array, left_half_idx);
        void *right_elem = arrayGet(array, right_half_idx);

        const int left_key  = key(left_elem);
        const int right_key = key(right_elem);

        if (left_key <= right_key) {
            temp[temp_idx++] = left_elem;
            left_half_idx++;

        } else {
            temp[temp_idx++] = right_elem;
            right_half_idx++;
        }
    }

    // Copy what's left in the left half
    while (left_half_idx < mid) {
        temp[temp_idx++] = arrayGet(array, left_half_idx++);
    }

    // Copy what's left in the right half
    while (right_half_idx < right) {
        temp[temp_idx++] = arrayGet(array, right_half_idx++);
    }

    // Copy back to the original array
    for (size_t idx = left; idx < right; idx++) {
        arraySet(array, idx, temp[idx]);
    }
}

static void mergeSortRecursive(
    Array *array,
    void **temp,

    const size_t left,
    const size_t right,

    const key_val_func key
    ) {

    // [left, right) = subarray to sort
    if (right - left < 2) return;  // Size 0 or 1 array is already sorted

    // >>> "divide et impera"
    // mid_point = (right - left) / 2
    const size_t midpoint_diff = (right - left) >> 1;
    const size_t mid = left + midpoint_diff;

    // Sort each half by itself recursively
    mergeSortRecursive(array, temp, left, mid,  key);
    mergeSortRecursive(array, temp, mid,  right, key);

    // Merge sorted halves
    merge(array, temp, left, mid, right, key);
}

void arrayMergeSort(Array *array, const key_val_func key) {
    const size_t length = arrayLength(array);
    if (length < 2) return;

    void **temp = (void **)bds_malloc(length * sizeof(void *));
    if (!temp) return;  // Memory allocation failed

    mergeSortRecursive(array, temp, 0, length, key);
    bds_free(temp);
}

Array *arrayBubbleSorted (const Array *array, const key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    arrayBubbleSort(sorted_array, key);

    return sorted_array;
}

Array *arrayInsertionSorted(const Array *array, const key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    arrayInsertionSort(sorted_array, key);

    return sorted_array;
}

Array *arraySelectionSorted(const Array *array, const key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    arraySelectionSort(sorted_array, key);

    return sorted_array;
}

Array *arrayCocktailShakerSorted(const Array *array, const key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    arrayCocktailShakerSort(sorted_array, key);

    return sorted_array;
}

Array *arrayGnomeSorted(const Array *array, const key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    arrayGnomeSort(sorted_array, key);

    return sorted_array;
}

Array *arrayMergeSorted(const Array *array, const key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    arrayMergeSort(sorted_array, key);

    return sorted_array;
}


Array *arrayHeapSorted(const Array *array, key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    // TODO: ===================
    // TODO: SORTING ALGORITHM HERE
    // TODO: ===================

    return sorted_array;
}


Array *arrayTimSorted(const Array *array, key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    // TODO: ===================
    // TODO: SORTING ALGORITHM HERE
    // TODO: ===================

    return sorted_array;
}


Array *arrayIntroSorted(const Array *array, key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    // TODO: ===================
    // TODO: SORTING ALGORITHM HERE
    // TODO: ===================

    return sorted_array;
}


// AVG: O(n log² n)
Array *arrayShellSorted(const Array *array, key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    // TODO: ===================
    // TODO: SORTING ALGORITHM HERE
    // TODO: ===================

    return sorted_array;
}


// AVG: O(n log n) ; WORST O(n²)
Array *arrayQuickSorted  (const Array *array, key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    // TODO: ===================
    // TODO: SORTING ALGORITHM HERE
    // TODO: ===================

    return sorted_array;
}

