#include "../../include/bds/heap/bds_heap_core.h"
#include "../../include/bds/heap/bds_heap_utils.h"

#include <stdlib.h>


static void _heapShiftUp(Heap *heap, const size_t idx) {
    // TODO:
}

static void _heapShiftDown(Heap *heap, const size_t idx) {
    // TODO:
}

/// ///

static void _heapHeapify(Heap *heap) {
    // TODO:
}

void minHeapHeapify(MinHeap *min_heap) {
    _heapHeapify((Heap *)min_heap);
}

void maxHeapHeapify(MaxHeap *max_heap) {
    _heapHeapify((Heap *)max_heap);
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


static void *_heapPopExtremum(Heap *heap) {
    // Takes the extremum (min or max) from the head of the heap,
    // then moves the tail to the head and shifts down to restore the heap property.
    if (!_heapExists(heap) || _heapIsEmpty(heap)) return NULL;

    void *extremum = heap->data[0];

    Heap *smaller_heap = _heapTailToHead(heap);
    _heapShiftDown(smaller_heap, 0);

    return extremum;
}

MinHeap *minHeapPopMin(const MinHeap *min_heap, key_val_func key) {
    return (MinHeap *)_heapPopExtremum((Heap *)min_heap);
}

MaxHeap *maxHeapPopMax(const MaxHeap *max_heap, key_val_func key) {
    return (MaxHeap *)_heapPopExtremum((Heap *)max_heap);
}

/// ///

static bool _heapAdd(Heap *heap, void *data) {
    if (!_heapExists(heap)) return false;

    const size_t old_len = heap->length;
    const size_t new_len = old_len + 1;

    void **new_data = realloc(heap->data, new_len * sizeof *heap->data);
    if (!new_data) return false;

    heap->data = new_data;
    heap->data[old_len] = data;
    heap->length = new_len;

    _heapShiftUp(heap, old_len);
    return true;
}

bool minHeapAdd(const MinHeap *min_heap, void *elem, key_val_func key) {
    return _heapAdd((Heap *)min_heap, elem);
}


bool maxHeapAdd(const MaxHeap *max_heap, void *elem, key_val_func key) {
    return _heapAdd((Heap *)max_heap, elem);
}


