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
