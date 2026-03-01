#include "../../../include/bds/heap/bds_heap_core.h"
#include "../../../include/bds/heap/bds_heap_utils.h"

#include <stdlib.h>

void minHeapShiftDown(MinHeap *min_heap, size_t index, const key_val_func key) {
    if (!minHeapExists(min_heap) || !min_heap->data || !key) return;

    const size_t heap_length = minHeapLength(min_heap);

    if (index >= min_heap->length) return;

    while (true) {
        const size_t left = heapLeftChildIdx(index);
        if (left >= heap_length) return; // no children => done

        const size_t right = heapRightChildIdx(index);

        // pick the child with the smaller key
        size_t smallest = left;
        if (right < heap_length && heapKeyCompare(
                min_heap->data[right],
                min_heap->data[left],
                key
            ) < 0) {

            smallest = right;
        }

        // if parent <= smallest child, heap property holds
        if (heapKeyCompare(
                min_heap->data[index],
                min_heap->data[smallest], key)
                <= 0) return;

        heapSwap((Heap *)min_heap, index, smallest);
        index = smallest;
    }
}


void minHeapShiftUp(MinHeap *min_heap, size_t index, const key_val_func key) {
    if (!minHeapExists(min_heap) || !min_heap->data || !key) return;

    const size_t min_heap_length = min_heap->length;

    if (index >= min_heap_length) return;

    while (index > 0) {
        const size_t parent = heapParentIdx(index);

        // if parent <= child, heap property holds
        if (heapKeyCompare(
                min_heap->data[parent],
                min_heap->data[index],
                key) <= 0) return;

        heapSwap((Heap *)min_heap, parent, index);
        index = parent;
    }
}


void minHeapHeapify(MinHeap *min_heap, const key_val_func key) {
    if (!minHeapExists(min_heap) || minHeapIsEmpty(min_heap)) return;

    for (size_t idx = minHeapLength(min_heap) >> 1; idx > 0; idx--) {
        minHeapShiftDown(min_heap, idx - 1, key);
    }
}


static MinHeap *minHeapTailToHead(MinHeap *min_Heap) {
    if (!minHeapExists(min_Heap)) return NULL;
    if (minHeapIsEmpty(min_Heap)) return min_Heap;

    // Empty heap has no self->data.
    if (minHeapLength(min_Heap) == 1) {
        free(min_Heap->data);
        min_Heap->data = NULL;
        min_Heap->length = 0;
        return min_Heap;
    }

    void *tail = min_Heap->data[min_Heap->length - 1];
    min_Heap->data[0] = tail;

    min_Heap->length--;
    // NOTE: NOTE: UNNECESSARY STEP, BUT KEEPS THE MEMORY CLEAN
    min_Heap->data[min_Heap->length] = NULL;

    void **reallocated_heap_data_array = realloc(min_Heap->data, min_Heap->length * sizeof *min_Heap->data);
    // NOTE: IT SHOULD BE FINE ANYWAY. IT'LL JUST GROW A BIT
    if (reallocated_heap_data_array) min_Heap->data = reallocated_heap_data_array;

    return min_Heap;
}


void *minHeapPopMin(MinHeap *min_heap, const key_val_func key) {
    // Takes the extremum (min) from the head of the heap,
    // then moves the tail to the head and shifts down to restore the heap property.
    if (!minHeapExists(min_heap) || minHeapIsEmpty(min_heap)) return NULL;

    void *minimum = min_heap->data[0];

    MinHeap *smaller_heap = minHeapTailToHead(min_heap);

    if (minHeapExists(smaller_heap) && minHeapLength(smaller_heap) > 0) {
        minHeapShiftDown(smaller_heap, 0, key);
    }

    return minimum;
}


bool minHeapAdd(MinHeap *min_heap, void *data, const key_val_func key) {
    if (!minHeapExists(min_heap)) return false;

    const size_t old_len = minHeapLength(min_heap);
    const size_t new_len = old_len + 1;

    void **new_data = realloc(min_heap->data, new_len * sizeof *min_heap->data);
    if (!new_data) return false;

    min_heap->data = new_data;
    min_heap->data[old_len] = data;
    min_heap->length = new_len;

    minHeapShiftUp(min_heap, old_len, key);
    return true;
}

