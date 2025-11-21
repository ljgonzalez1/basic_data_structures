#include "../../../include/bds/array/bds_array_core.h"
#include "../../../include/bds/array/bds_array_sort.h"

void arrayShellSort(Array *array, const key_val_func key) {
    const size_t length = arrayLength(array);
    if (length < 2) return;

    // TODO: ===================
    // TODO: SORTING ALGORITHM HERE
    // TODO: ===================
}

Array *arrayShellSorted(const Array *array, key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    arrayShellSort(sorted_array, key);

    return sorted_array;
}
