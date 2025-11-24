#include "../../../include/bds/array/bds_array_core.h"
#include "../../../include/bds/array/bds_array_sort.h"

#include <stdlib.h>

// Hybrid sorting algorithm derived from merge sort and insertion sort.
// Uses RUNs of ordered elements to optimize sorting time.

/**
 * 1. **Detect runs**: Detect runs of ordered elements in subarrays (both ascending and descending).
 * e.g.: [*2*, *3*, *8*, *9*, _7_, _5_, _1_, *2*, *3*, *4*] ---> [2, 3, 8, 9] && [7, 5, 1] && [2, 3, 4]
 * 2. **Normalize runs**: Reverse descending runs to make them ascending.
 * e.g.: [7, 5, 1] ---> [1, 5, 7]
 * 3. **minrun**: Fixed minimum run size (e.g., 32). If a run is smaller, extend it using **insertion sort**
 * **INSERTION SORT WORKS BETTER WITH PARTIALLY SORTED ARRAYS**.
 * 4. **Stack Runs**: Create a stack of runs.
 * R1:[2, 3, 8, 9] && R2:[7, 5, 1] && R3:[2, 3, 4] --> Stack:[R1, R2, R3]
 * 5. **Merge**: Merge runs from the stack while maintaining certain invariants to ensure efficient merging.
 * |R(n-2)| <= |R(n-1)| + |R(n)|
 * |R(n-1)| <= |R(n)|
 * 6. **Galloping**: Repeatedly merge runs until sorted using **galloping mode** to optimize merging.
 *
 * Galloping mode:
 *  (Galloping) Exponential search to find the point where elements from one run should be inserted into another,
*/

typedef struct tim_run {
    size_t start;   // start index of the run
    size_t length;  // length of the run
} TimRun;

/// ===============================================================
/// Utility: key-based comparison
/// ===============================================================

/**
 * Returns < 0 if a < b, 0 if a == b, > 0 if a > b, according to key().
 */
static int timCompare(
    const void *a,
    const void *b,
    const key_val_func key
) {
    const int ka = key(a);
    const int kb = key(b);

    if (ka < kb) return -1;
    if (ka > kb) return 1;

    return 0;
}

/// ===============================================================
/// minrun computation
/// ===============================================================

/**
 * Computes the minimal run length for Timsort.
 * For large n, minrun will be in the range [32, 64].
 */
static size_t timMinRun(
    size_t n
) {
    size_t r = 0;

    while (n >= 64) {
        r |= n & 1u;
        n >>= 1;
    }

    return n + r;
}

/// ===============================================================
/// Array helpers (subrange operations)
/// ===============================================================

/**
 * Reverses the subarray in range [lo, hi).
 * Both indices are 0-based. hi is exclusive.
 */
static void timReverseRange(
    Array *array,
    const size_t lo,
    const size_t hi
) {
    if (hi == 0) return; // guard for underflow when hi - 1
    if (lo >= hi) return;

    size_t i = lo;
    size_t j = hi - 1;

    while (i < j) {
        arraySwap(array, i, j);
        i++;
        j--;
    }
}

/**
 * Performs insertion sort on the subarray [lo, hi) using key().
 * Stable and efficient on small and partially sorted ranges.
 */
static void timInsertionSortRange(
    Array *array,
    const size_t lo,
    const size_t hi,
    const key_val_func key
) {
    if (hi - lo < 2) return;

    for (size_t i = lo + 1; i < hi; i++) {
        void *value = arrayGet(array, i);
        size_t j = i;

        // Move larger elements one step to the right
        while (j > lo) {
            void *prev = arrayGet(array, j - 1);

            if (timCompare(prev, value, key) <= 0) break;

            arraySet(array, j, prev);
            j--;
        }

        arraySet(array, j, value);
    }
}

/// ===============================================================
/// Run detection
/// ===============================================================

/**
 * Detects a run starting at index `start`, up to `length`.
 * Returns the run length and ensures it is ascending:
 *   - If the natural run is ascending, it is kept as is.
 *   - If it is descending, it is reversed.
 */
