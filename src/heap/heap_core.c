#include "../../include/bds/heap/bds_heap_core.h"

#include <stdlib.h>

/// ///

static Heap *_heapNew(const size_t length) {
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
    return (MinHeap *)_heapNew(length);
}

MaxHeap *maxHeapNew(const size_t length) {
    return (MaxHeap *)_heapNew(length);
}

/// ///

static Heap *_heapShallowCopy(const Heap *heap) {
    if (!_heapExists(heap)) return NULL;

    Heap *newHeap = _heapNew(heap->length);
    if (!newHeap) return NULL;

    for (size_t i = 0; i < heap->length; i++) {
        newHeap->data[i] = heap->data[i];
    }

    return newHeap;
}

MinHeap *minHeapShallowCopy(const MinHeap *min_heap) {
    return (MinHeap *)_heapShallowCopy((const Heap *)min_heap);
}

MaxHeap *maxHeapShallowCopy(const Heap *heap) {
    return (MaxHeap *)_heapShallowCopy(heap);
}

/// ///

static void _heapFree(Heap *heap) {
    if (!_heapExists(heap)) return;

    free(heap->data);
    free(heap);
}

void minHeapFree(MinHeap *min_heap) {
    _heapFree((Heap *)min_heap);
}

void maxHeapFree(MaxHeap *max_heap) {
    _heapFree((Heap *)max_heap);
}

static void _heapFreeWith(Heap *heap, const deleter_func deleter) {
    if (!_heapExists(heap)) return;

    if (!!deleter) {
        for (size_t i = 0; i < heap->length; i++) {
            deleter(heap->data[i]);
        }
    }

    _heapFree(heap);
}

void minHeapFreeWith(MinHeap *min_heap, const deleter_func deleter) {
    _heapFreeWith((Heap *)min_heap, deleter);
}

void maxHeapFreeWith(MaxHeap *max_heap, const deleter_func deleter) {
    _heapFreeWith((Heap *)max_heap, deleter);
}

/// ///

static inline void *_heapGetExtremum(const Heap *heap) {
    if (_heapIsEmpty(heap)) return NULL;
    return heap->data[0];
}

void *minHeapGetMin(const MinHeap *min_heap) {
    return _heapGetExtremum(min_heap);
}

void *minHeapGetMax(const MinHeap *min_heap) {
    return _heapGetExtremum(min_heap);
}









