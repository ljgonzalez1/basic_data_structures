/// Intro Sort O(n log n) | ARR

#include "../../../include/bds/array/bds_array_utils.h"
#include "../../../include/bds/array/bds_array_sort.h"

/**
 * IntroSort ~= QuickSort + HeapSort + InsertionSort
 */

#define INTRO_INSERTION_THRESHOLD 16

// ===============================================================
// Utility: integer log2 for size_t
// ===============================================================

/**
 * Returns floor(log2(n)) for n >= 1.
 * For n = 0, returns 0 (not used in this context).
 */
static size_t introLog2Size(size_t n) {
    size_t result = 0;

    while (n > 1) {
        n >>= 1;
        result++;
    }

    return result;
}

// ===============================================================
// Insertion Sort (for tiny partitions)
// ===============================================================

/**
 * Insertion sort over [lo, hi).
 * Stable and efficient for small, almost-sorted ranges.
 */
static void introInsertionSort(
    Array *array,
    const size_t lo,
    const size_t hi,
    const key_val_func key
) {
    if (hi - lo < 2) return;

    for (size_t i = lo + 1; i < hi; i++) {
        void *pivot = arrayGet(array, i);
        size_t j = i;

        // Shift elements greater than pivot to the right
        while (j > lo &&
               arrayKeyCompare(arrayGet(array, j - 1), pivot, key) > 0) {

            arraySet(array, j, arrayGet(array, j - 1));
            j--;
        }

        arraySet(array, j, pivot);
    }
}

// ===============================================================
// HeapSort fallback (max-heap) for depth limit
// ===============================================================

/**
 * Sifts a node down the heap to restore the max-heap property.
 * Heap range is [heap_lo, heap_hi).
 */
static void introHeapSiftDown(
    Array *array,
    size_t root,
    const size_t heap_lo,
    const size_t heap_hi,
    const key_val_func key
) {
    while (1) {
        const size_t left  = heap_lo + ((root - heap_lo) * 2u + 1u);
        const size_t right = left + 1u;

        if (left >= heap_hi) break;  // no children

        size_t largest = root;

        if (arrayKeyCompare(arrayGet(array, left), arrayGet(array, largest), key) > 0) {
            largest = left;
        }

        if (right < heap_hi &&
            arrayKeyCompare(arrayGet(array, right), arrayGet(array, largest), key) > 0) {
            largest = right;
        }

        if (largest == root) break;

        arraySwap(array, root, largest);
        root = largest;
    }
}

/**
 * HeapSort over [lo, hi).
 * Used only as a safe fallback when QuickSort recursion is too deep.
 */
static void introHeapSortRange(
    Array *array,
    const size_t lo,
    const size_t hi,
    const key_val_func key
) {
    const size_t length = hi - lo;
    if (length < 2) return;

    // Build max-heap in [lo, hi)
    // Last parent = lo + (length / 2) - 1
    for (size_t i = lo + (length >> 1); i-- > lo; ) {
        introHeapSiftDown(array, i, lo, hi, key);
    }

    // Extract max repeatedly and shrink heap
    for (size_t end = hi; end-- > lo + 1; ) {
        arraySwap(array, lo, end);
        introHeapSiftDown(array, lo, lo, end, key);
    }
}

// ===============================================================
// QuickSort partition (median-of-three)
// ===============================================================

/**
 * Partitions [lo, hi) around a median-of-three pivot.
 * Returns the final pivot index in [lo, hi).
 */
