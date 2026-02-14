/// Comb Sort O(n²) worst case, O(n log n) average | LST

#include "../../../include/bds/array/bds_array_sort.h"
#include "../../../include/bds/array/bds_array_utils.h"

void arrayCombSort(Array *array, const key_val_func key) {
    // Comb sort is an improvement over bubble sort.
    // Instead of comparing adjacent elements, it compares elements
    // separated by a gap that shrinks by a factor (typically 1.3)
    // until it becomes 1 (then it's essentially bubble sort).
    // The gap reduction eliminates small values at the end (turtles)
    // that slow down bubble sort.

    /*
    COMB-SORT(A, key)
        n ← length(A)
        if n < 2 then
            return

        gap ← n
        shrink ← 1.3
        swapped ← true

        while gap > 1 or swapped = true do
            gap ← ⌊gap / shrink⌋
            if gap < 1 then
                gap ← 1

            swapped ← false

            for i ← 0 to n − gap − 1 do
                if key(A[i]) > key(A[i + gap]) then
                    swap(A[i], A[i + gap])
                    swapped ← true
    */

    /*
          0    1    2    3    4    5    6    7    8    9   10
       -------------------------------------------------------
       [  7,   5,   6,   2,  45,   9,   6,  45,   1,   1,  96]

       // -- Pass 1: gap = 8 --
       [* 7,   5,   6,   2,  45,   9,   6,  45, * 1,   1,  96]
       [  1,   5,   6,   2,  45,   9,   6,  45,   7,   1,  96]
       [  1, * 5,   6,   2,  45,   9,   6,  45,   7, * 1,  96]
       [  1,   1,   6,   2,  45,   9,   6,  45,   7,   5,  96]
       [  1,   1, * 6,   2,  45,   9,   6,  45,   7,   5, *96]
       [  1,   1,   6,   2,  45,   9,   6,  45,   7,   5,  96]

       // -- Pass 2: gap = 6 --
       [* 1,   1,   6,   2,  45,   9, * 6,  45,   7,   5,  96]
       [  1,   1,   6,   2,  45,   9,   6,  45,   7,   5,  96]
       [  1, * 1,   6,   2,  45,   9,   6, *45,   7,   5,  96]
       [  1,   1,   6,   2,  45,   9,   6,  45,   7,   5,  96]
       [  1,   1, * 6,   2,  45,   9,   6,  45, * 7,   5,  96]
       [  1,   1,   6,   2,  45,   9,   6,  45,   7,   5,  96]
       [  1,   1,   6, * 2,  45,   9,   6,  45,   7, * 5,  96]
       [  1,   1,   6,   2,  45,   9,   6,  45,   7,   5,  96]
       [  1,   1,   6,   2, *45,   9,   6,  45,   7,   5, *96]
       [  1,   1,   6,   2,  45,   9,   6,  45,   7,   5,  96]

       // -- Pass 3: gap = 4 --
       [* 1,   1,   6,   2, *45,   9,   6,  45,   7,   5,  96]
       [  1,   1,   6,   2,  45,   9,   6,  45,   7,   5,  96]
       [  1, * 1,   6,   2,  45, * 9,   6,  45,   7,   5,  96]
       [  1,   1,   6,   2,  45,   9,   6,  45,   7,   5,  96]
       [  1,   1, * 6,   2,  45,   9, * 6,  45,   7,   5,  96]
       [  1,   1,   6,   2,  45,   9,   6,  45,   7,   5,  96]
       [  1,   1,   6, * 2,  45,   9,   6, *45,   7,   5,  96]
       [  1,   1,   6,   2,  45,   9,   6,  45,   7,   5,  96]
       [  1,   1,   6,   2, *45,   9,   6,  45, * 7,   5,  96]
       [  1,   1,   6,   2,   7,   9,   6,  45,  45,   5,  96]
       [  1,   1,   6,   2,   7, * 9,   6,  45,  45, * 5,  96]
       [  1,   1,   6,   2,   7,   5,   6,  45,  45,   9,  96]
       [  1,   1,   6,   2,   7,   5, * 6,  45,  45,   9, *96]
       [  1,   1,   6,   2,   7,   5,   6,  45,  45,   9,  96]

       // -- Pass 4: gap = 3 --
       [* 1,   1,   6, * 2,   7,   5,   6,  45,  45,   9,  96]
       [  1,   1,   6,   2,   7,   5,   6,  45,  45,   9,  96]
       [  1, * 1,   6,   2, * 7,   5,   6,  45,  45,   9,  96]
       [  1,   1,   6,   2,   7,   5,   6,  45,  45,   9,  96]
       [  1,   1, * 6,   2,   7, * 5,   6,  45,  45,   9,  96]
       [  1,   1,   5,   2,   7,   6,   6,  45,  45,   9,  96]
       [  1,   1,   5, * 2,   7,   6, * 6,  45,  45,   9,  96]
       [  1,   1,   5,   2,   7,   6,   6,  45,  45,   9,  96]
       [  1,   1,   5,   2, * 7,   6,   6, *45,  45,   9,  96]
       [  1,   1,   5,   2,   7,   6,   6,  45,  45,   9,  96]
       [  1,   1,   5,   2,   7, * 6,   6,  45, *45,   9,  96]
       [  1,   1,   5,   2,   7,   6,   6,  45,  45,   9,  96]
       [  1,   1,   5,   2,   7,   6, * 6,  45,  45, * 9,  96]
       [  1,   1,   5,   2,   7,   6,   6,  45,  45,   9,  96]
       [  1,   1,   5,   2,   7,   6,   6, *45,  45,   9, *96]
       [  1,   1,   5,   2,   7,   6,   6,  45,  45,   9,  96]

       // -- Pass 5: gap = 2 --
       [* 1,   1, * 5,   2,   7,   6,   6,  45,  45,   9,  96]
       [  1,   1,   5,   2,   7,   6,   6,  45,  45,   9,  96]
       [  1, * 1,   5, * 2,   7,   6,   6,  45,  45,   9,  96]
       [  1,   1,   5,   2,   7,   6,   6,  45,  45,   9,  96]
       [  1,   1, * 5,   2, * 7,   6,   6,  45,  45,   9,  96]
       [  1,   1,   5,   2,   7,   6,   6,  45,  45,   9,  96]
       [  1,   1,   5, * 2,   7, * 6,   6,  45,  45,   9,  96]
       [  1,   1,   5,   2,   7,   6,   6,  45,  45,   9,  96]
       [  1,   1,   5,   2, * 7,   6, * 6,  45,  45,   9,  96]
       [  1,   1,   5,   2,   6,   6,   7,  45,  45,   9,  96]
       [  1,   1,   5,   2,   6, * 6,   7, *45,  45,   9,  96]
       [  1,   1,   5,   2,   6,   6,   7,  45,  45,   9,  96]
       [  1,   1,   5,   2,   6,   6, * 7,  45, *45,   9,  96]
       [  1,   1,   5,   2,   6,   6,   7,  45,  45,   9,  96]
       [  1,   1,   5,   2,   6,   6,   7, *45,  45, * 9,  96]
       [  1,   1,   5,   2,   6,   6,   7,   9,  45,  45,  96]
       [  1,   1,   5,   2,   6,   6,   7,   9, *45,  45, *96]
       [  1,   1,   5,   2,   6,   6,   7,   9,  45,  45,  96]

       // -- Pass 6: gap = 1 (standard bubble sort) --
       [* 1, * 1,   5,   2,   6,   6,   7,   9,  45,  45,  96]
       [  1,   1,   5,   2,   6,   6,   7,   9,  45,  45,  96]
       [  1, * 1, * 5,   2,   6,   6,   7,   9,  45,  45,  96]
       [  1,   1,   5,   2,   6,   6,   7,   9,  45,  45,  96]
       [  1,   1, * 5, * 2,   6,   6,   7,   9,  45,  45,  96]
       [  1,   1,   2,   5,   6,   6,   7,   9,  45,  45,  96]
       [  1,   1,   2, * 5, * 6,   6,   7,   9,  45,  45,  96]
       [  1,   1,   2,   5,   6,   6,   7,   9,  45,  45,  96]
       [  1,   1,   2,   5, * 6, * 6,   7,   9,  45,  45,  96]
       [  1,   1,   2,   5,   6,   6,   7,   9,  45,  45,  96]
       [  1,   1,   2,   5,   6, * 6, * 7,   9,  45,  45,  96]
       [  1,   1,   2,   5,   6,   6,   7,   9,  45,  45,  96]
       [  1,   1,   2,   5,   6,   6, * 7, * 9,  45,  45,  96]
       [  1,   1,   2,   5,   6,   6,   7,   9,  45,  45,  96]
       [  1,   1,   2,   5,   6,   6,   7, * 9, *45,  45,  96]
       [  1,   1,   2,   5,   6,   6,   7,   9,  45,  45,  96]
       [  1,   1,   2,   5,   6,   6,   7,   9, *45, *45,  96]
       [  1,   1,   2,   5,   6,   6,   7,   9,  45,  45,  96]
       [  1,   1,   2,   5,   6,   6,   7,   9,  45, *45, *96]
       [  1,   1,   2,   5,   6,   6,   7,   9,  45,  45,  96]
    */

    /* Time Complexity Analysis:
       The gap starts at n and shrinks by a factor of 1.3 each iteration.

       Number of iterations for gap reduction:
         n, n/1.3, n/1.3², ..., 1
         ≈ log₁.₃(n) iterations

       For each gap value g, we perform O(n) comparisons.

       Best case (already sorted):
         T(n) = O(n log n)

       Average case:
         T(n) = O(n²/2^p) where p depends on the gap sequence
         With shrink factor 1.3: T(n) ≈ O(n log n)

       Worst case (reverse sorted):
         T(n) = O(n²)

       𝒪[T(n)]
        = 𝒪[n²]   (worst case)
        = O(n log n)   (average case with optimal shrink factor)
    */

    /* Additional Memory Analysis:
       m(n) = sizeof(gap) + sizeof(shrink) + sizeof(swapped) + sizeof(idx) + ...
        = c

       𝒪[m(n)]
        = 𝒪[c]
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

    // Shrink factor of 1.3 has been empirically found to be optimal
    const double shrink_factor = 1.3;
    size_t gap = length;
    bool swapped = true;

    while (gap > 1 || swapped) {
        // Update gap for next comb
        gap = (size_t)(gap / shrink_factor);
        if (gap < 1) gap = 1;

        swapped = false;

        // Compare elements with current gap
        for (size_t idx = 0; idx + gap < length; idx++) {
            const size_t this_idx = idx;
            const size_t gap_idx = idx + gap;

            const int this_val = key(arrayGet(array, this_idx));
            const int gap_val = key(arrayGet(array, gap_idx));

            if (this_val > gap_val) {
                arraySwap(array, this_idx, gap_idx);
                swapped = true;
            }
        }
    }
}

Array *arrayCombSorted(const Array *array, key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    arrayCombSort(sorted_array, key);

    return sorted_array;
}