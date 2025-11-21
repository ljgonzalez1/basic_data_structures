#include "../../include/bds/array/bds_array_core.h"
#include "../../include/bds/array/bds_array_sort.h"

#include <stdlib.h>

Array *arrayHeapSorted(const Array *array, key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    // TODO: ===================
    // TODO: SORTING ALGORITHM HERE
    // TODO: ===================

    return sorted_array;
}


Array *arrayTimSorted(const Array *array, key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    // TODO: ===================
    // TODO: SORTING ALGORITHM HERE
    // TODO: ===================

    return sorted_array;
}


Array *arrayIntroSorted(const Array *array, key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    // TODO: ===================
    // TODO: SORTING ALGORITHM HERE
    // TODO: ===================

    return sorted_array;
}


// AVG: O(n log² n)
Array *arrayShellSorted(const Array *array, key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    // TODO: ===================
    // TODO: SORTING ALGORITHM HERE
    // TODO: ===================

    return sorted_array;
}


// AVG: O(n log n) ; WORST O(n²)
Array *arrayQuickSorted  (const Array *array, key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    // TODO: ===================
    // TODO: SORTING ALGORITHM HERE
    // TODO: ===================

    return sorted_array;
}

