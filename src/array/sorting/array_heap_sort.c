// Heapsort O(n log n)

#include "../../../include/bds/array/bds_array_sort.h"
#include "../../../include/bds/array/bds_array_utils.h"

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

    /*
    HEAPSORT(A, key)
        n ← length(A)
        if n < 2 then
            return

        // Step 1: build max-heap (Floyd)
        BUILD-MAX-HEAP(A, n, key)

        // Step 2: extract max repeatedly
        heap_size ← n
        while heap_size > 1 do
            heap_size ← heap_size − 1
            swap(A[0], A[heap_size])
            HEAP-SIFT-DOWN(A, 0, heap_size, key)


    BUILD-MAX-HEAP(A, n, key)
        for i ← ⌊n/2⌋ − 1 downto 0 do
            HEAP-SIFT-DOWN(A, i, n, key)


    HEAP-SIFT-DOWN(A, start, heap_size, key)
        root ← start
        while true do
            left  ← 2·root + 1
            right ← left + 1

            if left ≥ heap_size then
                break

            largest ← root

            if key(A[left]) > key(A[largest]) then
                largest ← left

            if right < heap_size and key(A[right]) > key(A[largest]) then
                largest ← right

            if largest = root then
                break

            swap(A[root], A[largest])
            root ← largest
    */

    /*
         0   1   2   3   4   5   6   7   8   9  10
       -------------------------------------------
       [  7,  5,  6,  2, 45,  9,  6, 45,  1,  1, 96 ]  // initial

       Step 1: buildMaxHeap (heap_size = 11)
       [ 96, 45,  9,  7, 45,  6,  6,  2,  1,  1,  5 ]  // after buildMaxHeap (max at index 0)

       Step 2: extract max repeatedly (swap root with last, shrink heap, siftDown)

       [  5, 45,  9,  7, 45,  6,  6,  2,  1,  1 | 96 ]  // extract: swap(0,10)
       [ 45,  5,  9,  7, 45,  6,  6,  2,  1,  1 | 96 ]  // siftDown: swap(0,1)
       [ 45, 45,  9,  7,  5,  6,  6,  2,  1,  1 | 96 ]  // siftDown: swap(1,4)
       [ 45, 45,  9,  7,  5,  6,  6,  2,  1,  1 | 96 ]  // heap restored (heap_size=10)

       [  1, 45,  9,  7,  5,  6,  6,  2,  1 | 45, 96 ]  // extract: swap(0,9)
       [ 45,  1,  9,  7,  5,  6,  6,  2,  1 | 45, 96 ]  // siftDown: swap(0,1)
       [ 45,  7,  9,  1,  5,  6,  6,  2,  1 | 45, 96 ]  // siftDown: swap(1,3)
       [ 45,  7,  9,  2,  5,  6,  6,  1,  1 | 45, 96 ]  // siftDown: swap(3,7)
       [ 45,  7,  9,  2,  5,  6,  6,  1,  1 | 45, 96 ]  // heap restored (heap_size=9)

       [  1,  7,  9,  2,  5,  6,  6,  1 | 45, 45, 96 ]  // extract: swap(0,8)
       [  9,  7,  1,  2,  5,  6,  6,  1 | 45, 45, 96 ]  // siftDown: swap(0,2)
       [  9,  7,  6,  2,  5,  6,  1,  1 | 45, 45, 96 ]  // siftDown: swap(2,6)
       [  9,  7,  6,  2,  5,  6,  1,  1 | 45, 45, 96 ]  // heap restored (heap_size=8)

       [  1,  7,  6,  2,  5,  6,  1 |  9, 45, 45, 96 ]  // extract: swap(0,7)
       [  7,  1,  6,  2,  5,  6,  1 |  9, 45, 45, 96 ]  // siftDown: swap(0,1)
       [  7,  5,  6,  2,  1,  6,  1 |  9, 45, 45, 96 ]  // siftDown: swap(1,4)
       [  7,  5,  6,  2,  1,  6,  1 |  9, 45, 45, 96 ]  // heap restored (heap_size=7)

       [  1,  5,  6,  2,  1,  6 |  7,  9, 45, 45, 96 ]  // extract: swap(0,6)
       [  6,  5,  1,  2,  1,  6 |  7,  9, 45, 45, 96 ]  // siftDown: swap(0,2)
       [  6,  5,  6,  2,  1,  1 |  7,  9, 45, 45, 96 ]  // siftDown: swap(2,5)
       [  6,  5,  6,  2,  1,  1 |  7,  9, 45, 45, 96 ]  // heap restored (heap_size=6)

       [  1,  5,  6,  2,  1 |  6,  7,  9, 45, 45, 96 ]  // extract: swap(0,5)
       [  6,  5,  1,  2,  1 |  6,  7,  9, 45, 45, 96 ]  // siftDown: swap(0,2)
       [  6,  5,  1,  2,  1 |  6,  7,  9, 45, 45, 96 ]  // heap restored (heap_size=5)

       [  1,  5,  1,  2 |  6,  6,  7,  9, 45, 45, 96 ]  // extract: swap(0,4)
       [  5,  1,  1,  2 |  6,  6,  7,  9, 45, 45, 96 ]  // siftDown: swap(0,1)
       [  5,  2,  1,  1 |  6,  6,  7,  9, 45, 45, 96 ]  // siftDown: swap(1,3)
       [  5,  2,  1,  1 |  6,  6,  7,  9, 45, 45, 96 ]  // heap restored (heap_size=4)

       [  1,  2,  1 |  5,  6,  6,  7,  9, 45, 45, 96 ]  // extract: swap(0,3)
       [  2,  1,  1 |  5,  6,  6,  7,  9, 45, 45, 96 ]  // siftDown: swap(0,1)
       [  2,  1,  1 |  5,  6,  6,  7,  9, 45, 45, 96 ]  // heap restored (heap_size=3)

       [  1,  1 |  2,  5,  6,  6,  7,  9, 45, 45, 96 ]  // extract: swap(0,2)
       [  1,  1 |  2,  5,  6,  6,  7,  9, 45, 45, 96 ]  // heap restored (heap_size=2)

       [  1 |  1,  2,  5,  6,  6,  7,  9, 45, 45, 96 ]  // extract: swap(0,1)
       [  1 |  1,  2,  5,  6,  6,  7,  9, 45, 45, 96 ]  // heap restored (heap_size=1)

       [  1,  1,  2,  5,  6,  6,  7,  9, 45, 45, 96 ]  // sorted
    */

    /* Time complexity analysis:
         T_build(n) ≤ Σ_{h=0}^{⌊log2 n⌋} (n / 2^{h+1}) * O[h]
                  = O[ n * Σ_{h=0}^{⌊log2 n⌋} (h / 2^{h+1}) ] ; Σ h/2^{h+1} ---> cte ≤ 1
         T_build(n) = O[n]

         T_extract(n) = Σ_{k=2}^{n} ( O[1] + O[log k] )
                  = O[n] + O[ Σ_{k=2}^{n} log k ] ; Σ log k = log(n!) --> log(n!) ≤ n log n
                  = O[n] + O[ n log n ]
                  = O[ n log n ]

         T(n) = T_build(n) + T_extract(n)
              = O[n] + O[ n log n ]
              = O[n log n]
    */

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
