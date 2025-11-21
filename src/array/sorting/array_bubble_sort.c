#include "../../../include/bds/array/bds_array_core.h"
#include "../../../include/bds/array/bds_array_sort.h"

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

Array *arrayBubbleSorted(const Array *array, const key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    arrayBubbleSort(sorted_array, key);

    return sorted_array;
}
