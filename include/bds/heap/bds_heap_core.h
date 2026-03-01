#pragma once

#include "../bds_types.h"
#include "../bds_utils.h"

#include <stddef.h>   // size_t
#include <stdbool.h>  // bool

typedef struct bds_heap {
    void **data;
    size_t length;
} Heap;

// Aliases to avoid confusion.
// They do the same thing as Heap.
typedef Heap MinHeap;
typedef Heap MaxHeap;

/// Lifecycle
MinHeap *minHeapNew(size_t length);
MaxHeap *maxHeapNew(size_t length);

MinHeap *minHeapShallowCopy(const MinHeap *min_heap);
MaxHeap *maxHeapShallowCopy(const MaxHeap *max_heap);

void minHeapFreeWith(MinHeap *min_heap, deleter_func deleter);  // Frees payloads according to func
void maxHeapFreeWith(MaxHeap *max_heap, deleter_func deleter);  // Frees payloads according to func

void minHeapFree(MinHeap *min_heap);
void maxHeapFree(MaxHeap *max_heap);

/// Helper

static inline bool _heapExists(const Heap *heap) {
    return this_struct_exists((void *)heap);
}

static inline bool minHeapExists(const MinHeap *min_heap) {
    return _heapExists((const Heap *)min_heap);
}

static inline bool maxHeapExists(const MinHeap *max_heap) {
    return _heapExists((const Heap *)max_heap);
}

/// Info

static inline size_t _heapLength(const Heap *heap) {
    return _heapExists(heap) ? heap->length : 0;
}

static inline size_t minHeapLength(MinHeap *min_heap) {
    return _heapLength((const Heap *)min_heap);
}

static inline size_t maxHeapLength(MaxHeap *max_heap) {
    return _heapLength((const Heap *)max_heap);
}


static inline bool _heapIsEmpty(const Heap *heap) {
    return _heapExists(heap) ? heap->length == 0 : true;
}

static inline bool minHeapIsEmpty(MinHeap *min_heap) {
    return _heapIsEmpty((const Heap *)min_heap);
}

static inline bool maxHeapIsEmpty(MaxHeap *max_heap) {
    return _heapIsEmpty((const Heap *)max_heap);
}

/// Access

void *minHeapGetMin(const MinHeap *min_heap);
void *maxHeapGetMax(const MaxHeap *max_heap);


/// Change

void minHeapify(MinHeap *min_heap, key_val_func key);
void maxHeapify(MaxHeap *max_heap, key_val_func key);

void minHeapifyShiftUp(MinHeap *min_heap, size_t index, key_val_func key);
void maxHeapifyShiftUp(MaxHeap *max_heap, size_t index, key_val_func key);

void minHeapifyShiftDown(MinHeap *min_heap, size_t index, key_val_func key);
void maxHeapifyShiftDown(MaxHeap *max_heap, size_t index, key_val_func key);

// MinHeap *minHeapFromArray(Array *array, key_val_func key);
// MaxHeap *maxHeapFromArray(Array *array, key_val_func key);

// Returns new shorter heap and frees the old one
MinHeap *minHeapPopMin(const MinHeap *min_heap, key_val_func key);
MaxHeap *maxHeapPopMax(const MaxHeap *max_heap, key_val_func key);

// Returns new longer heap and frees the old one
MinHeap *minHeapPushMin(const MinHeap *min_heap, void *elem, key_val_func key);
MaxHeap *maxHeapPushMax(const MaxHeap *max_heap, void *elem, key_val_func key);
