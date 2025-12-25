/// Insertion Sort O(n²)

#include "../../../include/bds/array/bds_array_sort.h"

/* **INSERTION SORT WORKS BETTER WITH PARTIALLY SORTED ARRAYS**. */
void arrayInsertionSort(Array *array, const key_val_func key) {

    /* Insertion sort (stable)
    Take a[i] (key) and insert it into the sorted prefix [0..i-1]
    by shifting larger elements to the right. */

    /*
    INSERTION-SORT(A, key)
        n ← length(A)
        if n < 2 then
        return

        for i ← 1 to n − 1 do
        element ← A[i]
        k ← key(element)

        j ← i

        while j > 0 and key(A[j − 1]) > k do
        A[j] ← A[j − 1]     // shift right
        j ← j − 1

        A[j] ← element         // insert
    */

    /*    0    1    2    3    4    5    6    7    8    9   10
       -------------------------------------------------------
       [  7,   5,   6,   2,  45,   9,   6,  45,   1,   1,  96]

       [* 7, * 5,   6,   2,  45,   9,   6,  45,   1,   1,  96]  //// // i = 1, key = 5
       [  7,   7,   6,   2,  45,   9,   6,  45,   1,   1,  96]  // shift right (7 > 5)
       [  5,   7,   6,   2,  45,   9,   6,  45,   1,   1,  96]  // insert key

       [  5, * 7, * 6,   2,  45,   9,   6,  45,   1,   1,  96]  //// // i = 2, key = 6
       [  5,   7,   7,   2,  45,   9,   6,  45,   1,   1,  96]  // shift right (7 > 6)
       [  5,   6,   7,   2,  45,   9,   6,  45,   1,   1,  96]  // insert key

       [  5,   6, * 7, * 2,  45,   9,   6,  45,   1,   1,  96]  //// // i = 3, key = 2
       [  5,   6,   7,   7,  45,   9,   6,  45,   1,   1,  96]  // shift right (7 > 2)
       [  5,   6,   6,   7,  45,   9,   6,  45,   1,   1,  96]  // shift right (6 > 2)
       [  5,   5,   6,   7,  45,   9,   6,  45,   1,   1,  96]  // shift right (5 > 2)
       [  2,   5,   6,   7,  45,   9,   6,  45,   1,   1,  96]  // insert key

       [  2,   5,   6, * 7, *45,   9,   6,  45,   1,   1,  96]  //// // i = 4, key = 45
       [  2,   5,   6,   7,  45,   9,   6,  45,   1,   1,  96]  // insert key

       [  2,   5,   6,   7, *45, * 9,   6,  45,   1,   1,  96]  //// // i = 5, key = 9
       [  2,   5,   6,   7,  45,  45,   6,  45,   1,   1,  96]  // shift right (45 > 9)
       [  2,   5,   6,   7,   9,  45,   6,  45,   1,   1,  96]  // insert key

       [  2,   5,   6,   7,   9, *45, * 6,  45,   1,   1,  96]  //// // i = 6, key = 6
       [  2,   5,   6,   7,   9,  45,  45,  45,   1,   1,  96]  // shift right (45 > 6)
       [  2,   5,   6,   7,   9,   9,  45,  45,   1,   1,  96]  // shift right (9 > 6)
       [  2,   5,   6,   7,   7,   9,  45,  45,   1,   1,  96]  // shift right (7 > 6)
       [  2,   5,   6,   6,   7,   9,  45,  45,   1,   1,  96]  // insert key

       [  2,   5,   6,   6,   7,   9, *45, *45,   1,   1,  96]  //// // i = 7, key = 45
       [  2,   5,   6,   6,   7,   9,  45,  45,   1,   1,  96]  // insert key

       [  2,   5,   6,   6,   7,   9,  45, *45, * 1,   1,  96]  //// // i = 8, key = 1
       [  2,   5,   6,   6,   7,   9,  45,  45,  45,   1,  96]  // shift right (45 > 1)
       [  2,   5,   6,   6,   7,   9,   9,  45,  45,   1,  96]  // shift right (9 > 1)
       [  2,   5,   6,   6,   7,   7,   9,  45,  45,   1,  96]  // shift right (7 > 1)
       [  2,   5,   6,   6,   6,   7,   9,  45,  45,   1,  96]  // shift right (6 > 1)
       [  2,   5,   6,   6,   6,   7,   9,  45,  45,   1,  96]  // shift right (6 > 1)
       [  2,   5,   5,   6,   6,   7,   9,  45,  45,   1,  96]  // shift right (5 > 1)
       [  2,   2,   5,   6,   6,   7,   9,  45,  45,   1,  96]  // shift right (2 > 1)
       [  1,   2,   5,   6,   6,   7,   9,  45,  45,   1,  96]  // insert key

       [  1,   2,   5,   6,   6,   7,   9,  45, *45, * 1,  96]  //// // i = 9, key = 1
       [  1,   2,   5,   6,   6,   7,   9,  45,  45,  45,  96]  // shift right (45 > 1)
       [  1,   2,   5,   6,   6,   7,   9,   9,  45,  45,  96]  // shift right (9 > 1)
       [  1,   2,   5,   6,   6,   7,   7,   9,  45,  45,  96]  // shift right (7 > 1)
       [  1,   2,   5,   6,   6,   6,   7,   9,  45,  45,  96]  // shift right (6 > 1)
       [  1,   2,   5,   6,   6,   6,   7,   9,  45,  45,  96]  // shift right (6 > 1)
       [  1,   2,   5,   5,   6,   6,   7,   9,  45,  45,  96]  // shift right (5 > 1)
       [  1,   2,   2,   5,   6,   6,   7,   9,  45,  45,  96]  // shift right (2 > 1)
       [  1,   1,   2,   5,   6,   6,   7,   9,  45,  45,  96]  // insert key

       [  1,   1,   2,   5,   6,   6,   7,   9,  45, *45, *96]  //// // i = 10, key = 96
       [  1,   1,   2,   5,   6,   6,   7,   9,  45,  45,  96]  // insert key
    */

    /* Time complexity analysis:
       T(n)= Σ_{idx=1}^{n-1}{ ( Σ_{j=1}^{idx}{ 1 } ) + 1 }
         = Σ_{idx=1}^{n-1}{ Σ_{j=1}^{idx}{ 1 } } + Σ_{idx=1}^{n-1}{ 1 }
         = Σ_{idx=1}^{n-1}{ idx } + (n-1)
         = Σ_{idx=0}^{n-1}{ idx } + (n-1)
         = (n-1)n/2 + (n-1)
         = (n² - n)/2 + (n - 1)
         = n²/2 + n/2 - 1

       O[n²/2 + n/2 - 1]
        = O[n²]
    */

    //   Subarray sorted at the left
    const size_t length = arrayLength(array);
    if (length < 2) return;

    for (size_t idx = 1; idx < length; idx++) {
        void *to_insert_elem = arrayGet(array, idx);
        const int key_to_insert = key(to_insert_elem);

        size_t shifted_idx = idx;

        while (
            shifted_idx > 0 &&
            key(arrayGet(array, shifted_idx - 1)) > key_to_insert) {

            void *displaced_elem = arrayGet(array, shifted_idx - 1);
            arraySet(array, shifted_idx, displaced_elem);
            shifted_idx--;
            }

        arraySet(array, shifted_idx, to_insert_elem);
    }
}

Array *arrayInsertionSorted(const Array *array, const key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    arrayInsertionSort(sorted_array, key);

    return sorted_array;
}
