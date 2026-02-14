/// Shell Sort O(n log² n)

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
    /*
    SHELL-SORT(A, key)
        n ← length(A)
        if n < 2 then
            return

        // Compute initial gap using Hibbard sequence: 2^k − 1
        gap ← 1
        while (2·gap + 1) < n do
            gap ← 2·gap + 1

        // Decrease gap and perform gapped insertion sort
        while gap > 0 do
            for i ← gap to n − 1 do
                temp ← A[i]
                j ← i

                while j ≥ gap and key(A[j − gap]) > key(temp) do
                    A[j] ← A[j − gap]
                    j ← j − gap

                A[j] ← temp

            gap ← gap / 2        // next smaller gap
    */

    /*
              0    1    2    3    4    5    6    7    8    9   10
    -------------------------------------------------------
    [   7,   5,   6,   2,  45,   9,   6,  45,   1,   1,  96 ]

    -- Gap pass: gap = 7 --
    [ * 7,   5,   6,   2,  45,   9,   6, *45,   1,   1,  96 ]  //// // gap = 7, i = 7, key = 45  // already in order
    [   7, * 5,   6,   2,  45,   9,   6,  45, * 1,   1,  96 ]  //// // gap = 7, i = 8, key = 1
    [   7,   5,   6,   2,  45,   9,   6,  45,   5,   1,  96 ]  // shift by gap (5 moved 1->8)
    [   7,   1,   6,   2,  45,   9,   6,  45,   5,   1,  96 ]  // insert key at index 1
    [   7,   1, * 6,   2,  45,   9,   6,  45,   5, * 1,  96 ]  //// // gap = 7, i = 9, key = 1
    [   7,   1,   6,   2,  45,   9,   6,  45,   5,   6,  96 ]  // shift by gap (6 moved 2->9)
    [   7,   1,   1,   2,  45,   9,   6,  45,   5,   6,  96 ]  // insert key at index 2
    [   7,   1,   1, * 2,  45,   9,   6,  45,   5,   6, *96 ]  //// // gap = 7, i = 10, key = 96  // already in order

    -- Gap pass: gap = 3 --
    [ * 7,   1,   1, * 2,  45,   9,   6,  45,   5,   6,  96 ]  //// // gap = 3, i = 3, key = 2
    [   7,   1,   1,   7,  45,   9,   6,  45,   5,   6,  96 ]  // shift by gap (7 moved 0->3)
    [   2,   1,   1,   7,  45,   9,   6,  45,   5,   6,  96 ]  // insert key at index 0
    [   2, * 1,   1,   7, *45,   9,   6,  45,   5,   6,  96 ]  //// // gap = 3, i = 4, key = 45  // already in order
    [   2,   1, * 1,   7,  45, * 9,   6,  45,   5,   6,  96 ]  //// // gap = 3, i = 5, key = 9  // already in order
    [   2,   1,   1, * 7,  45,   9, * 6,  45,   5,   6,  96 ]  //// // gap = 3, i = 6, key = 6
    [   2,   1,   1,   7,  45,   9,   7,  45,   5,   6,  96 ]  // shift by gap (7 moved 3->6)
    [   2,   1,   1,   6,  45,   9,   7,  45,   5,   6,  96 ]  // insert key at index 3
    [   2,   1,   1,   6, *45,   9,   7, *45,   5,   6,  96 ]  //// // gap = 3, i = 7, key = 45  // already in order
    [   2,   1,   1,   6,  45, * 9,   7,  45, * 5,   6,  96 ]  //// // gap = 3, i = 8, key = 5
    [   2,   1,   1,   6,  45,   9,   7,  45,   9,   6,  96 ]  // shift by gap (9 moved 5->8)
    [   2,   1,   1,   6,  45,   5,   7,  45,   9,   6,  96 ]  // insert key at index 5
    [   2,   1,   1,   6,  45,   5, * 7,  45,   9, * 6,  96 ]  //// // gap = 3, i = 9, key = 6
    [   2,   1,   1,   6,  45,   5,   7,  45,   9,   7,  96 ]  // shift by gap (7 moved 6->9)
    [   2,   1,   1,   6,  45,   5,   6,  45,   9,   7,  96 ]  // insert key at index 6
    [   2,   1,   1,   6,  45,   5,   6, *45,   9,   7, *96 ]  //// // gap = 3, i = 10, key = 96  // already in order

    -- Gap pass: gap = 1 --
    [ * 2, * 1,   1,   6,  45,   5,   6,  45,   9,   7,  96 ]  //// // gap = 1, i = 1, key = 1
    [   2,   2,   1,   6,  45,   5,   6,  45,   9,   7,  96 ]  // shift by gap (2 moved 0->1)
    [   1,   2,   1,   6,  45,   5,   6,  45,   9,   7,  96 ]  // insert key at index 0
    [   1, * 2, * 1,   6,  45,   5,   6,  45,   9,   7,  96 ]  //// // gap = 1, i = 2, key = 1
    [   1,   2,   2,   6,  45,   5,   6,  45,   9,   7,  96 ]  // shift by gap (2 moved 1->2)
    [   1,   1,   2,   6,  45,   5,   6,  45,   9,   7,  96 ]  // insert key at index 1
    [   1,   1, * 2, * 6,  45,   5,   6,  45,   9,   7,  96 ]  //// // gap = 1, i = 3, key = 6  // already in order
    [   1,   1,   2, * 6, *45,   5,   6,  45,   9,   7,  96 ]  //// // gap = 1, i = 4, key = 45  // already in order
    [   1,   1,   2,   6, *45, * 5,   6,  45,   9,   7,  96 ]  //// // gap = 1, i = 5, key = 5
    [   1,   1,   2,   6,  45,  45,   6,  45,   9,   7,  96 ]  // shift by gap (45 moved 4->5)
    [   1,   1,   2,   6,   6,  45,   6,  45,   9,   7,  96 ]  // shift by gap (6 moved 3->4)
    [   1,   1,   2,   5,   6,  45,   6,  45,   9,   7,  96 ]  // insert key at index 3
    [   1,   1,   2,   5,   6, *45, * 6,  45,   9,   7,  96 ]  //// // gap = 1, i = 6, key = 6
    [   1,   1,   2,   5,   6,  45,  45,  45,   9,   7,  96 ]  // shift by gap (45 moved 5->6)
    [   1,   1,   2,   5,   6,   6,  45,  45,   9,   7,  96 ]  // insert key at index 5
    [   1,   1,   2,   5,   6,   6, *45, *45,   9,   7,  96 ]  //// // gap = 1, i = 7, key = 45  // already in order
    [   1,   1,   2,   5,   6,   6,  45, *45, * 9,   7,  96 ]  //// // gap = 1, i = 8, key = 9
    [   1,   1,   2,   5,   6,   6,  45,  45,  45,   7,  96 ]  // shift by gap (45 moved 7->8)
    [   1,   1,   2,   5,   6,   6,  45,  45,  45,   7,  96 ]  // shift by gap (45 moved 6->7)
    [   1,   1,   2,   5,   6,   6,   9,  45,  45,   7,  96 ]  // insert key at index 6
    [   1,   1,   2,   5,   6,   6,   9,  45, *45, * 7,  96 ]  //// // gap = 1, i = 9, key = 7
    [   1,   1,   2,   5,   6,   6,   9,  45,  45,  45,  96 ]  // shift by gap (45 moved 8->9)
    [   1,   1,   2,   5,   6,   6,   9,  45,  45,  45,  96 ]  // shift by gap (45 moved 7->8)
    [   1,   1,   2,   5,   6,   6,   9,   9,  45,  45,  96 ]  // shift by gap (9 moved 6->7)
    [   1,   1,   2,   5,   6,   6,   7,   9,  45,  45,  96 ]  // insert key at index 6
    [   1,   1,   2,   5,   6,   6,   7,   9,  45, *45, *96 ]  //// // gap = 1, i = 10, key = 96  // already in order

    [   1,   1,   2,   5,   6,   6,   7,   9,  45,  45,  96 ]  // sorted
    */

    /* Time Complexity Analysis:
       Let n = length(A).

       Shell sort's time complexity depends on the chosen gap sequence.
       Here:
         - initial gap is Hibbard-like (2^k − 1),
         - but the update rule is gap >>= 1 (halving), which is not the standard
           Hibbard sequence (which would typically go ... 15, 7, 3, 1).

       Per-gap work:
         A gapped insertion sort pass is O(n²) in the worst case (like insertion sort)
         but typically much less for large gaps because elements move closer to
         their final positions.

       Worst-case bound (generic safe bound):
         T(n) = O(n²)

       Practical/typical behavior:
         Usually observed around O(n^(3/2)) to O(n^(4/3)) for common gap sequences,
         but the exact asymptotic bound depends on the exact gap sequence used.

       𝒪[T(n)]
        = 𝒪[n²]
    */


    /* Additional Memory Analysis:
       m(n) = c

       Shell sort is in-place; gapped insertion uses only a constant number of
       temporaries and indices.

       𝒪[m(n)]
        = 𝒪[1]
    */

    /* Total Memory Analysis:
       M(n) = n + m(n)
        = n + c

       𝒪[M(n)]
        = 𝒪[n + c]
        = 𝒪[n]
    */



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