static size_t introPartition(
    Array *array,
    const size_t lo,
    const size_t hi,
    const key_val_func key
) {
    const size_t mid = lo + ((hi - lo) >> 1);
    const size_t hi_1 = hi - 1;

    // Median-of-three on (lo, mid, hi-1)
    if (arrayKeyCompare(arrayGet(array, mid), arrayGet(array, lo), key) < 0)
        arraySwap(array, lo, mid);

    if (arrayKeyCompare(arrayGet(array, hi_1), arrayGet(array, lo), key) < 0)
        arraySwap(array, lo, hi_1);

    if (arrayKeyCompare(arrayGet(array, hi_1), arrayGet(array, mid), key) < 0)
        arraySwap(array, mid, hi_1);

    // Use mid as pivot; move it near the end (hi-2)
    const size_t pivot_idx_tmp = hi_1 - 1;
    arraySwap(array, mid, pivot_idx_tmp);

    const void *pivot = arrayGet(array, pivot_idx_tmp);

    size_t i = lo;
    size_t j = pivot_idx_tmp;

    while (1) {
        while (i < pivot_idx_tmp &&
               arrayKeyCompare(arrayGet(array, i), pivot, key) < 0) {
            i++;
        }

        while (j > lo &&
               arrayKeyCompare(arrayGet(array, j), pivot, key) > 0) {
            j--;
        }

        if (i >= j) break;

        arraySwap(array, i, j);
        i++;
        j--;
    }

    // Place pivot in its final position
    arraySwap(array, i, pivot_idx_tmp);
    return i;
}

// ===============================================================
// Recursive IntroSort engine
// ===============================================================

/**
 * Recursive IntroSort over [lo, hi).
 * Uses QuickSort until depth_limit reaches 0, then calls HeapSort.
 */
static void introSortRecursive(
    Array *array,
    size_t lo,
    size_t hi,
    size_t depth_limit,
    const key_val_func key
) {
    while (hi - lo > INTRO_INSERTION_THRESHOLD) {
        if (depth_limit == 0) {
            // Fallback to heapsort on this range
            introHeapSortRange(array, lo, hi, key);
            return;
        }

        depth_limit--;

        const size_t pivot_idx = introPartition(array, lo, hi, key);

        // Tail-recursion optimization: recurse on smaller half first
        // and loop on the larger half.
        if (pivot_idx - lo < hi - (pivot_idx + 1)) {
            // Left side smaller
            introSortRecursive(array, lo, pivot_idx, depth_limit, key);
            // Continue on right side
            // lo = pivot_idx + 1;
            // hi stays the same
            lo = pivot_idx + 1;
        } else {
            // Right side smaller
            introSortRecursive(array, pivot_idx + 1, hi, depth_limit, key);
            // Continue on left side
            // hi = pivot_idx;
            // lo stays the same
            hi = pivot_idx;
        }
    }

    // Small range → insertion sort
    introInsertionSort(array, lo, hi, key);
}

// ===============================================================
// Public API
// ===============================================================

