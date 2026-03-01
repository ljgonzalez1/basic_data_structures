#include "../../include/bds/heap/bds_heap_core.h"

#include <stdlib.h>


static void _heapShiftUp(Heap *heap, const size_t idx, const key_val_func key) {
    // TODO:
}

static void _heapShiftDown(Heap *heap, const size_t idx, const key_val_func key) {
    // TODO:
}

/// ///

static void _heapHeapify(Heap *heap, const key_val_func key) {
    // TODO:
}

void minHeapHeapify(MinHeap *min_heap, const key_val_func key) {
    _heapHeapify((Heap *)min_heap, key);
}

void maxHeapHeapify(MaxHeap *max_heap, const key_val_func key) {
    _heapHeapify((Heap *)max_heap, key);
}

/// ///

static Heap *_heapTailToHead(Heap *heap) {
    if (!_heapExists(heap)) return NULL;
    if (_heapIsEmpty(heap)) return heap;

    // Empty heap has no self->data.
    if (_heapLength(heap) == 1) {
        free(heap->data);
        heap->data = NULL;
        heap->length = 0;
        return heap;
    }

    void *tail = heap->data[heap->length - 1];
    heap->data[0] = tail;

    heap->length--;
    // TODO: NOTE: UNNECESSARY STEP, BUT KEEPS THE MEMORY CLEAN
    heap->data[heap->length] = NULL;

    void **reallocated_heap_data_array = realloc(heap->data, heap->length * sizeof *heap->data);
    // TODO: IT SHOULD BE FINE ANYWAYS. IT'LL JUST GROW A BIT
    if (reallocated_heap_data_array) heap->data = reallocated_heap_data_array;

    return heap;
}


static void *_heapPopExtremum(Heap *heap, const key_val_func key) {
    // Takes the extremum (min or max) from the head of the heap,
    // then moves the tail to the head and shifts down to restore the heap property.
    if (!_heapExists(heap) || _heapIsEmpty(heap)) return NULL;

    void *extremum = heap->data[0];

    Heap *smaller_heap = _heapTailToHead(heap);

    if (_heapExists(smaller_heap) && _heapLength(smaller_heap) > 0) {
        _heapShiftDown(smaller_heap, 0, key);
    }

    return extremum;
}

void *minHeapPopMin(MinHeap *min_heap, const key_val_func key) {
    return _heapPopExtremum((Heap *)min_heap, key);
}

void *maxHeapPopMax(MaxHeap *max_heap, const key_val_func key) {
    return _heapPopExtremum((Heap *)max_heap, key);
}

/// ///

static bool _heapAdd(Heap *heap, void *data, const key_val_func key) {
    if (!_heapExists(heap)) return false;

    const size_t old_len = heap->length;
    const size_t new_len = old_len + 1;

    void **new_data = realloc(heap->data, new_len * sizeof *heap->data);
    if (!new_data) return false;

    heap->data = new_data;
    heap->data[old_len] = data;
    heap->length = new_len;

    _heapShiftUp(heap, old_len, key);
    return true;
}

bool minHeapAdd(MinHeap *min_heap, void *elem, const key_val_func key) {
    return _heapAdd((Heap *)min_heap, elem, key);
}


bool maxHeapAdd(MaxHeap *max_heap, void *elem, const key_val_func key) {
    return _heapAdd((Heap *)max_heap, elem, key);
}


