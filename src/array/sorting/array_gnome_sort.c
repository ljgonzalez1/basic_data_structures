/// Gnome Sort O(n²)

#include "../../../include/bds/array/bds_array_sort.h"
#include "../../../include/bds/array/bds_array_utils.h"

void arrayGnomeSort(Array *array, const key_val_func key) {
    // Similar to insertion sort
    // Simpler
    // If 2 elements are out of order, swap them and step back
    // Else, step forward
    // Repeat until the end of the array is reached

    /*
    GNOME-SORT(A, key)
        n ← length(A)
        if n < 2 then
            return

        i ← 0

        while i < n − 1 do
            curr ← key(A[i])
            next ← key(A[i + 1])

            if curr ≤ next then
                i ← i + 1                    // in order: step forward
            else
                swap(A[i], A[i + 1])         // out of order: swap

                if i > 0 then
                    i ← i − 1                // step back (keep bubbling left)
                else
                    i ← i + 1                // at start: can’t go back, step forward
    */

    /*
          0    1    2    3    4    5    6    7    8    9   10
       -------------------------------------------------------
       [   7,   5,   6,   2,  45,   9,   6,  45,   1,   1,  96]
       [ * 7, * 5,   6,   2,  45,   9,   6,  45,   1,   1,  96]
       [ > 5,   7,   6,   2,  45,   9,   6,  45,   1,   1,  96]  // swap, step back
       [ * 5, * 7,   6,   2,  45,   9,   6,  45,   1,   1,  96]
       [   5, * 7, * 6,   2,  45,   9,   6,  45,   1,   1,  96]
       [ > 5,   6,   7,   2,  45,   9,   6,  45,   1,   1,  96]  // swap, step back
       [ * 5, * 6,   7,   2,  45,   9,   6,  45,   1,   1,  96]
       [   5, * 6, * 7,   2,  45,   9,   6,  45,   1,   1,  96]
       [   5,   6, * 7, * 2,  45,   9,   6,  45,   1,   1,  96]
       [   5, > 6,   2,   7,  45,   9,   6,  45,   1,   1,  96]  // swap, step back
       [   5, * 6, * 2,   7,  45,   9,   6,  45,   1,   1,  96]
       [ > 5,   2,   6,   7,  45,   9,   6,  45,   1,   1,  96]  // swap, step back
       [ * 5, * 2,   6,   7,  45,   9,   6,  45,   1,   1,  96]
       [ > 2,   5,   6,   7,  45,   9,   6,  45,   1,   1,  96]  // swap, step back
       [ * 2, * 5,   6,   7,  45,   9,   6,  45,   1,   1,  96]
       [   2, * 5, * 6,   7,  45,   9,   6,  45,   1,   1,  96]
       [   2,   5, * 6, * 7,  45,   9,   6,  45,   1,   1,  96]
       [   2,   5,   6, * 7, *45,   9,   6,  45,   1,   1,  96]
       [   2,   5,   6,   7, *45, * 9,   6,  45,   1,   1,  96]
       [   2,   5,   6, > 7,   9,  45,   6,  45,   1,   1,  96]  // swap, step back
       [   2,   5,   6, * 7, * 9,  45,   6,  45,   1,   1,  96]
       [   2,   5,   6,   7, * 9, *45,   6,  45,   1,   1,  96]
       [   2,   5,   6,   7,   9, *45, * 6,  45,   1,   1,  96]
       [   2,   5,   6,   7, > 9,   6,  45,  45,   1,   1,  96]  // swap, step back
       [   2,   5,   6,   7, * 9, * 6,  45,  45,   1,   1,  96]
       [   2,   5,   6,   7, > 6,   9,  45,  45,   1,   1,  96]  // swap, step back
       [   2,   5,   6, * 7, * 6,   9,  45,  45,   1,   1,  96]
       [   2,   5, > 6,   6,   7,   9,  45,  45,   1,   1,  96]  // swap, step back
       [   2,   5, * 6, * 6,   7,   9,  45,  45,   1,   1,  96]
       [   2,   5,   6, * 6, * 7,   9,  45,  45,   1,   1,  96]
       [   2,   5,   6,   6, * 7, * 9,  45,  45,   1,   1,  96]
       [   2,   5,   6,   6,   7, * 9, *45,  45,   1,   1,  96]
       [   2,   5,   6,   6,   7,   9, *45, *45,   1,   1,  96]
       [   2,   5,   6,   6,   7,   9,  45, *45, * 1,   1,  96]
       [   2,   5,   6,   6,   7,   9, >45,   1,  45,   1,  96]  // swap, step back
       [   2,   5,   6,   6,   7,   9, *45, * 1,  45,   1,  96]
       [   2,   5,   6,   6,   7, > 9,   1,  45,  45,   1,  96]  // swap, step back
       [   2,   5,   6,   6,   7, * 9, * 1,  45,  45,   1,  96]
       [   2,   5,   6,   6, > 7,   1,   9,  45,  45,   1,  96]  // swap, step back
       [   2,   5,   6,   6, * 7, * 1,   9,  45,  45,   1,  96]
       [   2,   5,   6, > 6,   1,   7,   9,  45,  45,   1,  96]  // swap, step back
       [   2,   5,   6, * 6, * 1,   7,   9,  45,  45,   1,  96]
       [   2,   5, > 6,   1,   6,   7,   9,  45,  45,   1,  96]  // swap, step back
       [   2,   5, * 6, * 1,   6,   7,   9,  45,  45,   1,  96]
       [   2, > 5,   1,   6,   6,   7,   9,  45,  45,   1,  96]  // swap, step back
       [   2, * 5, * 1,   6,   6,   7,   9,  45,  45,   1,  96]
       [ > 2,   1,   5,   6,   6,   7,   9,  45,  45,   1,  96]  // swap, step back
       [ * 2, * 1,   5,   6,   6,   7,   9,  45,  45,   1,  96]
       [ > 1,   2,   5,   6,   6,   7,   9,  45,  45,   1,  96]  // swap, step back
       [ * 1, * 2,   5,   6,   6,   7,   9,  45,  45,   1,  96]
       [   1, * 2, * 5,   6,   6,   7,   9,  45,  45,   1,  96]
       [   1,   2, * 5, * 6,   6,   7,   9,  45,  45,   1,  96]
       [   1,   2,   5, * 6, * 6,   7,   9,  45,  45,   1,  96]
       [   1,   2,   5,   6, * 6, * 7,   9,  45,  45,   1,  96]
       [   1,   2,   5,   6,   6, * 7, * 9,  45,  45,   1,  96]
       [   1,   2,   5,   6,   6,   7, * 9, *45,  45,   1,  96]
       [   1,   2,   5,   6,   6,   7, >45,   9,  45,   1,  96]  // swap, step back
       [   1,   2,   5,   6,   6,   7, *45, * 9,  45,   1,  96]
       [   1,   2,   5,   6,   6, > 7,   9,  45,  45,   1,  96]  // swap, step back
       [   1,   2,   5,   6,   6, * 7, * 9,  45,  45,   1,  96]
       [   1,   2,   5,   6,   6,   7, * 9, *45,  45,   1,  96]
       [   1,   2,   5,   6,   6,   7,   9, *45, *45,   1,  96]
       [   1,   2,   5,   6,   6,   7,   9,  45, *45, * 1,  96]
       [   1,   2,   5,   6,   6,   7,   9, >45,   1,  45,  96]  // swap, step back
       [   1,   2,   5,   6,   6,   7,   9, *45, * 1,  45,  96]
       [   1,   2,   5,   6,   6,   7, > 1,   9,  45,  45,  96]  // swap, step back
       [   1,   2,   5,   6,   6,   7, * 1, * 9,  45,  45,  96]
       [   1,   2,   5,   6,   6, * 7, * 1,   9,  45,  45,  96]
       [   1,   2,   5,   6,   1, > 6,   7,   9,  45,  45,  96]  // swap, step back
       [   1,   2,   5,   6, * 6, * 1,   7,   9,  45,  45,  96]
       [   1,   2,   5,   1, > 6,   6,   7,   9,  45,  45,  96]  // swap, step back
       [   1,   2,   5, * 6, * 1,   6,   7,   9,  45,  45,  96]
       [   1,   2,   1, > 5,   6,   6,   7,   9,  45,  45,  96]  // swap, step back
       [   1,   2, * 5, * 1,   6,   6,   7,   9,  45,  45,  96]
       [   1, > 1,   2,   5,   6,   6,   7,   9,  45,  45,  96]  // swap, step back
       [   1, * 2, * 1,   5,   6,   6,   7,   9,  45,  45,  96]
       [   1,   1, * 2, * 5,   6,   6,   7,   9,  45,  45,  96]
       [   1,   1,   2, * 5, * 6,   6,   7,   9,  45,  45,  96]
       [   1,   1,   2,   5, * 6, * 6,   7,   9,  45,  45,  96]
       [   1,   1,   2,   5,   6, * 6, * 7,   9,  45,  45,  96]
       [   1,   1,   2,   5,   6,   6, * 7, * 9,  45,  45,  96]
       [   1,   1,   2,   5,   6,   6,   7, * 9, *45,  45,  96]
       [   1,   1,   2,   5,   6,   6,   7,   9, *45, *45,  96]
       [   1,   1,   2,   5,   6,   6,   7,   9,  45, *45, *96]
       [   1,   1,   2,   5,   6,   6,   7,   9,  45,  45,  96]  // sorted
    */

    /* Time Complexity Analysis

       T(n) = Σ_{i=1}^{n-1}{ i }
            = Σ_{i=0}^{n-1}{ i }
            = (n-1)n / 2
            = (n² - n) / 2

       𝒪[ (n² - n) / 2 ]
        = 𝒪[n²/2 - n/2]
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

    while (start < length - 1) {
        const size_t curr_idx = start;
        const size_t next_idx = start + 1;

        const int curr_val = key(arrayGet(array, curr_idx));
        const int next_val = key(arrayGet(array, next_idx));

        // If in order, step forward
        if (curr_val <= next_val) {
            start++;

            // If out of order, swap...
            // ...and step back (if possible)...
            // ...if not possible, we are at the start, just step forward.
        } else {
            arraySwap(array, curr_idx, next_idx);

            if (start > 0) {
                start--;

            } else {
                start ++;
            }
        }
    }
}

Array *arrayGnomeSorted(const Array *array, const key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    arrayGnomeSort(sorted_array, key);

    return sorted_array;
}
