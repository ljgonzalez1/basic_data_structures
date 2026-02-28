#pragma once

#include "bds_heap_core.h"

static inline int heapKeyCompare(
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