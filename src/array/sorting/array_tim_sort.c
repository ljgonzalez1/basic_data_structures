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
 * High-level flow:
 * 1) Detect natural runs (ascending/descending) and normalize to ascending.
 * 2) Ensure each run has at least `minrun` length using insertion sort.
 * 3) Push runs to a stack and collapse while invariants are violated.
 * 4) Merge runs with galloping (block copies when streaks appear).
 */

typedef struct tim_run {
    size_t start_index;   // inclusive
    size_t length;        // run length
} TimRun;

#define TIM_STACK_MAX 40  // conservative stack size for very large inputs

/// ===============================================================
/// Utility: key-based comparison
/// ===============================================================

/**
 * Returns < 0 if datapoint_1 < datapoint_2,
 *          0 if equal,
 *        > 0 if datapoint_1 > datapoint_2, according to key().
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
 * Computes TimSort's minimal run length in the range [32, 64] for large arrays.
 */
static size_t timMinRun(size_t array_length) {
    size_t remainder_bits = 0;
    while (array_length >= 64) {
        remainder_bits |= array_length & 1u;
        array_length >>= 1;
    }
    return array_length + remainder_bits;
}

/// ===============================================================
/// Array helpers (subrange operations)
/// ===============================================================

/**
 * Reverse the subarray in [low_inclusive, high_exclusive).
 * Both indices are 0-based; high_exclusive is exclusive.
 */
static void timReverseRange(
    Array *array,
    const size_t low_inclusive,
    const size_t high_exclusive
) {
    if (high_exclusive == 0) return; // protect underflow when computing high_exclusive - 1
    if (low_inclusive >= high_exclusive) return;

    size_t left_index  = low_inclusive;
    size_t right_index = high_exclusive - 1;

    while (left_index < right_index) {
        arraySwap(array, left_index, right_index);
        left_index++;
        right_index--;
    }
}

/**
 * Stable insertion sort over [low_inclusive, high_exclusive).
 * Great for small or partially ordered ranges.
 */
static void timInsertionSortRange(
    Array *array,
    const size_t low_inclusive,
    const size_t high_exclusive,
    const key_val_func key
) {
    if (high_exclusive - low_inclusive < 2) return;

    for (size_t pivot_index = low_inclusive + 1; pivot_index < high_exclusive; pivot_index++) {
        void *pivot_value = arrayGet(array, pivot_index);
        size_t scan_index = pivot_index;

        // Shift larger elements to the right
        while (scan_index > low_inclusive) {
            void *previous_value = arrayGet(array, scan_index - 1);
            if (timCompare(previous_value, pivot_value, key) <= 0) break; // stable: keep order for equals
            arraySet(array, scan_index, previous_value);
            scan_index--;
        }
        arraySet(array, scan_index, pivot_value);
    }
}

/// ===============================================================
/// Run detection
/// ===============================================================

/**
 * Detect a run starting at `run_start_index` within an array of total `array_length`.
 * If descending, it is reversed to become ascending.
 * Returns the run length.
 */
static size_t timCountRunAndMakeAscending(
    Array *array,
    const size_t run_start_index,
    const size_t array_length,
    const key_val_func key
) {
    const size_t last_index_exclusive = array_length;

    if (run_start_index >= last_index_exclusive - 1) return 1; // single-element run

    size_t run_end_index = run_start_index + 1;

    const void *first_value  = arrayGet(array, run_start_index);
    const void *second_value = arrayGet(array, run_end_index);

    // Descending run
    if (timCompare(second_value, first_value, key) < 0) {
        while (run_end_index + 1 < last_index_exclusive) {
            const void *prev_value = arrayGet(array, run_end_index);
            const void *next_value = arrayGet(array, run_end_index + 1);
            if (timCompare(next_value, prev_value, key) >= 0) break;
            run_end_index++;
        }
        // Normalize to ascending
        timReverseRange(array, run_start_index, run_end_index + 1);

    } else {
        // Ascending run
        while (run_end_index + 1 < last_index_exclusive) {
            const void *prev_value = arrayGet(array, run_end_index);
            const void *next_value = arrayGet(array, run_end_index + 1);
            if (timCompare(next_value, prev_value, key) < 0) break;
            run_end_index++;
        }
    }

    return run_end_index - run_start_index + 1;
}

