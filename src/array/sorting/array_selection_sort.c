/// Selection Sort O(n²)

#include "../../../include/bds/array/bds_array_sort.h"
#include "../../../include/bds/array/bds_array_utils.h"


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
            arraySwap(array, idx, min_idx);
        }
    }
}

Array *arraySelectionSorted(const Array *array, const key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    arraySelectionSort(sorted_array, key);

    return sorted_array;
}
