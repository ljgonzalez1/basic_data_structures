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


/// ===============================================================
/// TimSort with galloping
/// ===============================================================

/**
 * Flow:
 * 1) Detect natural runs (asc/desc) and normalize to ascending.
 * 2) Ensure each run is at least `minrun` via insertion sort.
 * 3) Push runs to a stack and collapse while invariants are violated.
 * 4) Merge runs using galloping (copy blocks when streaks appear).
 */

typedef struct tim_run {
    size_t start_idx;   // inclusive
    size_t length;      // run length
} TimRun;

#define TIM_STACK_MAX 40  // conservative for very large inputs

/// ===============================================================
/// Utility: key-based comparison
/// ===============================================================

/**
 * Returns < 0 if datapoint_1 < datapoint_2, 0 if equal, > 0 if >, by key().
 */
static int timCompare(
    const void *datapoint_1,
    const void *datapoint_2,
    const key_val_func key
) {
    const int key_1 = key(datapoint_1);
    const int key_2 = key(datapoint_2);
    if (key_1 < key_2) return -1;
    if (key_1 > key_2) return 1;
    return 0;
}

/// ===============================================================
/// minrun computation
/// ===============================================================

/**
 * Compute TimSort's minimal run length (in [32, 64] for large arrays).
 */
static size_t timMinRun(size_t total_len) {
    size_t rem_bits = 0;
    while (total_len >= 64) {
        rem_bits |= total_len & 1u;
        total_len >>= 1;
    }
    return total_len + rem_bits;
}

/// ===============================================================
/// Array helpers (subrange operations)
/// ===============================================================

/**
 * Reverse subarray in [low_idx, high_excl_idx). high_excl_idx is exclusive.
 */
static void timReverseRange(
    Array *array,
    const size_t low_idx,
    const size_t high_excl_idx
) {
    if (high_excl_idx == 0) return;    // protect underflow for high_excl_idx - 1
    if (low_idx >= high_excl_idx) return;

    size_t i = low_idx;
    size_t j = high_excl_idx - 1;

    while (i < j) {
        arraySwap(array, i, j);
        i++;
        j--;
    }
}

/**
 * Stable insertion sort over [low_idx, high_excl_idx).
 */
static void timInsertionSortRange(
    Array *array,
    const size_t low_idx,
    const size_t high_excl_idx,
    const key_val_func key
) {
    if (high_excl_idx - low_idx < 2) return;

    for (size_t pivot_idx = low_idx + 1; pivot_idx < high_excl_idx; pivot_idx++) {
        void *pivot_val = arrayGet(array, pivot_idx);
        size_t scan_idx = pivot_idx;

        // Shift larger elements right
        while (scan_idx > low_idx) {
            void *prev_val = arrayGet(array, scan_idx - 1);

            if (timCompare(prev_val, pivot_val, key) <= 0) break; // stable

            arraySet(array, scan_idx, prev_val);
            scan_idx--;
        }

        arraySet(array, scan_idx, pivot_val);
    }
}

/// ===============================================================
/// Run detection
/// ===============================================================

/**
 * Detect a run starting at `run_start_idx` within a full array of length `array_len`.
 * If descending, reverse it to become ascending. Return run length.
 */
static size_t timCountRunAndMakeAscending(
    Array *array,
    const size_t run_start_idx,
    const size_t array_len,
    const key_val_func key
) {
    const size_t last_excl = array_len;

    if (run_start_idx >= last_excl - 1) return 1; // single element

    size_t run_end_idx = run_start_idx + 1;

    const void *first_val  = arrayGet(array, run_start_idx);
    const void *second_val = arrayGet(array, run_end_idx);

    // Descending run
    if (timCompare(second_val, first_val, key) < 0) {
        while (run_end_idx + 1 < last_excl) {
            const void *prev_val = arrayGet(array, run_end_idx);
            const void *next_val = arrayGet(array, run_end_idx + 1);

            if (timCompare(next_val, prev_val, key) >= 0) break;

            run_end_idx++;
        }

        // Normalize to ascending
        timReverseRange(array, run_start_idx, run_end_idx + 1);

    } else {
        // Ascending run
        while (run_end_idx + 1 < last_excl) {
            const void *prev_val = arrayGet(array, run_end_idx);
            const void *next_val = arrayGet(array, run_end_idx + 1);

            if (timCompare(next_val, prev_val, key) < 0) break;

            run_end_idx++;
        }
    }

    return run_end_idx - run_start_idx + 1;
}

