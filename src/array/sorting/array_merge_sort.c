/// Merge Sort O(n log n)

#include "../../../include/bds/array/bds_array_core.h"
#include "../../../include/bds/array/bds_array_sort.h"

#include <stdlib.h>

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

    void **temp = (void **)malloc(length * sizeof(void *));
    if (!temp) return;  // Memory allocation failed

    mergeSortRecursive(array, temp, 0, length, key);
    free(temp);
}

Array *arrayMergeSorted(const Array *array, const key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    arrayMergeSort(sorted_array, key);

    return sorted_array;
}

