/// Selection Sort O(n²) | LST+ARR

#include "../../../include/bds/array/bds_array_sort.h"
#include "../../../include/bds/array/bds_array_utils.h"


void arraySelectionSort(Array *array, const key_val_func key) {
    // Swaps minimum to its correct position.

    /*
    SELECTION-SORT(A, key)

        n ← length(A)
        if n < 2 then
            return

        for i ← 0 to n − 2 do
            min ← i

            for j ← i + 1 to n − 1 do
                if key(A[j]) < key(A[min]) then
                    min ← j

            if min ≠ i then
                swap(A[i], A[min])
    */

    /*
        0     1   2   3   4   5   6   7   8   9  10
       ----------------------------------------------
       [  7,  5,  6,  2, 45,  9,  6, 45,  1,  1, 96]
       [  7,  5,  6,  2, 45,  9,  6, 45, *1,  1, 96] // First minimum
       [ *1,  5,  6,  2, 45,  9,  6, 45, *7,  1, 96] // Swap to the start
       [  1,  5,  6,  2, 45,  9,  6, 45,  7, *1, 96]
       [  1, *1,  6,  2, 45,  9,  6, 45,  7, *5, 96]
       [  1,  1,  6, *2, 45,  9,  6, 45,  7,  5, 96]
       [  1,  1, *2, *6, 45,  9,  6, 45,  7,  5, 96]
       [  1,  1,  2,  6, 45,  9,  6, 45,  7, *5, 96]
       [  1,  1,  2, *5, 45,  9,  6, 45,  7, *6, 96]
       [  1,  1,  2,  5, 45,  9, *6, 45,  7,  6, 96]
       [  1,  1,  2,  5, *6,  9,*45, 45,  7,  6, 96]
       [  1,  1,  2,  5,  6,  9, 45, 45,  7, *6, 96]
       [  1,  1,  2,  5,  6, *6, 45, 45,  7, *9, 96]
       [  1,  1,  2,  5,  6,  6, 45, 45, *7,  9, 96]
       [  1,  1,  2,  5,  6,  6, *7, 45,*45,  9, 96]
       [  1,  1,  2,  5,  6,  6,  7, 45, 45, *9, 96]
       [  1,  1,  2,  5,  6,  6,  7, *9, 45,*45, 96]
       [  1,  1,  2,  5,  6,  6,  7,  9,*45, 45, 96]
       [  1,  1,  2,  5,  6,  6,  7,  9, 45,*45, 96]
       [  1,  1,  2,  5,  6,  6,  7,  9, 45, 45,*96]
       [  1,  1,  2,  5,  6,  6,  7,  9, 45, 45, 96]
    */

    /* Time complexity analysis

    T(n)= Σ_{i=0}^{n−2}{ ( Σ_{j=i+1}^{n−1}{ 1 } ) + 1 }
       = Σ_{i=0}^{n−2}{ (n − 1 − i) + 1 }
       = Σ_{i=0}^{n−2}{ n − i }
       = Σ_{k=2}^{n}{ k }
       = n(n+1)/2 − 1

    𝒪[T(n)]
      = 𝒪[n(n+1)/2 − 1]
      = 𝒪[n²]
    */

    /* Additional Memory Analysis:
       m(n) = c

       Uses only a constant number of extra variables (indices, min value).

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

    // Last one is not considered
    for (size_t idx = 0; idx < length - 1; idx++) {
        // We assume the minimum is at idx
        size_t min_idx = idx;
        int min_val = key(arrayGet(array, idx));

        // find minimum in the rest of the array;
        for (size_t idx_right = idx + 1; idx_right < length; idx_right++) {
            const int idx_right_val = key(arrayGet(array, idx_right));

            if (idx_right_val < min_val) {
                min_val = idx_right_val;
                min_idx = idx_right;
            }
        }

        // swap minimum to its correct position
        if (min_idx != idx) {
            arraySwap(array, idx, min_idx);
        }
    }
}

Array *arraySelectionSorted(const Array *array, const key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    arraySelectionSort(sorted_array, key);

    return sorted_