#pragma once

#include "../bds_types.h"
#include "../array/bds_array.h"

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

// Cast to Heap and Min/Max Heap before and after copy
// MinHeap min_heap_copy = (MinHeap *)heapShallowCopy((Heap *)min_heap);
Heap *heapShallowCopy(const Heap *heap);

void heapFreeWith(Heap *heap, deleter_func deleter);  // Frees payloads according to func
void heapFree(Heap *heap);  // Just frees itself

/// Info
static inline size_t heapLength(const Heap *heap) {
    return heap->length;
}

static inline bool heapIsEmpty(const Heap *heap) {
    return heap->length == 0;
}

/// Access
static inline void *heapGet(const Heap *heap, const size_t index) {
    return heap->data[index];
}

/// Change
// sets heap[index] = data
static inline void heapSet(Heap *heap, const size_t index, void *data) {
    heap->data[index] = data;
}

static inline void heapSwap(Heap *heap, const size_t idx1, const size_t idx2) {
    if (idx1 == idx2) return;

    void *temp = heapGet(heap, idx1);
    heapSet(heap, idx1, heapGet(heap, idx2));
    heapSet(heap, idx2, temp);
}

/// Dependent functions (for consistency with other types)
/// Lifecycle

static inline MinHeap *minHeapShallowCopyConst(const MinHeap *min_heap) {
    return (MinHeap *)heapShallowCopy((const Heap *)min_heap);
}

static inline MaxHeap *maxHeapShallowCopyConst(const MaxHeap *max_heap) {
    return (MaxHeap *)heapShallowCopy((const Heap *)max_heap);
}

static inline void minHeapFreeWith(MinHeap *min_heap, const deleter_func deleter) {
    heapFreeWith((Heap *)min_heap, deleter);
}

static inline void maxHeapFreeWith(MaxHeap *max_heap, const deleter_func deleter) {
    heapFreeWith((Heap *)max_heap, deleter);
}

static inline void minHeapFree(MinHeap *min_heap) {
    heapFree((Heap *)min_heap);
}

static inline void maxHeapFree(MaxHeap *max_heap) {
    heapFree((Heap *)max_heap);
}

/// Info
static inline size_t minHeapLength(const MinHeap *heap) {
    return heapLength((Heap *)heap);
}

static inline size_t maxHeapLength(const MaxHeap *heap) {
    return heapLength((Heap *)heap);
}

static inline bool minHeapIsEmpty(const MinHeap *heap) {
    return heapIsEmpty((const Heap *)heap);
}

static inline bool maxHeapIsEmpty(const MaxHeap *heap) {
    return heapIsEmpty((const Heap *)heap);
}

/// Access (read-only to `void **data[i]`)
static inline void *minHeapGet(const MinHeap *heap, const size_t index) {
    return heapGet((Heap *)heap, index);
}

static inline void *maxHeapGet(const MaxHeap *heap, const size_t index) {
    return heapGet((Heap *)heap, index);
}

static inline void *minHeapGetMin(const MinHeap *min_heap) {
    if (minHeapIsEmpty(min_heap)) return NULL;
    return minHeapGet(min_heap, 0);
}

static inline void *maxHeapGetMax(const MaxHeap *max_heap) {
    if (maxHeapIsEmpty(max_heap)) return NULL;
    return maxHeapGet(max_heap, 0);
}

/// Change
static inline void minHeapSet(MinHeap *heap, const size_t index, void *data) {
    heapSet((Heap *)heap, index, data);
}

static inline void maxHeapSet(MaxHeap *heap, const size_t index, void *data) {
    heapSet((Heap *)heap, index, data);
}

static inline void minHeapSwap(MinHeap *heap, const size_t idx1, const size_t idx2) {
    heapSwap((Heap *)heap, idx1, idx2);
}

static inline void maxHeapSwap(MaxHeap *heap, const size_t idx1, const size_t idx2) {
    heapSwap((Heap *)heap, idx1, idx2);
}

void minHeapify(MinHeap *min_heap, key_val_func key);
void maxHeapify(MaxHeap *max_heap, key_val_func key);

void minHeapifyShiftUp(MinHeap *min_heap, size_t index, key_val_func key);
void maxHeapifyShiftUp(MaxHeap *max_heap, size_t index, key_val_func key);

void minHeapifyShiftDown(MinHeap *min_heap, size_t index, key_val_func key);
void maxHeapifyShiftDown(MaxHeap *max_heap, size_t index, key_val_func key);

MinHeap *minHeapFromArray(Array *array, key_val_func key);
MaxHeap *maxHeapFromArray(Array *array, key_val_func key);

// Returns new shorter heap and frees the old one
MinHeap *minHeapPopMin(const MinHeap *min_heap, key_val_func key);
MaxHeap *maxHeapPopMax(const MaxHeap *max_heap, key_val_func key);

// Returns new longer heap and frees the old one
MinHeap *minHeapPushMin(const MinHeap *min_heap, void *elem, key_val_func key);
MaxHeap *maxHeapPushMax(const MaxHeap *max_heap, void *elem, key_val_func key);
