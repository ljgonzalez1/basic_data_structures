/// Shell Sort O(n log² n)

#include "../../../include/bds/array/bds_array_core.h"
#include "../../../include/bds/array/bds_array_sort.h"

// ===============================================================
// Shell Sort (Hibbard gaps: 1, 3, 7, 15...)
// ===============================================================
//
// Shell Sort improves Insertion Sort by allowing comparisons of
// elements far apart. It uses a gap sequence to partially sort
// the array multiple times, reducing the overall number of moves.
//
// Complexity:
//   - Best:    O(n log n)
//   - Average: O(n^(3/2))
//   - Worst:   depends on gap sequence (Hibbard: O(n^(3/2)))
//
// Properties:
//   - Not stable.
//   - In-place.
//   - Very fast in practice with a good gap sequence.
//
// ===============================================================

// ---------------------------------------------------------------
// Hibbard gap calculation: largest 2^k - 1 <= length
// ---------------------------------------------------------------
static size_t shellInitialGap(const size_t length) {
    size_t gap = 1;

    while ((gap << 1) + 1 < length) {
        gap = (gap << 1) + 1;   // 1, 3, 7, 15, 31, ...
    }

    return gap;
}

// ---------------------------------------------------------------
// Single "gapped" insertion sort pass
// ---------------------------------------------------------------
static void shellGappedInsertionSort(
    Array *array,
    const size_t length,
    const size_t gap,
    const key_val_func key
) {
    for (size_t i = gap; i < length; i++) {
        void *value = arrayGet(array, i);
        size_t j = i;

        // Standard insertion sort, but stepping by 'gap'
        while (j >= gap &&
               key(arrayGet(array, j - gap)) > key(value)) {

            arraySet(array, j, arrayGet(array, j - gap));
            j -= gap;
        }

        arraySet(array, j, value);
    }
}

// ---------------------------------------------------------------
// Public API: Shell Sort
// ---------------------------------------------------------------
void arrayShellSort(
    Array *array,
    const key_val_func key
) {
    const size_t length = arrayLength(array);
    if (length < 2) return;

    // Start with largest Hibbard gap
    for (size_t gap = shellInitialGap(length); gap > 0; gap >>= 1) {
        shellGappedInsertionSort(array, length, gap, key);
    }
}

Array *arrayShellSorted(
    const Array *array,
    key_val_func key
) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    arrayShellSort(sorted_array, key);
    return sorted_array;
}
