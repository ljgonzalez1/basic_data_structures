#include "../../../include/bds/array/bds_array_core.h"
#include "../../../include/bds/array/bds_array_sort.h"

// Hybrid sorting algorithm derived from merge sort and insertion sort.
// Uses RUNs of ordered elements to optimize sorting time.

/*
 * 1. **Detect runs**: Detect runs of ordered elements in subarrays (both ascending and descending).
 * Eg.: [*2*, *3*, *8*, *9*, _7_, _5_, _1_, *2*, *3*, *4*] ---> [2, 3, 8, 9] && [7, 5, 1] && [2, 3, 4]
 * 2. **Normalize runs**: Reverse descending runs to make them ascending.
 * Eg.: [7, 5, 1] ---> [1, 5, 7]
 * 3. **minrun**: Fixed minimum run size (e.g., 32). If a run is smaller, extend it using **insertion sort**
 * **INSERTION SORT WORKS BETTER WITH PARTIALLY SORTED ARRAYS**.
 * 4. **Stack Runs**: Create a stack of runs.
 * R1:[2, 3, 8, 9] && R2:[7, 5, 1] && R3:[2, 3, 4] --> Stack:[R1, R2, R3]
 * 5. **Merge**: Merge runs from the stack while maintaining certain invariants to ensure efficient merging.
 * |R(n-2)| <= |R(n-1)| + |R(n)|
 * |R(n-1)| <= |R(n)|
 * 6. **Galloping**: Repeatedly merge runs until sorted using **galloping mode** to optimize merging.
 *
 * Galloping mode:
 *  (Galloping) Exponential search to find the point where elements from one run should be inserted into another,
*/
void arrayTimSort(Array *array, const key_val_func key) {
    const size_t length = arrayLength(array);
    if (length < 2) return;

    // TODO: ===================
    // TODO: SORTING ALGORITHM HERE
    // TODO: ===================
}

Array *arrayTimSorted(const Array *array, key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    arrayTimSort(sorted_array, key);

    return sorted_array;
}