/// ===============================================================
/// Galloping helpers
/// ===============================================================

/**
 * Gallop Left:
 * In ascending `left_buff_arr[left_base_idx ... left_base_idx + left_len)`,
 * return count of elements <= pivot_val.
 * (First index `i` where `pivot_val < left_buff_arr[left_base_idx + i]`).
 */
static size_t timGallopLeft(
    const void *pivot_val,
    void **left_buff_arr,
    const size_t left_base_idx,
    const size_t left_len,
    const key_val_func key
) {
    if (left_len == 0) return 0;

    if (timCompare(pivot_val, left_buff_arr[left_base_idx], key) < 0) {
        return 0;
    }
    if (timCompare(pivot_val, left_buff_arr[left_base_idx + left_len - 1], key) >= 0) {
        return left_len;
    }

    size_t low_idx  = 0;
    size_t high_idx = 1;

    // Exponential search to bracket answer in (low_idx, high_idx]
    while (high_idx < left_len &&
           timCompare(pivot_val, left_buff_arr[left_base_idx + high_idx], key) >= 0) {

        low_idx  = high_idx;
        high_idx = (high_idx << 1) + 1;

        if (high_idx > left_len - 1) high_idx = left_len - 1;
    }

    // Now left_buff_arr[base+low_idx] <= pivot_val < left_buff_arr[base+high_idx]
    size_t bin_left  = low_idx + 1;
    size_t bin_right = high_idx;

    // Binary search in (low_idx, high_idx] for first index with pivot_val < left_buff_arr[base+i]
    while (bin_left <= bin_right) {
        const size_t mid_idx = bin_left + ((bin_right - bin_left) >> 1);

        if (timCompare(pivot_val, left_buff_arr[left_base_idx + mid_idx], key) < 0) {
            if (mid_idx == 0) break; // safety

            bin_right = mid_idx - 1;

        } else {
            bin_left = mid_idx + 1;
        }
    }

    return bin_left; // number of elements <= pivot_val
}

/**
 * Gallop Right:
 * In ascending array[right_base_idx ... right_base_idx + right_len),
 * return count of elements < pivot_val.
 * (First index `i` where `array[right_base_idx + i] >= pivot_val`).
 */
static size_t timGallopRight(
    const void *pivot_val,
    Array *array,
    const size_t right_base_idx,
    const size_t right_len,
    const key_val_func key
) {
    if (right_len == 0) return 0;

    const void *first_val = arrayGet(array, right_base_idx);

    if (timCompare(first_val, pivot_val, key) >= 0) return 0;

    const void *last_val = arrayGet(array, right_base_idx + right_len - 1);

    if (timCompare(last_val, pivot_val, key) < 0) return right_len;

    size_t low_idx  = 0;
    size_t high_idx = 1;

    // Exponential search to bracket answer in (low_idx, high_idx]
    while (high_idx < right_len &&
           timCompare(arrayGet(array, right_base_idx + high_idx), pivot_val, key) < 0) {
        low_idx  = high_idx;
        high_idx = (high_idx << 1) + 1;

        if (high_idx > right_len - 1) high_idx = right_len - 1;
    }

    // Now array[base+low_idx] < pivot_val <= array[base+high_idx]
    size_t bin_left  = low_idx + 1;
    size_t bin_right = high_idx;

    // Binary search in (low_idx, high_idx] for first index with array[base+i] >= pivot_val
    while (bin_left <= bin_right) {
        const size_t mid_idx = bin_left + ((bin_right - bin_left) >> 1);
        if (timCompare(arrayGet(array, right_base_idx + mid_idx), pivot_val, key) < 0) {
            bin_left = mid_idx + 1;

        } else {
            if (mid_idx == 0) break; // safety

            bin_right = mid_idx - 1;
        }
    }

    return bin_left; // number of elements < pivot_val
}

