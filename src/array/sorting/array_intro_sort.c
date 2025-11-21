#include "../../../include/bds/array/bds_array_core.h"
#include "../../../include/bds/array/bds_array_sort.h"

void arrayIntroSort(Array *array, const key_val_func key) {
    const size_t length = arrayLength(array);
    if (length < 2) return;

    // TODO: ===================
    // TODO: SORTING ALGORITHM HERE
    // TODO: ===================
}

Array *arrayIntroSorted(const Array *array, const key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    arrayIntroSort(sorted_array, key);

    return sorted_array;
}