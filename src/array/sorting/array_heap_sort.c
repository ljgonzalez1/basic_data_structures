// Heapsort O(n log n)

#include "../../../include/bds/array/bds_array_sort.h"

/// ===============================================================
/// Max-heap helpers (operating directly over Array)
/// ===============================================================

/**
 * Sifts a node down the heap to restore the max-heap property.
 * Operates in the index range [0, heap_size).
 */
static void heapSiftDown(
    Array *array,
    const size_t start_idx,
    const size_t heap_size,
    const key_val_func key
) {
    size_t root = start_idx;

    while (true) {
        const size_t left  = (root << 1) + 1; // left child index
        const size_t right = left + 1;        // right child index

        // Root has no children -> already a leaf
        if (left >= heap_size) break;

        size_t largest = root;
        int largest_key = key(arrayGet(array, largest));

        // Compare with left child
        const int left_key = key(arrayGet(array, left));

        if (left_key > largest_key) {
            largest = left;
            largest_key = left_key;
        }

        // Compare with right child if it exists
        if (right < heap_size) {
            const int right_key = key(arrayGet(array, right));

            if (right_key > largest_key) largest = right;
        }

        // If root is already the largest, heap property holds
        if (largest == root) break;

        arraySwap(array, root, largest);
        root = largest; // continue sifting down
    }
}

/**
 * Builds a max-heap in O(n) time.
 * After completion: array[0] is the maximum element.
 */
static void buildMaxHeap(
    Array *array,
    const size_t length,
    const key_val_func key
) {
    if (length < 2) return;

    // Last parent node = (length / 2) - 1
    // Use "i-- > 0" to safely iterate backwards with size_t
    for (size_t i = length / 2; i-- > 0; ) {
        heapSiftDown(array, i, length, key);
    }
}

/// ===============================================================
/// Heapsort (ascending order)
/// ===============================================================

/**
 * In-place heapsort (ascending).
 * Steps:
 *   1) Build max-heap.
 *   2) Repeatedly move max element to the end and shrink heap.
 */
void arrayHeapSort(
    Array *array,
    const key_val_func key
) {
    const size_t length = arrayLength(array);
    if (length < 2) return;

    // Step 1: build max-heap
    buildMaxHeap(array, length, key);

    // Step 2: extract max repeatedly
    size_t heap_size = length;

    while (heap_size > 1) {
        heap_size--;                     // last valid index of heap
        arraySwap(array, 0, heap_size);  // move max to its final position
        heapSiftDown(array, 0, heap_size, key);
    }
}

/**
 * Returns a new shallow-copied array, sorted using heapsort.
 * Returns NULL on allocation failure.
 */
Array *arrayHeapSorted(
    const Array *array,
    const key_val_func key
) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    arrayHeapSort(sorted_array, key);
    return sorted_array;
}
