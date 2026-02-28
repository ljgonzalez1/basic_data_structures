#include "../../include/bds/heap/bds_heap_core.h"

#include <stdlib.h>

/// ///

static Heap *__heapNew(const size_t length) {
    Heap *heap = (Heap *)malloc(sizeof *heap);
    if (!heap) return NULL;

    heap->data = (void **)malloc(length * sizeof(void *));
    if (!heap->data) {
        free(heap);
        return NULL;
    }

    heap->length = length;

    return heap;
}

MinHeap *minHeapNew(const size_t length) {
    return (MinHeap *)__heapNew(length);
}

MaxHeap *maxHeapNew(const size_t length) {
    return (MaxHeap *)__heapNew(length);
}

/// ///

static Heap *__heapShallowCopy(const Heap *heap) {
    if (!__heapExists(heap)) return NULL;

    Heap *newHeap = __heapNew(heap->length);
    if (!newHeap) return NULL;

    for (size_t i = 0; i < heap->length; i++) {
        newHeap->data[i] = heap->data[i];
    }

    return newHeap;
}

MinHeap *minHeapShallowCopy(const MinHeap *min_heap) {
    return (MinHeap *)__heapShallowCopy((const Heap *)min_heap);
}

MaxHeap *maxHeapShallowCopy(const Heap *heap) {
    return (MaxHeap *)__heapShallowCopy(heap);
}

/// ///

static void __heapFree(Heap *heap) {
    if (!__heapExists(heap)) return;

    free(heap->data);
    free(heap);
}

void minHeapFree(MinHeap *min_heap) {
    __heapFree((Heap *)min_heap);
}

void maxHeapFree(MaxHeap *max_heap) {
    __heapFree((Heap *)max_heap);
}

static void __heapFreeWith(Heap *heap, const deleter_func deleter) {
    if (!__heapExists(heap)) return;

    if (!!deleter) {
        for (size_t i = 0; i < heap->length; i++) {
            deleter(heap->data[i]);
        }
    }

    __heapFree(heap);
}

void minHeapFreeWith(MinHeap *min_heap, const deleter_func deleter) {
    __heapFreeWith((Heap *)min_heap, deleter);
}

void maxHeapFreeWith(MaxHeap *max_heap, const deleter_func deleter) {
    __heapFreeWith((Heap *)max_heap, deleter);
}

/// ///

static inline void *__heapGetExtremum(const Heap *heap) {
    if (__heapIsEmpty(heap)) return NULL;
    return heap->data[0];
}

void *minHeapGetMin(const MinHeap *min_heap) {
    return __heapGetExtremum(min_heap);
}

void *minHeapGetMax(const MinHeap *min_heap) {
    return __heapGetExtremum(min_heap);
}

/// ///