/// ===============================================================
/// Merge with galloping
/// ===============================================================

/**
 * Merge two adjacent ascending runs:
 *   left :  [merge_base_idx, merge_base_idx + left_len)
 *   right:  [merge_base_idx + left_len, merge_base_idx + left_len + right_len)
 * Stable; uses a buffer for the left run and galloping for block copies.
 */
static void timMergeAt(
    Array *array,
    const size_t merge_base_idx,
    const size_t left_len,
    const size_t right_len,
    const key_val_func key
) {
    void **left_buff_arr = (void **)malloc(left_len * sizeof(void *));
    if (!left_buff_arr) return; // simple OOM behavior

    for (size_t copy_idx = 0; copy_idx < left_len; copy_idx++) {
        left_buff_arr[copy_idx] = arrayGet(array, merge_base_idx + copy_idx);
    }

    size_t left_idx  = 0;
    size_t right_idx = 0;
    size_t write_idx = merge_base_idx;

    const size_t right_base_idx = merge_base_idx + left_len;

    // Merge with galloping on both sides
    while (left_idx < left_len && right_idx < right_len) {
        void *left_val  = left_buff_arr[left_idx];
        void *right_val = arrayGet(array, right_base_idx + right_idx);

        if (timCompare(left_val, right_val, key) <= 0) {
            // Stable: take from left
            arraySet(array, write_idx++, left_val);
            left_idx++;

            // Gallop on left: copy block of <= right_val
            if (left_idx < left_len) {
                const size_t adv_cnt = timGallopLeft(
                    right_val,
                    left_buff_arr,
                    left_idx,
                    left_len - left_idx,
                    key
                );

                for (size_t k = 0; k < adv_cnt; ++k) {
                    arraySet(array, write_idx++, left_buff_arr[left_idx++]);
                }
            }
        } else {
            // Take from right
            arraySet(array, write_idx++, right_val);
            right_idx++;

            // Gallop on right: copy block of < left_val
            if (right_idx < right_len) {
                const size_t adv_cnt = timGallopRight(
                    left_val,
                    array,
                    right_base_idx + right_idx,
                    right_len - right_idx,
                    key
                );

                for (size_t k = 0; k < adv_cnt; ++k) {
                    void *val = arrayGet(array, right_base_idx + right_idx);
                    arraySet(array, write_idx++, val);
                    right_idx++;
                }
            }
        }
    }

    // Copy remaining left elements
    while (left_idx < left_len) {
        arraySet(array, write_idx++, left_buff_arr[left_idx++]);
    }

    // Remaining right elements are already in place
    free(left_buff_arr);
}

/// ===============================================================
/// Stack invariants and collapse logic
/// ===============================================================

/**
 * Collapse while simplified TimSort invariants are violated:
 *   - run[n-3].length > run[n-2].length + run[n-1].length
 *   - run[n-2].length > run[n-1].length
 * Merge the pair that keeps sizes more balanced.
 */
