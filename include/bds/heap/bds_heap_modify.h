#pragma once

#include "bds_heap_core.h"
#include "bds_heap_sort.h"

// Returns new array with the extra element into a sorted array and heapifies it again
// Frees the old array
MaxHeap *maxHeapInsert(const Heap *max_heap, key_val_func key);
MinHeap *minHeapInsert(const Heap *min_heap, key_val_func key);

// Remove min/max element from the min/max heap, re-heapifies the array and returns the new, smallr, array
// Frees the old array
MaxHeap *maxHeapRemove(const Heap *max_heap, key_val_func key);
MinHeap *minHeapRemove(const Heap *min_heap, key_val_func key);
