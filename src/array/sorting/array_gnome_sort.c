/// Gnome Sort O(n²)

#include "../../../include/bds/array/bds_array_core.h"
#include "../../../include/bds/array/bds_array_sort.h"

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
            arraySwap(array, curr_idx, next_idx);

            if (start > 0) {
                start--;

            } else {
                start ++;
            }
        }
    }
}

Array *arrayGnomeSorted(const Array *array, const key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    arrayGnomeSort(sorted_array, key);

    return sorted_array;
}
