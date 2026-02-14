/// Quick Sort O(n log n) on average, O(n²) worst case. M(log n)

#include "../../../include/bds/array/bds_array_utils.h"
#include "../../../include/bds/array/bds_array_sort.h"

/**
 *1. Choose a pivot element from the array.
 *  a. Either randomly
 *  b. or **median-of-three** (usually the best choice)
 *  c. or first/last element.
 *2. Move elements smaller than the pivot to its left,
 *   and elements larger than the pivot to its right.
 *3. Recursively apply the same process to the left and right sub-arrays.
 *
 * [  lower_than_pivot  ]  pivot  [  higher_than_pivot  ]
 */


/// >>> "divide et impera"


/// ===============================================================
/// QuickSort (average O(n log n), worst O(n²))
/// ===============================================================
///
/// Uses:
///   - Median-of-three pivot selection to reduce bad pivots.
///   - Lomuto-style partitioning on the range [lo, hi).
///   - Insertion sort for very small partitions.
///
/// Not stable. In-place. Average-case very fast.
/// ===============================================================

#define QUICK_INSERTION_THRESHOLD 16

/// ---------------------------------------------------------------
/// Insertion sort for small ranges [lo, hi)
/// ---------------------------------------------------------------

/**
 * Insertion sort over [lo, hi).
 * Stable and efficient for tiny partitions.
 */
static void quickInsertionSortRange(
    Array *array,
    const size_t lo,
    const size_t hi,
    const key_val_func key
) {
    if (hi - lo < 2) return;

    for (size_t i = lo + 1; i < hi; i++) {
        void  *pivot     = arrayGet(array, i);
        size_t scan_idx  = i;

        // Shift larger elements to the right
        while (scan_idx > lo &&
               arrayKeyCompare(arrayGet(array, scan_idx - 1), pivot, key) > 0) {

            arraySet(array, scan_idx, arrayGet(array, scan_idx - 1));
            scan_idx--;
        }

        arraySet(array, scan_idx, pivot);
    }
}

/// ---------------------------------------------------------------
/// Partition (Lomuto) with median-of-three pivot
/// ---------------------------------------------------------------

/**
 * Partitions the subarray [lo, hi) using a median-of-three pivot:
 *   - Choose pivot as median of (lo, mid, hi-1).
 *   - Move pivot to hi-1.
 *   - Partition elements so that:
 *       [lo, pivot_idx)   < pivot
 *       [pivot_idx]       == pivot
 *       (pivot_idx, hi)   >= pivot
 *
 * Returns: final pivot index in [lo, hi).
 */
static size_t quickPartition(
    Array *array,
    const size_t lo,
    const size_t hi,
    const key_val_func key
) {
    const size_t hi_1 = hi - 1;
    const size_t mid  = lo + ((hi - lo) >> 1);

    // Median-of-three on (lo, mid, hi-1)
    if (arrayKeyCompare(arrayGet(array, mid), arrayGet(array, lo), key) < 0)
        arraySwap(array, lo, mid);

    if (arrayKeyCompare(arrayGet(array, hi_1), arrayGet(array, lo), key) < 0)
        arraySwap(array, lo, hi_1);

    if (arrayKeyCompare(arrayGet(array, hi_1), arrayGet(array, mid), key) < 0)
        arraySwap(array, mid, hi_1);

    // Use mid as pivot; move it to hi-1 for Lomuto partitioning
    arraySwap(array, mid, hi_1);
    const void *pivot = arrayGet(array, hi_1);

    size_t store_idx = lo;

    // Lomuto partition: scan [lo, hi-1)
    for (size_t i = lo; i < hi_1; i++) {
        if (arrayKeyCompare(arrayGet(array, i), pivot, key) < 0) {
            arraySwap(array, i, store_idx);
            store_idx++;
        }
    }

    // Place pivot in its final position
    arraySwap(array, store_idx, hi_1);

    return store_idx;
}

/// ---------------------------------------------------------------
/// Recursive QuickSort over [lo, hi)
/// ---------------------------------------------------------------

/**
 * Recursive QuickSort engine on [lo, hi).
 * Uses insertion sort for small ranges to reduce overhead.
 */
