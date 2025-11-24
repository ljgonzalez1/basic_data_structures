#pragma once

#include "bds_heap_core.h"

static inline void heapSwap(Heap *heap, const size_t idx1, const size_t idx2) {
    if (idx1 == idx2) return;

    void *temp = heapGet(heap, idx1);
    heapSet(heap, idx1, heapGet(heap, idx2));
    heapSet(heap, idx2, temp);
}

static inline void minHeapSwap(MinHeap *heap, const size_t idx1, const size_t idx2) {
    heapSwap((Heap *)heap, idx1, idx2);
}

static inline void maxHeapSwap(MaxHeap *heap, const size_t idx1, const size_t idx2) {
    heapSwap((Heap *)heap, idx1, idx2);
}
