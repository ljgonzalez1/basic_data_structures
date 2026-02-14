#pragma once

#include "bds_array_core.h"

static inline int arrayKeyCompare(
    const void *datapoint_1,
    const void *datapoint_2,
    const key_val_func key
) {
    const int key_1 = key(datapoint_1);
    const int key_2 = key(datapoint_2);

    if (key_1 < key_2) return -1;
    if (key_1 > key_2) return 1;
    return 0;
}

static inline void arraySwap(Array *array, const size_t idx1, const size_t idx2) {
    if (idx1 == idx2) return;

    void *temp = arrayGet(array, idx1);
    arraySet(array, idx1, arrayGet(array, idx2));
    arraySet(array, idx2, temp);
}