/// ===============================================================
/// Galloping helpers
/// ===============================================================

/**
 * Gallop Left:
 * On ascending buffer_array[buffer_base .. buffer_base + buffer_length),
 * return the count of elements <= pivot_value.
 * Equivalent to the first index i such that pivot_value < buffer_array[buffer_base + i].
 * This preserves stability (ties come from the left run first).
 */
static size_t timGallopLeft(
    const void *pivot_value,
    void **buffer_array,
    size_t buffer_base,
    size_t buffer_length,
    const key_val_func key
) {
    if (buffer_length == 0) return 0;

    if (timCompare(pivot_value, buffer_array[buffer_base], key) < 0) {
        return 0;
    }
    if (timCompare(pivot_value, buffer_array[buffer_base + buffer_length - 1], key) >= 0) {
        return buffer_length;
    }

    size_t low_index  = 0;
    size_t high_index = 1;

    // Exponential search to bracket the answer in (low_index, high_index]
    while (high_index < buffer_length &&
           timCompare(pivot_value, buffer_array[buffer_base + high_index], key) >= 0) {
        low_index  = high_index;
        high_index = (high_index << 1) + 1; // 2*high_index + 1
        if (high_index > buffer_length - 1) high_index = buffer_length - 1;
    }

    // Now buffer_array[base+low_index] <= pivot_value < buffer_array[base+high_index]
    size_t binary_left  = low_index + 1;
    size_t binary_right = high_index;

    // Binary search in (low_index, high_index] for first index where pivot_value < buffer_array[base+idx]
    while (binary_left <= binary_right) {
        size_t mid_index = binary_left + ((binary_right - binary_left) >> 1);
        if (timCompare(pivot_value, buffer_array[buffer_base + mid_index], key) < 0) {
            if (mid_index == 0) break; // safety (shouldn't happen)
            binary_right = mid_index - 1;
        } else {
            binary_left = mid_index + 1;
        }
    }

    return binary_left; // number of elements <= pivot_value
}

/**
 * Gallop Right:
 * On ascending array[run_base .. run_base + run_length),
 * return the count of elements < pivot_value.
 * Equivalent to the first index i such that array[run_base + i] >= pivot_value.
 * Stability preserved (ties are taken from the left run).
 */
static size_t timGallopRight(
    const void *pivot_value,
    Array *array,
    size_t run_base,
    size_t run_length,
    const key_val_func key
) {
    if (run_length == 0) return 0;

    void *first_value_in_right = arrayGet(array, run_base);
    if (timCompare(first_value_in_right, pivot_value, key) >= 0) {
        return 0;
    }

    void *last_value_in_right = arrayGet(array, run_base + run_length - 1);
    if (timCompare(last_value_in_right, pivot_value, key) < 0) {
        return run_length;
    }

    size_t low_index  = 0;
    size_t high_index = 1;

    // Exponential search to bracket the answer in (low_index, high_index]
    while (high_index < run_length &&
           timCompare(arrayGet(array, run_base + high_index), pivot_value, key) < 0) {
        low_index  = high_index;
        high_index = (high_index << 1) + 1;
        if (high_index > run_length - 1) high_index = run_length - 1;
    }

    // Now array[run_base+low_index] < pivot_value <= array[run_base+high_index]
    size_t binary_left  = low_index + 1;
    size_t binary_right = high_index;

    // Binary search in (low_index, high_index] for first index with array[run_base+idx] >= pivot_value
    while (binary_left <= binary_right) {
        size_t mid_index = binary_left + ((binary_right - binary_left) >> 1);
        if (timCompare(arrayGet(array, run_base + mid_index), pivot_value, key) < 0) {
            binary_left = mid_index + 1;
        } else {
            if (mid_index == 0) break; // safety
            binary_right = mid_index - 1;
        }
    }

    return binary_left; // number of elements < pivot_value
}

