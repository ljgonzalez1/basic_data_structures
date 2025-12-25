/// Selection Sort O(n²)

#include "../../../include/bds/array/bds_array_sort.h"
#include "../../../include/bds/array/bds_array_utils.h"


void arraySelectionSort(Array *array, const key_val_func key) {
    // Swaps minimum to its correct position.
    
    //  0     1   2   3   4   5   6   7   8   9  10
    // ----------------------------------------------  
    // [  7,  5,  6,  2, 45,  9,  6, 45,  1,  1, 96]
    // [  7,  5,  6,  2, 45,  9,  6, 45, *1,  1, 96] // First minimum
    // [ *1,  5,  6,  2, 45,  9,  6, 45, *7,  1, 96] // Swap to the start
    // [  1,  5,  6,  2, 45,  9,  6, 45,  7, *1, 96]
    // [  1, *1,  6,  2, 45,  9,  6, 45,  7, *5, 96]
    // [  1,  1,  6, *2, 45,  9,  6, 45,  7,  5, 96]
    // [  1,  1, *2, *6, 45,  9,  6, 45,  7,  5, 96]
    // [  1,  1,  2,  6, 45,  9,  6, 45,  7, *5, 96]
    // [  1,  1,  2, *5, 45,  9,  6, 45,  7, *6, 96]
    // [  1,  1,  2,  5, 45,  9, *6, 45,  7,  6, 96]
    // [  1,  1,  2,  5, *6,  9,*45, 45,  7,  6, 96]
    // [  1,  1,  2,  5,  6,  9, 45, 45,  7, *6, 96]
    // [  1,  1,  2,  5,  6, *6, 45, 45,  7, *9, 96]
    // [  1,  1,  2,  5,  6,  6, 45, 45, *7,  9, 96]
    // [  1,  1,  2,  5,  6,  6, *7, 45,*45,  9, 96]
    // [  1,  1,  2,  5,  6,  6,  7, 45, 45, *9, 96]
    // [  1,  1,  2,  5,  6,  6,  7, *9, 45,*45, 96]
    // [  1,  1,  2,  5,  6,  6,  7,  9,*45, 45, 96]
    // [  1,  1,  2,  5,  6,  6,  7,  9, 45,*45, 96]
    // [  1,  1,  2,  5,  6,  6,  7,  9, 45, 45,*96]
    // [  1,  1,  2,  5,  6,  6,  7,  9, 45, 45, 96]

    // T(n)= n * Σ_{i=0}^{n}{ n-i }

    // O[n * Σ_{i=0}^{n}{ n-i }]
    //  = O[n * Σ_{i=1}^{n}{ i }]
    //  = O[n²(n+1)/2] 
    //  = O[n²]

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
