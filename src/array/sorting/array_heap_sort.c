#include "../../../include/bds/array/bds_array_core.h"
#include "../../../include/bds/array/bds_array_sort.h"

void arrayHeapSort(Array *array, const key_val_func key) {
    const size_t length = arrayLength(array);
    if (length < 2) return;

    // TODO: ===================
    // TODO: SORTING ALGORITHM HERE
    // TODO: ===================
}

Array *arrayHeapSorted(const Array *array, const key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    arrayHeapSort(sorted_array, key);

    return sorted_array;
}