/// ===============================================================
/// Merge with galloping
/// ===============================================================

/**
 * Merge two adjacent ascending runs:
 *   left run : [merge_base_index, merge_base_index + left_length)
 *   right run: [merge_base_index + left_length, merge_base_index + left_length + right_length)
 * Stable. Uses a temporary buffer for the left run and galloping to copy blocks.
 */
static void timMergeAt(
    Array *array,
    const size_t merge_base_index,
    const size_t left_length,
    const size_t right_length,
    const key_val_func key
) {
    // Temporary buffer for the entire left run
    void **left_buffer_array = (void **)malloc(left_length * sizeof(void *));
    if (!left_buffer_array) return; // simple OOM behavior: skip merge

    for (size_t copy_index = 0; copy_index < left_length; copy_index++) {
        left_buffer_array[copy_index] = arrayGet(array, merge_base_index + copy_index);
    }

    size_t left_index   = 0;
    size_t right_index  = 0;
    size_t write_index  = merge_base_index;

    const size_t right_base_index = merge_base_index + left_length;

    // Main merge loop with galloping on both sides
    while (left_index < left_length && right_index < right_length) {
        void *left_value  = left_buffer_array[left_index];
        void *right_value = arrayGet(array, right_base_index + right_index);

        if (timCompare(left_value, right_value, key) <= 0) {
            // Stable: take from left
            arraySet(array, write_index++, left_value);
            left_index++;

            // Gallop on left: copy a block of elements <= right_value
            if (left_index < left_length) {
                size_t advance_count = timGallopLeft(
                    right_value,
                    left_buffer_array,
                    left_index,
                    left_length - left_index,
                    key
                );
                for (size_t advance_index = 0; advance_index < advance_count; advance_index++) {
                    arraySet(array, write_index++, left_buffer_array[left_index++]);
                }
            }
        } else {
            // Take from right
            arraySet(array, write_index++, right_value);
            right_index++;

            // Gallop on right: copy a block of elements < left_value
            if (right_index < right_length) {
                size_t advance_count = timGallopRight(
                    left_value,
                    array,
                    right_base_index + right_index,
                    right_length - right_index,
                    key
                );
                for (size_t advance_index = 0; advance_index < advance_count; advance_index++) {
                    void *value_to_move = arrayGet(array, right_base_index + right_index);
                    arraySet(array, write_index++, value_to_move);
                    right_index++;
                }
            }
        }
    }

    // Copy any remaining elements from the left run
    while (left_index < left_length) {
        arraySet(array, write_index++, left_buffer_array[left_index++]);
    }

    // Remaining elements in the right run are already in place

    free(left_buffer_array);
}

/// ===============================================================
/// Stack invariants and collapse logic
/// ===============================================================

/**
 * Collapse while invariants (simplified TimSort invariants) are violated:
 *   - run[n-3].length > run[n-2].length + run[n-1].length
 *   - run[n-2].length > run[n-1].length
 * If violated, merge the pair that keeps sizes more balanced.
 */
