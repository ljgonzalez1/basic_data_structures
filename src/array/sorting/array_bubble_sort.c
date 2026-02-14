/// Bubble Sort O(n²) | LST+ARR

#include "../../../include/bds/array/bds_array_sort.h"
#include "../../../include/bds/array/bds_array_utils.h"

void arrayBubbleSort(Array *array, const key_val_func key) {
    /*
    BUBBLE-SORT(A, key)
        n ← length(A)
        if n < 2 then
            return

        repeat
            swapped ← false

            for i ← 0 to n − 2 do
                if key(A[i]) > key(A[i + 1]) then
                    swap(A[i], A[i + 1])
                    swapped ← true

            n ← n − 1          // last element is now in correct position

        until swapped = false
    */

    // Swaps with neighbour if lower.
    // Repeats n times or until no swaps are made.
    // It pushes highest values to the end of the array.

    /*
        0     1   2   3   4   5   6   7   8   9  10
       ----------------------------------------------
       [  7,  5,  6,  2, 45,  9,  6, 45,  1,  1, 96]
       [ *7, *5,  6,  2, 45,  9,  6, 45,  1,  1, 96]
       [  5, *7, *6,  2, 45,  9,  6, 45,  1,  1, 96]
       [  5,  6, *7, *2, 45,  9,  6, 45,  1,  1, 96]
       [  5,  6,  2, *7,*45,  9,  6, 45,  1,  1, 96]
       [  5,  6,  2,  7,*45, *9,  6, 45,  1,  1, 96]
       [  5,  6,  2,  7,  9,*45, *6, 45,  1,  1, 96]
       [  5,  6,  2,  7,  9,  6,*45,*45,  1,  1, 96]
       [  5,  6,  2,  7,  9,  6, 45,*45, *1,  1, 96]
       [  5,  6,  2,  7,  9,  6, 45,  1,*45, *1, 96]
       [  5,  6,  2,  7,  9,  6, 45,  1,  1,*45,*96]
       [ *5, *6,  2,  7,  9,  6, 45,  1,  1, 45, 96]
       [  5, *6, *2,  7,  9,  6, 45,  1,  1, 45, 96]
       [  5,  2, *6, *7,  9,  6, 45,  1,  1, 45, 96]
       [  5,  2,  6, *7, *9,  6, 45,  1,  1, 45, 96]
       [  5,  2,  6,  7, *9, *6, 45,  1,  1, 45, 96]
       [  5,  2,  6,  7,  6, *9,*45,  1,  1, 45, 96]
       [  5,  2,  6,  7,  6,  9,*45, *1,  1, 45, 96]
       [  5,  2,  6,  7,  6,  9,  1,*45, *1, 45, 96]
       [  5,  2,  6,  7,  6,  9,  1,  1,*45,*45, 96]
       [ *5, *2,  6,  7,  6,  9,  1,  1, 45, 45, 96]
       [  2, *5, *6,  7,  6,  9,  1,  1, 45, 45, 96]
       [  2,  5, *6, *7,  6,  9,  1,  1, 45, 45, 96]
       [  2,  5,  6, *7, *6,  9,  1,  1, 45, 45, 96]
       [  2,  5,  6,  6, *7, *9,  1,  1, 45, 45, 96]
       [  2,  5,  6,  6,  7, *9, *1,  1, 45, 45, 96]
       [  2,  5,  6,  6,  7,  1, *9, *1, 45, 45, 96]
       [  2,  5,  6,  6,  7,  1,  1, *9,*45, 45, 96]
       [ *2, *5,  6,  6,  7,  1,  1,  9, 45, 45, 96]
       [  2, *5, *6,  6,  7,  1,  1,  9, 45, 45, 96]
       [  2,  5, *6, *6,  7,  1,  1,  9, 45, 45, 96]
       [  2,  5,  6, *6, *7,  1,  1,  9, 45, 45, 96]
       [  2,  5,  6,  6, *7, *1,  1,  9, 45, 45, 96]
       [  2,  5,  6,  6,  1, *7, *1,  9, 45, 45, 96]
       [  2,  5,  6,  6,  1,  1, *7, *9, 45, 45, 96]
       [ *2, *5,  6,  6,  1,  1,  7,  9, 45, 45, 96]
       [  2, *5, *6,  6,  1,  1,  7,  9, 45, 45, 96]
       [  2,  5, *6, *6,  1,  1,  7,  9, 45, 45, 96]
       [  2,  5,  6, *6, *1,  1,  7,  9, 45, 45, 96]
       [  2,  5,  6,  1, *6, *1,  7,  9, 45, 45, 96]
       [  2,  5,  6,  1,  1, *6, *7,  9, 45, 45, 96]
       [ *2, *5,  6,  1,  1,  6,  7,  9, 45, 45, 96]
       [  2, *5, *6,  1,  1,  6,  7,  9, 45, 45, 96]
       [  2,  5, *6, *1,  1,  6,  7,  9, 45, 45, 96]
       [  2,  5,  1, *6, *1,  6,  7,  9, 45, 45, 96]
       [  2,  5,  1,  1, *6, *6,  7,  9, 45, 45, 96]
       [ *2, *5,  1,  1,  6,  6,  7,  9, 45, 45, 96]
       [  2, *5, *1,  1,  6,  6,  7,  9, 45, 45, 96]
       [  2,  1, *5, *1,  6,  6,  7,  9, 45, 45, 96]
       [  2,  1,  1, *5, *6,  6,  7,  9, 45, 45, 96]
       [ *2, *1,  1,  5,  6,  6,  7,  9, 45, 45, 96]
       [  1, *2, *1,  5,  6,  6,  7,  9, 45, 45, 96]
       [  1,  1, *2, *5,  6,  6,  7,  9, 45, 45, 96]
       [ *1, *1,  2,  5,  6,  6,  7,  9, 45, 45, 96]
       [  1, *1, *2,  5,  6,  6,  7,  9, 45, 45, 96]
       [  1,  1,  2,  5,  6,  6,  7,  9, 45, 45, 96]
    */

    /* Time Complexity Analysis:
       T(n) = Σ_{i=0}^{n}{ n-i }
        = Σ_{i=1}^{n}{ i }
        = (n²+1)/2

       𝒪[T(n)]
        = 𝒪[(n² + 1)/2]
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

    size_t length = arrayLength(array);
    if (length < 2) return;

    bool swapped;

    do {
        swapped = false;

        for (size_t idx = 0; idx < length - 1; idx++) {
            const size_t next_idx = idx + 1;

            const int this_val = key(arrayGet(array, idx));
            const int next_val = key(arrayGet(array, next_idx));

            if (this_val > next_val) {
                arraySwap(array, idx, next_idx);
                swapped = true;
            }
        }

        length--; // Last element is in place, no need to check again.
    } while (swapped);
}

Array *arrayBubbleSorted(const Array *array, const key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    arrayBubbleSort(sorted_array, key);

    return sorted_array;
}