static size_t timCountRunAndMakeAscending(
    Array *array,
    const size_t start,
    const size_t length,
    const key_val_func key
) {
    const size_t max = length;

    if (start >= max - 1) return 1; // Single element run

    size_t run_end = start + 1;

    // Decide direction: ascending or descending
    const void *a = arrayGet(array, start);
    const void *b = arrayGet(array, run_end);

    // Descending run
    if (timCompare(b, a, key) < 0) {
        // Extend descending run
        while (run_end + 1 < max) {
            const void *prev = arrayGet(array, run_end);
            const void *next = arrayGet(array, run_end + 1);

            if (timCompare(next, prev, key) >= 0) break;

            run_end++;
        }

        // Reverse to make it ascending
        timReverseRange(array, start, run_end + 1);

    } else {
        // Ascending run
        while (run_end + 1 < max) {
            const void *prev = arrayGet(array, run_end);
            const void *next = arrayGet(array, run_end + 1);

            if (timCompare(next, prev, key) < 0) break;

            run_end++;
        }
    }

    return run_end - start + 1;
}

/// ===============================================================
/// Merge helpers
/// ===============================================================

/**
 * Merges two consecutive ascending runs:
 *   left:  [base, base + len1)
 *   right: [base + len1, base + len1 + len2)
 * The result is a single ascending run at [base, base + len1 + len2).
 *
 * This implementation uses a temporary buffer for the left run.
 * It is stable: ties are taken from the left run first.
 */
static void timMergeAt(
    Array *array,
    const size_t base,
    const size_t len1,
    const size_t len2,
    const key_val_func key
) {
    // Allocate buffer for the left run
    void **temp = (void **)malloc(len1 * sizeof(void *));

    // Allocation failure: merge cannot proceed safely.
    // In a production setting, this should be reported or handled.
    if (!temp) return;

    // Copy left run into temp
    for (size_t i = 0; i < len1; i++) temp[i] = arrayGet(array, base + i);

    size_t left_idx = 0;
    size_t right_idx = 0;
    size_t dest = base;

    const size_t right_base = base + len1;

    // Merge from temp (left) and array (right)
    while (left_idx < len1 && right_idx < len2) {
        void *left_val  = temp[left_idx];
        void *right_val = arrayGet(array, right_base + right_idx);

        // Stable: if equal, prefer the left value
        if (timCompare(left_val, right_val, key) <= 0) {
            arraySet(array, dest++, left_val);
            left_idx++;

        } else {
            arraySet(array, dest++, right_val);
            right_idx++;
        }
    }

    // Copy remaining left elements (if any)
    while (left_idx < len1) {
        arraySet(array, dest++, temp[left_idx++]);
    }

    // Right run remainder is already in place if any remain

    free(temp);
}

/// ===============================================================
/// Stack invariants and collapse logic
/// ===============================================================

/**
 * Collapses runs on the stack while Timsort invariants are violated.
 * This should be called after each new run is pushed.
 *
 * Invariants (simplified):
 *   - A[n-3].length > A[n-2].length + A[n-1].length
 *   - A[n-2].length > A[n-1].length
 */
static void timMergeCollapse(
    Array *array,
    TimRun *stack,
    size_t *stack_size,
    const key_val_func key
) {
    while (*stack_size > 1) {
        const size_t n = *stack_size;

        if (n >= 3) {
            const size_t lenA = stack[n - 3].length;
            const size_t lenB = stack[n - 2].length;
            const size_t lenC = stack[n - 1].length;

            // Check invariants
            if (lenA <= lenB + lenC || lenB <= lenC) {
                // Merge either (n-3,n-2) or (n-2,n-1)
                if (lenA < lenC) {
                    // Merge B and C
                    const size_t base  = stack[n - 2].start;
                    const size_t len1  = stack[n - 2].length;
                    const size_t len2  = stack[n - 1].length;

                    timMergeAt(array, base, len1, len2, key);

                    // Replace run B with merged run, pop C
                    stack[n - 2].length = len1 + len2;
                    (*stack_size)--;

                } else {
                    // Merge A and B
                    const size_t base  = stack[n - 3].start;
                    const size_t len1  = stack[n - 3].length;
                    const size_t len2  = stack[n - 2].length;

                    timMergeAt(array, base, len1, len2, key);

                    stack[n - 3].length = len1 + len2;

                    // Shift C down
                    stack[n - 2] = stack[n - 1];
                    (*stack_size)--;
                }

            } else if (lenB <= lenC) {
                // Merge B and C
                const size_t base  = stack[n - 2].start;
                const size_t len1  = stack[n - 2].length;
                const size_t len2  = stack[n - 1].length;

                timMergeAt(array, base, len1, len2, key);

                stack[n - 2].length = len1 + len2;
                (*stack_size)--;

            } else {
                break;
            }

        } else {
            // Only two runs: enforce simple invariant
            const size_t lenA = stack[n - 2].length;
            const size_t lenB = stack[n - 1].length;

            if (lenA <= lenB) {
                const size_t base  = stack[n - 2].start;
                const size_t len1  = stack[n - 2].length;
                const size_t len2  = stack[n - 1].length;

                timMergeAt(array, base, len1, len2, key);

                stack[n - 2].length = len1 + len2;
                (*stack_size)--;

            } else {
                break;
            }
        }
    }
}

