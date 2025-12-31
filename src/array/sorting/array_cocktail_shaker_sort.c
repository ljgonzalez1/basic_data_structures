/// Cocktail Shaker Sort O(n²)

#include "../../../include/bds/array/bds_array_sort.h"
#include "../../../include/bds/array/bds_array_utils.h"

void arrayCocktailShakerSort(Array *array, const key_val_func key) {
    // Bubble sort to the right, then to the left, narrowing the bounds each time.
    // More efficient than bubble sort on average.
    // Though still O(n²) time complexity.

    /*
    COCKTAIL-SHAKER-SORT(A, key)
        n ← length(A)
        if n < 2 then
            return

        start ← 0
        end ← n − 1
        swapped ← true

        do
            // Forward pass (bubble max to the right)
            swapped ← false

            for i ← start to end − 1 do
                if key(A[i]) > key(A[i + 1]) then
                    swap(A[i], A[i + 1])
                    swapped ← true

            if swapped = false then
                break

            end ← end − 1          // rightmost is fixed

            // Backward pass (bubble min to the left)
            swapped ← false

            for i ← end down to start + 1 do
                if key(A[i − 1]) > key(A[i]) then
                    swap(A[i − 1], A[i])
                    swapped ← true

            start ← start + 1      // leftmost is fixed

        while swapped = true
    */

    /*
          0    1    2    3    4    5    6    7    8    9   10
       -------------------------------------------------------
       [  7,   5,   6,   2,  45,   9,   6,  45,   1,   1,  96]
       [* 5, * 7,   6,   2,  45,   9,   6,  45,   1,   1,  96] // -- Forward pass 1 --
       [  5, * 6, * 7,   2,  45,   9,   6,  45,   1,   1,  96]
       [  5,   6, * 2, * 7,  45,   9,   6,  45,   1,   1,  96]
       [  5,   6,   2, * 7, *45,   9,   6,  45,   1,   1,  96]
       [  5,   6,   2,   7, * 9, *45,   6,  45,   1,   1,  96]
       [  5,   6,   2,   7,   9, * 6, *45,  45,   1,   1,  96]
       [  5,   6,   2,   7,   9,   6, *45, *45,   1,   1,  96]
       [  5,   6,   2,   7,   9,   6,  45, * 1, *45,   1,  96]
       [  5,   6,   2,   7,   9,   6,  45,   1, * 1, *45,  96]
       [  5,   6,   2,   7,   9,   6,  45,   1,   1, *45, *96]
       [  5,   6,   2,   7,   9,   6,  45,   1, * 1, *45,  96] // -- Backward pass 1 --
       [  5,   6,   2,   7,   9,   6,  45, * 1, * 1,  45,  96]
       [  5,   6,   2,   7,   9,   6, * 1, *45,   1,  45,  96]
       [  5,   6,   2,   7,   9, * 1, * 6,  45,   1,  45,  96]
       [  5,   6,   2,   7, * 1, * 9,   6,  45,   1,  45,  96]
       [  5,   6,   2, * 1, * 7,   9,   6,  45,   1,  45,  96]
       [  5,   6, * 1, * 2,   7,   9,   6,  45,   1,  45,  96]
       [  5, * 1, * 6,   2,   7,   9,   6,  45,   1,  45,  96]
       [* 1, * 5,   6,   2,   7,   9,   6,  45,   1,  45,  96]
       [  1, * 5, * 6,   2,   7,   9,   6,  45,   1,  45,  96] // -- Forward pass 2 --
       [  1,   5, * 2, * 6,   7,   9,   6,  45,   1,  45,  96]
       [  1,   5,   2, * 6, * 7,   9,   6,  45,   1,  45,  96]
       [  1,   5,   2,   6, * 7, * 9,   6,  45,   1,  45,  96]
       [  1,   5,   2,   6,   7, * 6, * 9,  45,   1,  45,  96]
       [  1,   5,   2,   6,   7,   6, * 9, *45,   1,  45,  96]
       [  1,   5,   2,   6,   7,   6,   9, * 1, *45,  45,  96]
       [  1,   5,   2,   6,   7,   6,   9,   1, *45, *45,  96]
       [  1,   5,   2,   6,   7,   6,   9,   1, *45, *45,  96] // -- Backward pass 2 --
       [  1,   5,   2,   6,   7,   6, * 1, * 9,  45,  45,  96]
       [  1,   5,   2,   6,   7, * 1, * 6,   9,  45,  45,  96]
       [  1,   5,   2,   6, * 1, * 7,   6,   9,  45,  45,  96]
       [  1,   5,   2, * 1, * 6,   7,   6,   9,  45,  45,  96]
       [  1,   5, * 1, * 2,   6,   7,   6,   9,  45,  45,  96]
       [  1, * 1, * 5,   2,   6,   7,   6,   9,  45,  45,  96]
       [  1,   1, * 2, * 5,   6,   7,   6,   9,  45,  45,  96] // -- Forward pass 3 --
       [  1,   1,   2, * 5, * 6,   7,   6,   9,  45,  45,  96]
       [  1,   1,   2,   5, * 6, * 7,   6,   9,  45,  45,  96]
       [  1,   1,   2,   5,   6, * 6, * 7,   9,  45,  45,  96]
       [  1,   1,   2,   5,   6,   6, * 7, * 9,  45,  45,  96]
       [  1,   1,   2,   5,   6,   6, * 7, * 9,  45,  45,  96] // -- Backward pass 3 --
       [  1,   1,   2,   5,   6, * 6, * 7,   9,  45,  45,  96]
       [  1,   1,   2,   5, * 6, * 6,   7,   9,  45,  45,  96]
       [  1,   1,   2, * 5, * 6,   6,   7,   9,  45,  45,  96]
       [  1,   1, * 2, * 5,   6,   6,   7,   9,  45,  45,  96]
       [  1,   1,   2,   5,   6,   6,   7,   9,  45,  45,  96]
    */

    /*   Time Complexity Analysis:
       T(n)= Σ_{i=0}^{½n−1}{ 2(n − 2i − 1) }
         = 2 * Σ_{i=0}^{½n−1}{ n − 2i − 1 }
         = 2 * ( Σ_{i=0}^{½n−1}{ n } - 2 * Σ_{i=0}^{½n−1}{ i } - Σ_{i=0}^{½n−1}{ 1 } )
         = 2 * ( Σ_{i=0}^{m−1}{ n } - 2 * Σ_{i=0}^{m−1}{ i } - Σ_{i=0}^{m−1}{ 1 } ) ; m = ½n.
         = 2 * ( (n * m) - 2 * (m(m-1)/2) - m )
         = 2 * ( n m - m(m-1) - m )
         = 2 * ( n m - (m² - m) - m )
         = 2 * ( n m - m² )
         = 2 * ( n(n/2) - (n/2)² ) ; m = n/2:
         = 2 * ( n²/2 - n²/4 )
         = 2 * ( n²/4 )
         = n²/2

       𝒪[T(n)]
        = 𝒪[n²/2]
        = 𝒪[n²]
    */

    /* Additional Memory Analysis:
       m(n) = c

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

    size_t start = 0;
    size_t end = length - 1;
    bool swapped = true;

    do {
        // Bubble up to the right (forward pass; like bubble sort; highest to the right)
        swapped = false;

        for (size_t idx = start; idx < end; idx++) {
            const size_t this_idx = idx;
            const size_t next_idx = idx + 1;

            const int this_val = key(arrayGet(array, this_idx));
            const int next_val = key(arrayGet(array, next_idx));

            if (this_val > next_val) {
                arraySwap(array, this_idx, next_idx);
                swapped = true;
            }
        }

        // If no swap, the array is sorted
        if (!swapped) break;
        end--;  // Last element is in place

        // Bubble down to the left (backward pass; lowest to the left)
        swapped = false;

        for (size_t idx = end; idx > start; idx--) {
            const size_t this_idx = idx;
            const size_t prev_idx = idx - 1;

            const int this_val = key(arrayGet(array, this_idx));
            const int prev_val = key(arrayGet(array, prev_idx));

            if (prev_val > this_val) {
                arraySwap(array, prev_idx, this_idx);
                swapped = true;
            }
        }

        start++; // First element is in place
    } while (swapped);
}

Array *arrayCocktailShakerSorted(const Array *array, const key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    arrayCocktailShakerSort(sorted_array, key);

    return sorted_array;
}