void arrayIntroSort(
    /*
    INTRO-SORT(A, key)
        n ← length(A)
        if n < 2 then
            return

        depth_limit ← 2 * ⌊log2(n)⌋
        INTRO-SORT-REC(A, 0, n, depth_limit, key)

    INTRO-SORT-REC(A, lo, hi, depth_limit, key)
        // sorts A[lo..hi) (hi is exclusive)
        while (hi − lo) > INTRO_INSERTION_THRESHOLD do
            if depth_limit = 0 then
                HEAP-SORT-RANGE(A, lo, hi, key)     // safe fallback
                return

            depth_limit ← depth_limit − 1

            p ← PARTITION-MEDIAN3(A, lo, hi, key)

            // Tail recursion optimization:
            // recurse into smaller side, iterate on larger side
            if (p − lo) < (hi − (p + 1)) then
                INTRO-SORT-REC(A, lo, p, depth_limit, key)
                lo ← p + 1
            else
                INTRO-SORT-REC(A, p + 1, hi, depth_limit, key)
                hi ← p

        // small range → insertion sort
        INSERTION-SORT-RANGE(A, lo, hi, key)

    LOG2-SIZE(n)
        // returns ⌊log2(n)⌋ for n ≥ 1
        r ← 0
        while n > 1 do
            n ← ⌊n/2⌋
            r ← r + 1
        return r

    INSERTION-SORT-RANGE(A, lo, hi, key)
        for i ← lo + 1 to hi − 1 do
            x ← A[i]
            j ← i
            while j > lo and key(A[j − 1]) > key(x) do
                A[j] ← A[j − 1]
                j ← j − 1
            A[j] ← x

    PARTITION-MEDIAN3(A, lo, hi, key)
        mid ← lo + ⌊(hi − lo)/2⌋
        last ← hi − 1

        // median-of-three ordering of (lo, mid, last)
        if key(A[mid]) < key(A[lo]) then swap(A[lo], A[mid])
        if key(A[last]) < key(A[lo]) then swap(A[lo], A[last])
        if key(A[last]) < key(A[mid]) then swap(A[mid], A[last])

        // move pivot near end, then two-pointer partition
        pivot_tmp ← last − 1
        swap(A[mid], A[pivot_tmp])
        pivot ← A[pivot_tmp]

        i ← lo
        j ← pivot_tmp

        while true do
            while i < pivot_tmp and key(A[i]) < key(pivot) do
                i ← i + 1

            while j > lo and key(A[j]) > key(pivot) do
                j ← j − 1

            if i ≥ j then
                break

            swap(A[i], A[j])
            i ← i + 1
            j ← j − 1

        swap(A[i], A[pivot_tmp])     // pivot to final position
        return i

    HEAP-SORT-RANGE(A, lo, hi, key)
        len ← hi − lo
        if len < 2 then
            return

        // Build max-heap in [lo, hi)
        for i ← lo + ⌊len/2⌋ − 1 downto lo do
            HEAP-SIFT-DOWN(A, i, lo, hi, key)

        // Extract max repeatedly
        for end ← hi − 1 downto lo + 1 do
            swap(A[lo], A[end])
            HEAP-SIFT-DOWN(A, lo, lo, end, key)

    HEAP-SIFT-DOWN(A, root, heap_lo, heap_hi, key)
        while true do
            left  ← heap_lo + 2*(root − heap_lo) + 1
            right ← left + 1

            if left ≥ heap_hi then
                break

            largest ← root
            if key(A[left]) > key(A[largest]) then
                largest ← left
            if right < heap_hi and key(A[right]) > key(A[largest]) then
                largest ← right

            if largest = root then
                break

            swap(A[root], A[largest])
            root ← largest
    */

    /* Time Complexity Analysis:
       Let n = length(A).

       IntroSort combines:
         - QuickSort (fast average) with median-of-three pivots,
         - a recursion depth limit (≈ 2*log2(n)),
         - and HeapSort fallback to guarantee worst-case bounds,
         - plus insertion sort for small partitions.

       Average-case:
         QuickSort dominates with good pivots:
           T_avg(n) = Θ(n log n)

         𝒪[T_avg(n)]
          = 𝒪[n log n]

       Worst-case:
         Even if partitions are consistently bad, the depth limit forces a switch
         to HeapSort on the problematic range, yielding:
           T_worst(n) = O(n log n)

         𝒪[T_worst(n)]
          = 𝒪[n log n]

       Best-case:
         Balanced partitions + linear partition work per level:
           T_best(n) = Θ(n log n)

       The insertion-sort threshold improves constants (tiny ranges) but does not
       change the asymptotic bounds.
    */

    /* Additional Memory Analysis:
       m(n) = log n

       - The algorithm is in-place for the array contents.
       - It uses recursion for QuickSort partitions, but with:
           * depth limit = O(log n)
           * plus tail-recursion optimization (recurse on smaller side first)
         therefore the maximum call stack depth remains O(log n).

       HeapSort fallback is iterative over the range (no extra array).
       Insertion sort uses constant locals.

       𝒪[m(n)]
        = 𝒪[log n]
    */

    /* Total Memory Analysis:
       M(n) = n + m(n)
        = n + log n

       𝒪[M(n)]
        = 𝒪[n + log n]
        = 𝒪[n]
    */


    Array *array,
    const key_val_func key
) {
    const size_t length = arrayLength(array);
    if (length < 2) return;

    // Depth limit ~ 2 * floor(log2(n))
    const size_t depth_limit = 2u * introLog2Size(length);

    introSortRecursive(array, 0, length, depth_limit, key);
}

Array *arrayIntroSorted(
    const Array *array,
    const key_val_func key
) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    arrayIntroSort(sorted_array, key);
    return sorted_array;
}
