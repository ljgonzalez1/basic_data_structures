#include "../../../include/bds/heap/bds_heap_core.h"
#include "../../../include/bds/heap/bds_heap_utils.h"

#include <stdlib.h>

void maxHeapShiftDown(MaxHeap *max_heap, size_t index, const key_val_func key) {
    if (!maxHeapExists(max_heap) || !max_heap->data || !key) return;

    const size_t heap_length = maxHeapLength(max_heap);

    if (index >= max_heap->length) return;

    while (true) {
        const size_t left = heapLeftChildIdx(index);
        if (left >= heap_length) return; // no children => done

        const size_t right = heapRightChildIdx(index);

        // pick the child with the larger key
        size_t largest = left;
        if (right < heap_length &&
            heapKeyCompare(
                max_heap->data[right],
                max_heap->data[left],
                key) > 0
                ) {
            largest = right;
            }

        // if parent >= largest child, heap property holds
        if (heapKeyCompare(
                max_heap->data[index],
                max_heap->data[largest],
                key) >= 0) return;

        heapSwap((Heap *)max_heap, index, largest);
        index = largest;
    }
}


void maxHeapShiftUp(MaxHeap *max_heap, size_t index, const key_val_func key) {
    if (!maxHeapExists(max_heap) || !max_heap->data || !key) return;

    const size_t max_heap_length = max_heap->length;

    if (index >= max_heap_length) return;

    while (index > 0) {
        const size_t parent = heapParentIdx(index);

        // if parent >= child, heap property holds
        if (heapKeyCompare(
                max_heap->data[parent],
                max_heap->data[index],
                key) >= 0) return;

        heapSwap((Heap *)max_heap, parent, index);
        index = parent;
    }
}

void maxHeapHeapify(MaxHeap *max_heap, const key_val_func key) {
    if (!maxHeapExists(max_heap) || maxHeapIsEmpty(max_heap)) return;

    for (size_t idx = maxHeapLength(max_heap) >> 1; idx > 0; idx--) {
        maxHeapShiftDown(max_heap, idx - 1, key);
    }
}

static MaxHeap *maxHeapTailToHead(MaxHeap *max_heap) {
    if (!maxHeapExists(max_heap)) return NULL;
    if (maxHeapIsEmpty(max_heap)) return max_heap;

    // Empty heap has no self->data.
    if (maxHeapLength(max_heap) == 1) {
        free(max_heap->data);
        max_heap->data = NULL;
        max_heap->length = 0;
        return max_heap;
    }

    void *tail = max_heap->data[max_heap->length - 1];
    max_heap->data[0] = tail;

    max_heap->length--;
    // NOTE: NOTE: UNNECESSARY STEP, BUT KEEPS THE MEMORY CLEAN
    max_heap->data[max_heap->length] = NULL;

    void **reallocated_heap_data_array = realloc(max_heap->data, max_heap->length * sizeof *max_heap->data);
    // NOTE: IT SHOULD BE FINE ANYWAY. IT'LL JUST GROW A BIT
    if (reallocated_heap_data_array) max_heap->data = reallocated_heap_data_array;

    return max_heap;
}

void *maxHeapPopMax(MaxHeap *max_heap, const key_val_func key) {
    // Takes the extremum (max) from the head of the heap,
    // then moves the tail to the head and shifts down to restore the heap property.
    if (!maxHeapExists(max_heap) || maxHeapIsEmpty(max_heap)) return NULL;

    void *maximum = max_heap->data[0];

    MaxHeap *smaller_heap = maxHeapTailToHead(max_heap);

    if (_heapExists(smaller_heap) && maxHeapLength(smaller_heap) > 0) {
        maxHeapShiftDown(smaller_heap, 0, key);
    }

    return maximum;
}


bool maxHeapAdd(MaxHeap *max_heap, void *data, const key_val_func key) {
    if (!maxHeapExists(max_heap)) return false;

    const size_t old_len = maxHeapLength(max_heap);
    const size_t new_len = old_len + 1;

    void **new_data = realloc(max_heap->data, new_len * sizeof *max_heap->data);
    if (!new_data) return false;

    max_heap->data = new_data;
    max_heap->data[old_len] = data;
    max_heap->length = new_len;

    maxHeapShiftUp(max_heap, old_len, key);
    return true;
}
