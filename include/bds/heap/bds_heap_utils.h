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

static inline size_t heapParentIdx(const size_t idx) {
    return (idx - 1) / 2;
}

static inline size_t heapLeftChildIdx(const size_t idx) {
    return 2 * idx + 1;
}

static inline size_t heapRightChildIdx(const size_t idx) {
    return 2 * idx + 2;
}

static inline void heapSwap(Heap *heap, const size_t idx1, const size_t idx2) {
    if (idx1 == idx2) return;

    void *temp = heap->data[idx1];
    heap->data[idx1] = heap->data[idx2];
    heap->data[idx2] = temp;
}