/// Merge Sort O(n log n)

#include "../../../include/bds/array/bds_array_sort.h"

#include <stdlib.h>

// No clue how this one works...
static void merge(
    Array *array,
    void **temp,

    const size_t left,
    const size_t mid,
    const size_t right,

    const key_val_func key
    ) {

    size_t left_half_idx = left;
    size_t right_half_idx = mid;
    size_t temp_idx = left;  // buffer

    while (left_half_idx < mid && right_half_idx < right) {
        void *left_elem  = arrayGet(array, left_half_idx);
        void *right_elem = arrayGet(array, right_half_idx);

        const int left_key  = key(left_elem);
        const int right_key = key(right_elem);

        if (left_key <= right_key) {
            temp[temp_idx++] = left_elem;
            left_half_idx++;

        } else {
            temp[temp_idx++] = right_elem;
            right_half_idx++;
        }
    }

    // Copy what's left in the left half
    while (left_half_idx < mid) {
        temp[temp_idx++] = arrayGet(array, left_half_idx++);
    }

    // Copy what's left in the right half
    while (right_half_idx < right) {
        temp[temp_idx++] = arrayGet(array, right_half_idx++);
    }

    // Copy back to the original array
    for (size_t idx = left; idx < right; idx++) {
        arraySet(array, idx, temp[idx]);
    }
}

static void mergeSortRecursive(
    Array *array,
    void **temp,

    const size_t left,
    const size_t right,

    const key_val_func key
    ) {

    // [left, right) = subarray to sort
    if (right - left < 2) return;  // Size 0 or 1 array is already sorted

    // >>> "divide et impera"
    // mid_point = (right - left) / 2
    const size_t midpoint_diff = (right - left) >> 1;
    const size_t mid = left + midpoint_diff;

    // Sort each half by itself recursively
    mergeSortRecursive(array, temp, left, mid,  key);
    mergeSortRecursive(array, temp, mid,  right, key);

    // Merge sorted halves
    merge(array, temp, left, mid, right, key);
}

void arrayMergeSort(Array *array, const key_val_func key) {
    /*
    MERGE-SORT(A, key)
        n ← length(A)
        if n < 2 then
            return

        temp ← new array of size n          // auxiliary buffer
        MERGE-SORT-REC(A, temp, 0, n, key)

    MERGE-SORT-REC(A, temp, left, right, key)
        // sorts subarray A[left..right)  (right is exclusive)
        if right − left < 2 then
            return                          // size 0 or 1 already sorted

        mid ← left + ⌊(right − left)/2⌋     // divide
        MERGE-SORT-REC(A, temp, left, mid,  key)
        MERGE-SORT-REC(A, temp, mid,  right, key)

        MERGE(A, temp, left, mid, right, key)

    MERGE(A, temp, left, mid, right, key)
        i ← left            // pointer into left half  [left, mid)
        j ← mid             // pointer into right half [mid, right)
        k ← left            // pointer into temp buffer starting at left

        while i < mid and j < right do
            if key(A[i]) ≤ key(A[j]) then
                temp[k] ← A[i]
                i ← i + 1
            else
                temp[k] ← A[j]
                j ← j + 1
            k ← k + 1

        while i < mid do
            temp[k] ← A[i]
            i ← i + 1
            k ← k + 1

        while j < right do
            temp[k] ← A[j]
            j ← j + 1
            k ← k + 1

        for t ← left to right − 1 do
            A[t] ← temp[t]                  // copy back
    */


    /* Time Complexity Analysis:
       Let n = length(A).

       Each recursive call splits the array roughly in half and then merges in linear time:
         T(n) = 2T(n/2) + Θ(n)

       By the Master Theorem (a=2, b=2, f(n)=Θ(n)):
         T(n) = Θ(n log n)

       𝒪[T(n)]
        = 𝒪[n log n]

       (Applies to best/average/worst case for standard merge sort.)
    */

    /* Additional Memory Analysis:
       m(n) = n + log n

       - temp buffer: Θ(n) pointers/elements (allocated once and reused).
       - recursion stack: Θ(log n) depth.

       𝒪[m(n)]
        = 𝒪[n]
    */

    /* Total Memory Analysis:
       M(n) = n + m(n)
        = n + (n + log n)
        = 2n + log n

       𝒪[M(n)]
        = 𝒪[2n + log n]
        = 𝒪[n]
    */

    const size_t length = arrayLength(array);
    if (length < 2) return;

    void **temp = (void **)malloc(length * sizeof(void *));
    if (!temp) return;  // Memory allocation failed

    mergeSortRecursive(array, temp, 0, length, key);
    free(temp);
}

Array *arrayMergeSorted(const Array *array, const key_val_func key) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    arrayMergeSort(sorted_array, key);

    return sorted_array;
}

