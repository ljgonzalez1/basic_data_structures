#include "../../../include/bds/array/bds_array_core.h"
#include "../../../include/bds/array/bds_array_sort.h"

/* **INSERTION SORT WORKS BETTER WITH PARTIALLY SORTED ARRAYS**. */
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

Array *arrayInsertionSorted(const Array *array, const key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    arrayInsertionSort(sorted_array, key);

    return sorted_array;
}
