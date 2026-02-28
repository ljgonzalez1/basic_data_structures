/// Odd-Even Sort (Brick Sort) O(n²) | LST+ARR

#include "../../../include/bds/array/bds_array_sort.h"
#include "../../../include/bds/array/bds_array_utils.h"

void arrayOddEvenSort(Array *array, const key_val_func key) {
    // Odd-Even Sort (also known as Brick Sort) is a variation of bubble sort.
    // It alternates between two phases:
    //   1. Compare all odd/even indexed pairs (1-2, 3-4, 5-6, ...)
    //   2. Compare all even/odd indexed pairs (0-1, 2-3, 4-5, ...)
    // This continues until no swaps are made in a complete pass.
    // Useful for parallel implementations as all comparisons in a phase
    // are independent.

    /*
    ODD-EVEN-SORT(A, key)
        n ← length(A)
        if n < 2 then
            return

        swapped ← true

        while swapped = true do
            swapped ← false

            // Odd phase: compare (A[1], A[2]), (A[3], A[4]), ...
            for i ← 1 to n − 2 step 2 do
                if key(A[i]) > key(A[i + 1]) then
                    swap(A[i], A[i + 1])
                    swapped ← true

            // Even phase: compare (A[0], A[1]), (A[2], A[3]), ...
            for i ← 0 to n − 2 step 2 do
                if key(A[i]) > key(A[i + 1]) then
                    swap(A[i], A[i + 1])
                    swapped ← true
    */

    /*
          0    1    2    3    4    5    6    7    8    9   10
       -------------------------------------------------------
       [  7,   5,   6,   2,  45,   9,   6,  45,   1,   1,  96]

       // -- Iteration 1 (Odd phase) --
       [  7, * 5, * 6,   2,  45,   9,   6,  45,   1,   1,  96]
       [  7,   5,   6,   2,  45,   9,   6,  45,   1,   1,  96]
       [  7,   5,   6, * 2, *45,   9,   6,  45,   1,   1,  96]
       [  7,   5,   6,   2,  45,   9,   6,  45,   1,   1,  96]
       [  7,   5,   6,   2,  45, * 9, * 6,  45,   1,   1,  96]
       [  7,   5,   6,   2,  45,   6,   9,  45,   1,   1,  96]
       [  7,   5,   6,   2,  45,   6,   9, *45, * 1,   1,  96]
       [  7,   5,   6,   2,  45,   6,   9,   1,  45,   1,  96]
       [  7,   5,   6,   2,  45,   6,   9,   1,  45, * 1, *96]
       [  7,   5,   6,   2,  45,   6,   9,   1,  45,   1,  96]

       // -- Iteration 1 (Even phase) --
       [* 7, * 5,   6,   2,  45,   6,   9,   1,  45,   1,  96]
       [  5,   7,   6,   2,  45,   6,   9,   1,  45,   1,  96]
       [  5,   7, * 6, * 2,  45,   6,   9,   1,  45,   1,  96]
       [  5,   7,   2,   6,  45,   6,   9,   1,  45,   1,  96]
       [  5,   7,   2,   6, *45, * 6,   9,   1,  45,   1,  96]
       [  5,   7,   2,   6,   6,  45,   9,   1,  45,   1,  96]
       [  5,   7,   2,   6,   6,  45, * 9, * 1,  45,   1,  96]
       [  5,   7,   2,   6,   6,  45,   1,   9,  45,   1,  96]
       [  5,   7,   2,   6,   6,  45,   1,   9, *45, * 1,  96]
       [  5,   7,   2,   6,   6,  45,   1,   9,   1,  45,  96]

       // -- Iteration 2 (Odd phase) --
       [  5, * 7, * 2,   6,   6,  45,   1,   9,   1,  45,  96]
       [  5,   2,   7,   6,   6,  45,   1,   9,   1,  45,  96]
       [  5,   2,   7, * 6, * 6,  45,   1,   9,   1,  45,  96]
       [  5,   2,   7,   6,   6,  45,   1,   9,   1,  45,  96]
       [  5,   2,   7,   6,   6, *45, * 1,   9,   1,  45,  96]
       [  5,   2,   7,   6,   6,   1,  45,   9,   1,  45,  96]
       [  5,   2,   7,   6,   6,   1,  45, * 9, * 1,  45,  96]
       [  5,   2,   7,   6,   6,   1,  45,   1,   9,  45,  96]
       [  5,   2,   7,   6,   6,   1,  45,   1,   9, *45, *96]
       [  5,   2,   7,   6,   6,   1,  45,   1,   9,  45,  96]

       // -- Iteration 2 (Even phase) --
       [* 5, * 2,   7,   6,   6,   1,  45,   1,   9,  45,  96]
       [  2,   5,   7,   6,   6,   1,  45,   1,   9,  45,  96]
       [  2,   5, * 7, * 6,   6,   1,  45,   1,   9,  45,  96]
       [  2,   5,   6,   7,   6,   1,  45,   1,   9,  45,  96]
       [  2,   5,   6,   7, * 6, * 1,  45,   1,   9,  45,  96]
       [  2,   5,   6,   7,   1,   6,  45,   1,   9,  45,  96]
       [  2,   5,   6,   7,   1,   6, *45, * 1,   9,  45,  96]
       [  2,   5,   6,   7,   1,   6,   1,  45,   9,  45,  96]
       [  2,   5,   6,   7,   1,   6,   1,  45, * 9, *45,  96]
       [  2,   5,   6,   7,   1,   6,   1,  45,   9,  45,  96]

       // -- Iteration 3 (Odd phase) --
       [  2, * 5, * 6,   7,   1,   6,   1,  45,   9,  45,  96]
       [  2,   5,   6,   7,   1,   6,   1,  45,   9,  45,  96]
       [  2,   5,   6, * 7, * 1,   6,   1,  45,   9,  45,  96]
       [  2,   5,   6,   1,   7,   6,   1,  45,   9,  45,  96]
       [  2,   5,   6,   1,   7, * 6, * 1,  45,   9,  45,  96]
       [  2,   5,   6,   1,   7,   1,   6,  45,   9,  45,  96]
       [  2,   5,   6,   1,   7,   1,   6, *45, * 9,  45,  96]
       [  2,   5,   6,   1,   7,   1,   6,   9,  45,  45,  96]
       [  2,   5,   6,   1,   7,   1,   6,   9,  45, *45, *96]
       [  2,   5,   6,   1,   7,   1,   6,   9,  45,  45,  96]

       // -- Iteration 3 (Even phase) --
       [* 2, * 5,   6,   1,   7,   1,   6,   9,  45,  45,  96]
       [  2,   5,   6,   1,   7,   1,   6,   9,  45,  45,  96]
       [  2,   5, * 6, * 1,   7,   1,   6,   9,  45,  45,  96]
       [  2,   5,   1,   6,   7,   1,   6,   9,  45,  45,  96]
       [  2,   5,   1,   6, * 7, * 1,   6,   9,  45,  45,  96]
       [  2,   5,   1,   6,   1,   7,   6,   9,  45,  45,  96]
       [  2,   5,   1,   6,   1,   7, * 6, * 9,  45,  45,  96]
       [  2,   5,   1,   6,   1,   7,   6,   9,  45,  45,  96]
       [  2,   5,   1,   6,   1,   7,   6,   9, *45, *45,  96]
       [  2,   5,   1,   6,   1,   7,   6,   9,  45,  45,  96]

       // -- Iteration 4 (Odd phase) --
       [  2, * 5, * 1,   6,   1,   7,   6,   9,  45,  45,  96]
       [  2,   1,   5,   6,   1,   7,   6,   9,  45,  45,  96]
       [  2,   1,   5, * 6, * 1,   7,   6,   9,  45,  45,  96]
       [  2,   1,   5,   1,   6,   7,   6,   9,  45,  45,  96]
       [  2,   1,   5,   1,   6, * 7, * 6,   9,  45,  45,  96]
       [  2,   1,   5,   1,   6,   6,   7,   9,  45,  45,  96]
       [  2,   1,   5,   1,   6,   6,   7, * 9, *45,  45,  96]
       [  2,   1,   5,   1,   6,   6,   7,   9,  45,  45,  96]
       [  2,   1,   5,   1,   6,   6,   7,   9,  45, *45, *96]
       [  2,   1,   5,   1,   6,   6,   7,   9,  45,  45,  96]

       // -- Iteration 4 (Even phase) --
       [* 2, * 1,   5,   1,   6,   6,   7,   9,  45,  45,  96]
       [  1,   2,   5,   1,   6,   6,   7,   9,  45,  45,  96]
       [  1,   2, * 5, * 1,   6,   6,   7,   9,  45,  45,  96]
       [  1,   2,   1,   5,   6,   6,   7,   9,  45,  45,  96]
       [  1,   2,   1,   5, * 6, * 6,   7,   9,  45,  45,  96]
       [  1,   2,   1,   5,   6,   6,   7,   9,  45,  45,  96]
       [  1,   2,   1,   5,   6,   6, * 7, * 9,  45,  45,  96]
       [  1,   2,   1,   5,   6,   6,   7,   9,  45,  45,  96]
       [  1,   2,   1,   5,   6,   6,   7,   9, *45, *45,  96]
       [  1,   2,   1,   5,   6,   6,   7,   9,  45,  45,  96]

       // -- Iteration 5 (Odd phase) --
       [  1, * 2, * 1,   5,   6,   6,   7,   9,  45,  45,  96]
       [  1,   1,   2,   5,   6,   6,   7,   9,  45,  45,  96]
       [  1,   1,   2, * 5, * 6,   6,   7,   9,  45,  45,  96]
       [  1,   1,   2,   5,   6,   6,   7,   9,  45,  45,  96]
       [  1,   1,   2,   5,   6, * 6, * 7,   9,  45,  45,  96]
       [  1,   1,   2,   5,   6,   6,   7,   9,  45,  45,  96]
       [  1,   1,   2,   5,   6,   6,   7, * 9, *45,  45,  96]
       [  1,   1,   2,   5,   6,   6,   7,   9,  45,  45,  96]
       [  1,   1,   2,   5,   6,   6,   7,   9,  45, *45, *96]
       [  1,   1,   2,   5,   6,   6,   7,   9,  45,  45,  96]

       // -- Iteration 5 (Even phase) --
       [* 1, * 1,   2,   5,   6,   6,   7,   9,  45,  45,  96]
       [  1,   1,   2,   5,   6,   6,   7,   9,  45,  45,  96]
       [  1,   1, * 2, * 5,   6,   6,   7,   9,  45,  45,  96]
       [  1,   1,   2,   5,   6,   6,   7,   9,  45,  45,  96]
       [  1,   1,   2,   5, * 6, * 6,   7,   9,  45,  45,  96]
       [  1,   1,   2,   5,   6,   6,   7,   9,  45,  45,  96]
       [  1,   1,   2,   5,   6,   6, * 7, * 9,  45,  45,  96]
       [  1,   1,   2,   5,   6,   6,   7,   9,  45,  45,  96]
       [  1,   1,   2,   5,   6,   6,   7,   9, *45, *45,  96]
       [  1,   1,   2,   5,   6,   6,   7,   9,  45,  45,  96]

       // No swaps in last iteration → sorted
    */

    /* Time Complexity Analysis:
       In the worst case, we need n/2 iterations (each with odd and even phases).
       Each phase does O(n) comparisons.

       Worst case:
         T(n) = (n/2) * 2 * n
              = n²

       Best case (already sorted):
         T(n) = 2n (one complete pass with no swaps)
              = O(n)

       Average case:
         T(n) = O(n²)

       𝒪[T(n)]
        = 𝒪[n²]
    */

    /* Additional Memory Analysis:
       m(n) = sizeof(swapped) + sizeof(idx) + ...
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

    bool swapped = true;

    while (swapped) {
        swapped = false;

        // Odd phase: compare all (odd, even) index pairs
        for (size_t idx = 1; idx + 1 < length; idx += 2) {
            const size_t this_idx = idx;
            const size_t next_idx = idx + 1;

            const int this_val = key(arrayGet(array, this_idx));
            const int next_val = key(arrayGet(array, next_idx));

            if (this_val > next_val) {
                arraySwap(array, this_idx, next_idx);
                swapped = true;
            }
        }

        // Even phase: compare all (even, odd) index pairs
        for (size_t idx = 0; idx + 1 < length; idx += 2) {
            const size_t this_idx = idx;
            const size_t next_idx = idx + 1;

            const int this_val = key(arrayGet(array, this_idx));
            const int next_val = key(arrayGet(array, next_idx));

            if (this_val > next_val) {
                arraySwap(array, this_idx, next_idx);
                swapped = true;
            }
        }
    }
}

Array *arrayOddEvenSorted(const Array *array, key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    arrayOddEvenSort(sorted_array, key);

    return sorted_array;
}