static void quickSortRecursive(
    Array *array,
    const size_t lo,
    const size_t hi,
    const key_val_func key
) {
    // Base case: empty or single element
    if (hi - lo < 2) return;

    // For small ranges, use insertion sort
    if (hi - lo <= QUICK_INSERTION_THRESHOLD) {
        quickInsertionSortRange(array, lo, hi, key);
        return;
    }

    // Partition around pivot
    const size_t pivot_idx = quickPartition(array, lo, hi, key);

    // Recursively sort partitions (excluding pivot)
    if (pivot_idx > lo)
        quickSortRecursive(array, lo, pivot_idx, key);

    if (pivot_idx + 1 < hi)
        quickSortRecursive(array, pivot_idx + 1, hi, key);
}

/// ---------------------------------------------------------------
/// Public API
/// ---------------------------------------------------------------

void arrayQuickSort(
    /*
    QUICK-SORT(A, key)
    n ← length(A)
    if n < 2 then
        return

    QUICK-SORT-REC(A, 0, n, key)

    QUICK-SORT-REC(A, lo, hi, key)
    // sorts A[lo..hi) (hi is exclusive)
    if hi − lo < 2 then
        return

    if hi − lo ≤ QUICK_INSERTION_THRESHOLD then
        INSERTION-SORT-RANGE(A, lo, hi, key)
        return

    p ← PARTITION-MEDIAN3-LOMUTO(A, lo, hi, key)

    if p > lo then
        QUICK-SORT-REC(A, lo, p, key)         // left partition [lo, p)

    if p + 1 < hi then
        QUICK-SORT-REC(A, p + 1, hi, key)     // right partition (p, hi)

    INSERTION-SORT-RANGE(A, lo, hi, key)
    // stable insertion sort over A[lo..hi)
    for i ← lo + 1 to hi − 1 do
        x ← A[i]
        j ← i

        while j > lo and key(A[j − 1]) > key(x) do
            A[j] ← A[j − 1]
            j ← j − 1

        A[j] ← x

    PARTITION-MEDIAN3-LOMUTO(A, lo, hi, key)
    // choose pivot by median-of-three: (lo, mid, hi-1)
    mid ← lo + ⌊(hi − lo)/2⌋
    last ← hi − 1

    if key(A[mid]) < key(A[lo]) then swap(A[lo], A[mid])
    if key(A[last]) < key(A[lo]) then swap(A[lo], A[last])
    if key(A[last]) < key(A[mid]) then swap(A[mid], A[last])

    // move chosen pivot to end (last) and run Lomuto on [lo, last)
    swap(A[mid], A[last])
    pivot ← A[last]

    store ← lo

    for i ← lo to last − 1 do
        if key(A[i]) < key(pivot) then
            swap(A[i], A[store])
            store ← store + 1

    swap(A[store], A[last])
    return store
    */

    /* Time Complexity Analysis:
       Let n = length(A).

       Partition step:
         PARTITION runs a single scan over the subarray ⇒ Θ(n) work per level.

       Average-case (with good pivots; median-of-three reduces bad pivots):
         Recurrence: T(n) = T(a n) + T((1-a) n) + Θ(n)
         with a roughly near 1/2 on average ⇒ T(n) = Θ(n log n)

         𝒪[T(n)] = 𝒪[n log n]

       Best-case (perfectly balanced partitions every time):
         T(n) = 2T(n/2) + Θ(n) = Θ(n log n)

       Worst-case (highly unbalanced partitions, e.g. 0 and n-1 repeatedly):
         T(n) = T(n − 1) + Θ(n) = Θ(n²)

         𝒪[T(n)] = 𝒪[n²]

       Note on the insertion-sort threshold:
         For partitions of size ≤ 16, the algorithm switches to insertion sort.
         This improves constants but does not change the asymptotic bounds.
    */

    /* Additional Memory Analysis:
       m(n) = log n   (average / best)
       m(n) = n       (worst)

       The algorithm is in-place for the array contents, but it uses recursion:
         - average/best recursion depth: Θ(log n)
         - worst-case recursion depth: Θ(n)

       𝒪[m(n)]
        = 𝒪[log n]   average/best
        = 𝒪[n]       worst
    */

    /* Total Memory Analysis:
       M(n) = n + m(n)

       Average / best:
         M(n) = n + log n
         𝒪[M(n)] = 𝒪[n]

       Worst:
         M(n) = n + n = 2n
         𝒪[M(n)] = 𝒪[n]
    */


    Array *array,
    const key_val_func key
) {
    const size_t length = arrayLength(array);
    if (length < 2) return;

    quickSortRecursive(array, 0, length, key);
}

Array *arrayQuickSorted(
    const Array *array,
    const key_val_func key
) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    arrayQuickSort(sorted_array, key);

    return sorted_array;
}