static void timMergeCollapse(
    Array *array,
    TimRun *run_stack,
    size_t *stack_size,
    const key_val_func key
) {
    while (*stack_size > 1) {
        const size_t n = *stack_size;

        if (n >= 3) {
            const size_t len_A = run_stack[n - 3].length;
            const size_t len_B = run_stack[n - 2].length;
            const size_t len_C = run_stack[n - 1].length;

            if (len_A <= len_B + len_C || len_B <= len_C) {
                if (len_A < len_C) {
                    // Merge B and C
                    const size_t base  = run_stack[n - 2].start_idx;
                    const size_t l1    = run_stack[n - 2].length;
                    const size_t l2    = run_stack[n - 1].length;

                    timMergeAt(array, base, l1, l2, key);
                    run_stack[n - 2].length = l1 + l2;
                    (*stack_size)--; // pop C
                } else {
                    // Merge A and B
                    const size_t base  = run_stack[n - 3].start_idx;
                    const size_t l1    = run_stack[n - 3].length;
                    const size_t l2    = run_stack[n - 2].length;

                    timMergeAt(array, base, l1, l2, key);
                    run_stack[n - 3].length = l1 + l2;

                    // Shift C down
                    run_stack[n - 2] = run_stack[n - 1];
                    (*stack_size)--;
                }
            } else {
                break;
            }

        } else {
            // With two runs, enforce len_left > len_right
            const size_t len_L = run_stack[n - 2].length;
            const size_t len_R = run_stack[n - 1].length;

            if (len_L <= len_R) {
                const size_t base = run_stack[n - 2].start_idx;
                timMergeAt(array, base, len_L, len_R, key);
                run_stack[n - 2].length = len_L + len_R;
                (*stack_size)--;
            } else {
                break;
            }
        }
    }
}

/**
 * Force-collapse the whole stack until a single run remains.
 */
static void timMergeForceCollapse(
    Array *array,
    TimRun *run_stack,
    size_t *stack_size,
    const key_val_func key
) {
    while (*stack_size > 1) {
        const size_t n = *stack_size;

        if (n >= 3) {
            const size_t len_A = run_stack[n - 3].length;
            const size_t len_C = run_stack[n - 1].length;

            if (len_A < len_C) {
                const size_t base = run_stack[n - 2].start_idx;
                const size_t l1   = run_stack[n - 2].length;
                const size_t l2   = run_stack[n - 1].length;

                timMergeAt(array, base, l1, l2, key);
                run_stack[n - 2].length = l1 + l2;
                (*stack_size)--;
            } else {
                const size_t base = run_stack[n - 3].start_idx;
                const size_t l1   = run_stack[n - 3].length;
                const size_t l2   = run_stack[n - 2].length;

                timMergeAt(array, base, l1, l2, key);
                run_stack[n - 3].length = l1 + l2;
                run_stack[n - 2] = run_stack[n - 1];
                (*stack_size)--;
            }
        } else {
            // Only two runs: merge them
            const size_t base = run_stack[0].start_idx;
            const size_t l1   = run_stack[0].length;
            const size_t l2   = run_stack[1].length;

            timMergeAt(array, base, l1, l2, key);
            run_stack[0].length = l1 + l2;
            (*stack_size)--;
        }
    }
}

/// ===============================================================
/// Public API
/// ===============================================================

void arrayTimSort(
    Array *array,
    const key_val_func key
) {
    const size_t total_len = arrayLength(array);
    if (total_len < 2) return;

    const size_t minrun_len = timMinRun(total_len);

    TimRun run_stack[TIM_STACK_MAX];
    size_t stack_size = 0;

    size_t remaining = total_len;
    size_t curr_idx  = 0;

    while (remaining > 0) {
        // 1) Detect natural run and normalize to ascending
        size_t run_len = timCountRunAndMakeAscending(array, curr_idx, total_len, key);

        // 2) Extend to minrun using insertion sort
        if (run_len < minrun_len) {
            const size_t target_len = minrun_len < remaining ? minrun_len : remaining;
            timInsertionSortRange(array, curr_idx, curr_idx + target_len, key);
            run_len = target_len;
        }

        // 3) Push run
        run_stack[stack_size].start_idx = curr_idx;
        run_stack[stack_size].length    = run_len;
        stack_size++;

        // 4) Collapse while invariants are violated
        timMergeCollapse(array, run_stack, &stack_size, key);

        curr_idx  += run_len;
        remaining -= run_len;
    }

    // 5) Final collapse
    timMergeForceCollapse(array, run_stack, &stack_size, key);
}

Array *arrayTimSorted(
    const Array *array,
    const key_val_func key
) {
    Array *sorted = arrayShallowCopy(array);
    if (!sorted) return NULL;

    arrayTimSort(sorted, key);
    return sorted;
}
