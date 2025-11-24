/// Cocktail Shaker Sort O(n²)

#include "../../../include/bds/array/bds_array_core.h"
#include "../../../include/bds/array/bds_array_sort.h"

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
                arraySwap(array, this_idx, next_idx);
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
                arraySwap(array, prev_idx, this_idx);
                swapped = true;
            }
        }

        start++; // First element is in place
    } while (swapped);
}

Array *arrayCocktailShakerSorted(const Array *array, const key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    arrayCocktailShakerSort(sorted_array, key);

    return sorted_array;
}