static void timMergeCollapse(
    Array *array,
    TimRun *run_stack,
    size_t *stack_size,
    const key_val_func key
) {
    while (*stack_size > 1) {
        size_t run_count = *stack_size;

        if (run_count >= 3) {
            size_t length_run_A = run_stack[run_count - 3].length;
            size_t length_run_B = run_stack[run_count - 2].length;
            size_t length_run_C = run_stack[run_count - 1].length;

            // If any invariant is broken, merge
            if (length_run_A <= length_run_B + length_run_C || length_run_B <= length_run_C) {
                if (length_run_A < length_run_C) {
                    // Merge B and C
                    size_t merge_base_index = run_stack[run_count - 2].start_index;
                    size_t left_length      = run_stack[run_count - 2].length;
                    size_t right_length     = run_stack[run_count - 1].length;

                    timMergeAt(array, merge_base_index, left_length, right_length, key);
                    run_stack[run_count - 2].length = left_length + right_length;
                    (*stack_size)--; // pop C
                } else {
                    // Merge A and B
                    size_t merge_base_index = run_stack[run_count - 3].start_index;
                    size_t left_length      = run_stack[run_count - 3].length;
                    size_t right_length     = run_stack[run_count - 2].length;

                    timMergeAt(array, merge_base_index, left_length, right_length, key);
                    run_stack[run_count - 3].length = left_length + right_length;

                    // Shift C down
                    run_stack[run_count - 2] = run_stack[run_count - 1];
                    (*stack_size)--; // pop
                }
            } else {
                break;
            }

        } else {
            // With only two runs, ensure run[n-2].length > run[n-1].length
            size_t length_left  = run_stack[run_count - 2].length;
            size_t length_right = run_stack[run_count - 1].length;

            if (length_left <= length_right) {
                size_t merge_base_index = run_stack[run_count - 2].start_index;
                timMergeAt(array, merge_base_index, length_left, length_right, key);
                run_stack[run_count - 2].length = length_left + length_right;
                (*stack_size)--; // pop
            } else {
                break;
            }
        }
    }
}

/**
 * Force-collapse the entire stack until a single run remains.
 */
static void timMergeForceCollapse(
    Array *array,
    TimRun *run_stack,
    size_t *stack_size,
    const key_val_func key
) {
    while (*stack_size > 1) {
        size_t run_count = *stack_size;

        if (run_count >= 3) {
            size_t length_run_A = run_stack[run_count - 3].length;
            size_t length_run_C = run_stack[run_count - 1].length;

            if (length_run_A < length_run_C) {
                size_t merge_base_index = run_stack[run_count - 2].start_index;
                size_t left_length      = run_stack[run_count - 2].length;
                size_t right_length     = run_stack[run_count - 1].length;

                timMergeAt(array, merge_base_index, left_length, right_length, key);
                run_stack[run_count - 2].length = left_length + right_length;
                (*stack_size)--;

            } else {
                size_t merge_base_index = run_stack[run_count - 3].start_index;
                size_t left_length      = run_stack[run_count - 3].length;
                size_t right_length     = run_stack[run_count - 2].length;

                timMergeAt(array, merge_base_index, left_length, right_length, key);
                run_stack[run_count - 3].length = left_length + right_length;
                run_stack[run_count - 2] = run_stack[run_count - 1];
                (*stack_size)--;
            }

        } else {
            // Only two runs left: merge them
            size_t merge_base_index = run_stack[0].start_index;
            size_t left_length      = run_stack[0].length;
            size_t right_length     = run_stack[1].length;

            timMergeAt(array, merge_base_index, left_length, right_length, key);
            run_stack[0].length = left_length + right_length;
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
    const size_t total_length = arrayLength(array);
    if (total_length < 2) return;

    const size_t minrun_length = timMinRun(total_length);

    TimRun run_stack[TIM_STACK_MAX];
    size_t stack_size = 0;

    size_t remaining_count = total_length;
    size_t current_index   = 0;

    while (remaining_count > 0) {
        // 1) Detect natural run and normalize to ascending
        size_t run_length = timCountRunAndMakeAscending(array, current_index, total_length, key);

        // 2) Extend to minrun using insertion sort
        if (run_length < minrun_length) {
            const size_t target_length   = (minrun_length < remaining_count) ? minrun_length : remaining_count;
            const size_t end_exclusive   = current_index + target_length;
            timInsertionSortRange(array, current_index, end_exclusive, key);
            run_length = target_length;
        }

        // 3) Push to stack
        run_stack[stack_size].start_index = current_index;
        run_stack[stack_size].length      = run_length;
        stack_size++;

        // 4) Collapse while invariants are violated
        timMergeCollapse(array, run_stack, &stack_size, key);

        current_index   += run_length;
        remaining_count -= run_length;
    }

    // 5) Final collapse: merge everything into a single run
    timMergeForceCollapse(array, run_stack, &stack_size, key);
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