/**
 * Forces all runs on the stack to be merged until a single run remains.
 * This is called after all runs have been identified and pushed.
 */
static void timMergeForceCollapse(
    Array *array,
    TimRun *stack,
    size_t *stack_size,
    const key_val_func key
) {
    while (*stack_size > 1) {
        const size_t n = *stack_size;

        if (n >= 3) {
            const size_t lenA = stack[n - 3].length;
            const size_t lenC = stack[n - 1].length;

            // Choose which pair to merge to keep sizes balanced
            if (lenA < lenC) {
                const size_t base  = stack[n - 2].start;
                const size_t len1  = stack[n - 2].length;
                const size_t len2  = stack[n - 1].length;

                timMergeAt(array, base, len1, len2, key);

                stack[n - 2].length = len1 + len2;
                (*stack_size)--;
            } else {
                const size_t base  = stack[n - 3].start;
                const size_t len1  = stack[n - 3].length;
                const size_t len2  = stack[n - 2].length;

                timMergeAt(array, base, len1, len2, key);

                stack[n - 3].length = len1 + len2;
                stack[n - 2] = stack[n - 1];
                (*stack_size)--;
            }
        } else {
            // Only two runs left
            const size_t base  = stack[0].start;
            const size_t len1  = stack[0].length;
            const size_t len2  = stack[1].length;

            timMergeAt(array, base, len1, len2, key);

            stack[0].length = len1 + len2;
            (*stack_size)--;
        }
    }
}

/// ===============================================================
/// Public API: arrayTimSort / arrayTimSorted
/// ===============================================================

void arrayTimSort(
    Array *array,
    const key_val_func key
) {
    const size_t length = arrayLength(array);
    if (length < 2) return;

    const size_t minrun = timMinRun(length);

    // The stack size is bounded by log²(n), 40 is enough even for huge n.
    TimRun stack[32];
    size_t stack_size = 0;

    size_t remaining = length;
    size_t curr = 0;

    while (remaining > 0) {
        // 1) Detect natural run and make it ascending
        size_t run_len = timCountRunAndMakeAscending(array, curr, length, key);

        // 2) Extend run to at least minrun using insertion sort
        if (run_len < minrun) {
            const size_t force = minrun < remaining ? minrun : remaining;
            const size_t hi = curr + force;
            timInsertionSortRange(array, curr, hi, key);
            run_len = force;
        }

        // 3) Push run into stack
        stack[stack_size].start  = curr;
        stack[stack_size].length = run_len;
        stack_size++;

        // 4) Collapse stack according to Timsort invariants
        timMergeCollapse(array, stack, &stack_size, key);

        curr += run_len;
        remaining -= run_len;
    }

    // 5) Final collapse: merge everything into a single run
    timMergeForceCollapse(array, stack, &stack_size, key);
}

Array *arrayTimSorted(
    const Array *array,
    const key_val_func key
) {
    Array *sorted_array = arrayShallowCopy(array);
    if (!sorted_array) return NULL;

    arrayTimSort(sorted_array, key);
    return sorted_array;
}
