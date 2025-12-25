#pragma once

#include "bds_array_core.h"

/// Sorting (SAME sorted array)

//////////////////////////// AVG: O(n²) ////////////////////////////

/**
 * @brief Sorts an Array in-place using Bubble Sort.
 *
 * Repeatedly scans the array and swaps adjacent elements that are out
 * of order, moving larger elements towards the end on each pass.
 *
 * Time complexity:
 *  - In the worst and average case, it performs on the order of
 *    n * (n - 1) / 2 comparisons and swaps, which is O(n²).
 *  - Best case can be O(n) if the implementation stops early when
 *    no swaps occur on a full pass (already sorted array).
 *
 * Favourable conditions:
 *  - Almost-sorted arrays where few swaps are needed can benefit from
 *    the early-exit optimization.
 *
 * Unfavourable conditions:
 *  - Random or reverse-sorted data, where almost every adjacent pair
 *    is out of order, leads to the full O(n²) behaviour.
 *
 * @param array Pointer to the Array to be sorted. May be NULL.
 * @param key   Function used to extract an integer key from each element.
 */
void arrayBubbleSort(Array *array, key_val_func key);

/**
 * @brief Sorts an Array in-place using Insertion Sort.
 *
 * Builds a sorted prefix on the left side by repeatedly inserting the
 * current element into its correct position within the already-sorted
 * part of the array.
 *
 * Time complexity:
 *  - Worst and average case: about n * (n - 1) / 2 comparisons and
 *    shifts, giving O(n²).
 *  - Best case: O(n) when the array is already sorted and each element
 *    is inserted without shifting.
 *
 * Favourable conditions:
 *  - Nearly-sorted arrays or arrays with small “local disorder”, where
 *    elements are close to their final positions.
 *
 * Unfavourable conditions:
 *  - Reverse-sorted arrays or data where most elements need to move
 *    across large portions of the array, triggering many shifts.
 *
 * @param array Pointer to the Array to be sorted. May be NULL.
 * @param key   Function used to extract an integer key from each element.
 */
void arrayInsertionSort(Array *array, key_val_func key);

/**
 * @brief Sorts an Array in-place using Selection Sort.
 *
 * On each pass, finds the smallest remaining element and swaps it into
 * its final position, growing a sorted prefix from the left.
 *
 * Time complexity:
 *  - Always performs roughly n * (n - 1) / 2 comparisons, regardless
 *    of the initial order: O(n²).
 *  - Performs at most n - 1 swaps, which can be an advantage when
 *    swaps are significantly more expensive than comparisons.
 *
 * Favourable conditions:
 *  - Situations where the cost of swapping elements is high but
 *    comparisons are relatively cheap.
 *
 * Unfavourable conditions:
 *  - Large arrays where O(n²) comparisons are too expensive,
 *    independent of how sorted the input already is.
 *
 * @param array Pointer to the Array to be sorted. May be NULL.
 * @param key   Function used to extract an integer key from each element.
 */
void arraySelectionSort(Array *array, key_val_func key);

/**
 * @brief Sorts an Array in-place using Cocktail Shaker Sort.
 *
 * A bidirectional variation of Bubble Sort that alternates passes
 * from left-to-right and right-to-left, moving large elements towards
 * the end and small elements towards the beginning in each full cycle.
 *
 * Time complexity:
 *  - Worst and average case: still O(n²), due to repeated passes over
 *    the array with adjacent comparisons.
 *  - Best case can be closer to O(n) if the array is already sorted
 *    and the algorithm stops early when no swaps occur.
 *
 * Favourable conditions:
 *  - Arrays where elements that are out of place tend to be near both
 *    ends; the bidirectional passes can converge faster than plain
 *    Bubble Sort.
 *
 * Unfavourable conditions:
 *  - Random or reverse-sorted data on large arrays; as with Bubble Sort,
 *    many passes are needed, leading to O(n²) behaviour.
 *
 * @param array Pointer to the Array to be sorted. May be NULL.
 * @param key   Function used to extract an integer key from each element.
 */
void arrayCocktailShakerSort(Array *array, key_val_func key);

/**
 * @brief Sorts an Array in-place using Gnome Sort.
 *
 * A simple algorithm related to Insertion Sort: it walks through the
 * array, swapping adjacent out-of-order elements and stepping backwards
 * when a swap occurs, forwards otherwise.
 *
 * Time complexity:
 *  - Worst and average case: O(n²), since elements may move step by
 *    step across the array using adjacent swaps.
 *  - Best case: O(n) when the array is already sorted and no swaps
 *    are needed.
 *
 * Favourable conditions:
 *  - Nearly-sorted arrays where only small local corrections are
 *    required; the backward steps stay short.
 *
 * Unfavourable conditions:
 *  - Reverse-sorted or highly disordered data, where many elements
 *    need long sequences of adjacent swaps to reach their positions.
 *
 * @param array Pointer to the Array to be sorted. May be NULL.
 * @param key   Function used to extract an integer key from each element.
 */
void arrayGnomeSort(Array *array, key_val_func key);


//////////////////////////// AVG: O(n log n) ////////////////////////////

/**
 * @brief Sorts an Array in-place using Merge Sort.
 *
 * Recursively divides the array into halves, sorts each half, and merges
 * them back together in sorted order. The merge phase requires an
 * auxiliary buffer of the same size as the array.
 *
 * Time complexity:
 *  - Always O(n log n), regardless of input order.
 *  - Merge operations dominate the cost: each level merges n elements,
 *    and there are log n levels.
 *
 * Favorable conditions:
 *  - Inputs of any shape; performance is predictable and stable.
 *  - Situations requiring consistent runtime guarantees.
 *
 * Unfavorable conditions:
 *  - Environments where additional memory is expensive, since merging
 *    requires a temporary buffer of size n.
 *
 * @param array Pointer to the Array to sort. May be NULL.
 * @param key   Function used to extract an integer key.
 */
void arrayMergeSort(Array *array, key_val_func key);

/**
 * @brief Sorts an Array in-place using Heap Sort.
 *
 * TODO: DOCUMENTAR
 *
 * @param array Pointer to the Array to sort. May be NULL.
 * @param key   Function used to extract an integer key.
 */
void arrayHeapSort(Array *array, key_val_func key);

/**
 * @brief Sorts an Array in-place using TimSort.
 *
 * Hybrid stable sort combining Merge Sort and Insertion Sort. It detects
 * naturally occurring runs (ascending or descending), normalizes them,
 * extends short runs using insertion sort, and merges them using adaptive
 * strategies including galloping.
 *
 * Time complexity:
 *  - Best case O(n) when the input consists largely of natural runs.
 *  - Typical and average case O(n log n).
 *  - Worst-case O(n log n) due to controlled merge policies.
 *
 * Favorable conditions:
 *  - Real-world data that is partially ordered or contains long runs.
 *  - Scenarios requiring stability along with high performance.
 *
 * Unfavorable conditions:
 *  - Highly random data with no natural runs still achieves O(n log n),
 *    but does not benefit from TimSort’s optimizations.
 *
 * @param array Pointer to the Array to sort. May be NULL.
 * @param key   Function used to extract an integer key.
 */
void arrayTimSort(Array *array, key_val_func key);

/**
 * @brief Sorts an Array in-place using Intro Sort.
 *
 * Hybrid algorithm that begins with QuickSort and switches to HeapSort
 * when recursion depth exceeds a computed limit, preventing QuickSort’s
 * worst-case O(n²) behavior. Small partitions are finished using
 * insertion sort.
 *
 * Time complexity:
 *  - Average and typical case: O(n log n).
 *  - Guaranteed worst case: O(n log n), due to the HeapSort fallback.
 *  - Depth limit typically set to 2 * floor(log₂(n)).
 *
 * Favorable conditions:
 *  - Situations requiring QuickSort-like speed but with deterministic
 *    worst-case bounds.
 *  - Inputs where partitioning is generally good but occasional bad
 *    pivots must be mitigated.
 *
 * Unfavorable conditions:
 *  - Data requiring stability (IntroSort is unstable).
 *  - Tiny arrays where simpler algorithms perform better.
 *
 * @param array Pointer to the Array to sort. May be NULL.
 * @param key   Function used to extract an integer key.
 */
void arrayIntroSort(Array *array, key_val_func key);

//////////////////////////// AVG: O(n log² n) ///////////////////////////

/**
 * @brief Sorts an Array in-place using Shell Sort (Hibbard gap sequence).
 *
 * Shell Sort is a generalization of Insertion Sort that improves
 * performance by comparing elements far apart at the beginning and
 * gradually reducing the gap. This reduces the amount of shifting needed
 * when elements are far from their correct positions.
 *
 * Time complexity:
 *  - Best case: O(n log n)
 *  - Typical/average case: between O(n^(3/2)) and O(n log² n),
 *    depending on the input and gap sequence.
 *  - Worst case: O(n^(3/2)) with Hibbard gaps used here.
 *
 * Favorable conditions:
 *  - Moderately sized arrays.
 *  - Data with moderate disorder where long-distance movements improve
 *    performance over simple Insertion Sort.
 *
 * Unfavorable conditions:
 *  - Very large arrays where O(n log n) algorithms (Merge, Heap, Tim, Intro)
 *    are typically faster.
 *  - When stability is required, as Shell Sort is not stable.
 *
 * @param array Pointer to the Array to sort. May be NULL.
 * @param key   Function used to extract an integer key.
 */
void arrayShellSort(Array *array, key_val_func key);

//////////////////////////// AVG: O(n log n) ; WORST O(n²) ////////////////////////////

/**
 * @brief Sorts an Array in-place using Quick Sort.
 *
 * Uses a median-of-three pivot selection strategy combined with
 * Lomuto partitioning to reduce the likelihood of poor pivot choices.
 * Small subarrays are handled using insertion sort to reduce overhead.
 * The algorithm is in-place but not stable.
 *
 * Time complexity:
 *  - Average case: O(n log n)
 *  - Best case: O(n log n)
 *  - Worst case: O(n²), typically avoided by median-of-three pivoting
 *    and by switching to insertion sort on small partitions.
 *
 * Favorable conditions:
 *  - Random or moderately structured data.
 *  - Workloads where average-case speed is more important than
 *    worst-case guarantees.
 *  - Arrays large enough to benefit from the low overhead of in-place partitioning.
 *
 * Unfavorable conditions:
 *  - Already sorted or reverse-sorted data *when not using median-of-three*,
 *    though your implementation specifically avoids this pathology.
 *  - Situations requiring algorithmic stability.
 *  - Applications that require a strict O(n log n) worst-case guarantee
 *    → IntroSort is more suitable.
 *
 * @param array Pointer to the Array to sort. May be NULL.
 * @param key   Function used to extract an integer key.
 */
void arrayQuickSort(Array *array, key_val_func key);

/// Sorting (returns NEW sorted array)

// AVG: O(n²)
/**
 * @brief Returns a new array sorted using Bubble Sort.
 *
 * Creates a shallow copy of the input array and sorts it in-place.
 *
 * @param array Source array. May be NULL.
 * @param key   Function used to extract integer keys.
 * @return New sorted array, or NULL on allocation failure.
 */
Array *arrayBubbleSorted (const Array *array, key_val_func key);

/**
 * @brief Returns a new array sorted using Insertion Sort.
 *
 * Creates a shallow copy of the input array and sorts it in-place.
 *
 * @param array Source array. May be NULL.
 * @param key   Function used to extract integer keys.
 * @return New sorted array, or NULL on allocation failure.
 */
Array *arrayInsertionSorted(const Array *array, key_val_func key);

/**
 * @brief Returns a new array sorted using Selection Sort.
 *
 * Creates a shallow copy of the input array and sorts it in-place.
 *
 * @param array Source array. May be NULL.
 * @param key   Function used to extract integer keys.
 * @return New sorted array, or NULL on allocation failure.
 */
Array *arraySelectionSorted(const Array *array, key_val_func key);

/**
 * @brief Returns a new array sorted using Cocktail Shaker Sort.
 *
 * Creates a shallow copy of the input array and sorts it in-place.
 *
 * @param array Source array. May be NULL.
 * @param key   Function used to extract integer keys.
 * @return New sorted array, or NULL on allocation failure.
 */
Array *arrayCocktailShakerSorted(const Array *array, key_val_func key);

/**
 * @brief Returns a new array sorted using Gnome Sort.
 *
 * Creates a shallow copy of the input array and sorts it in-place.
 *
 * @param array Source array. May be NULL.
 * @param key   Function used to extract integer keys.
 * @return New sorted array, or NULL on allocation failure.
 */
Array *arrayGnomeSorted(const Array *array, key_val_func key);

// AVG: O(n log n)
/**
 * @brief Returns a new array sorted using Merge Sort.
 *
 * Creates a shallow copy of the input array and sorts it in-place.
 *
 * @param array Source array. May be NULL.
 * @param key   Function used to extract integer keys.
 * @return New sorted array, or NULL on allocation failure.
 */
Array *arrayMergeSorted(const Array *array, key_val_func key);

/**
 * @brief Returns a new array sorted using Heap Sort.
 *
 * Creates a shallow copy of the input array and sorts it in-place.
 *
 * @param array Source array. May be NULL.
 * @param key   Function used to extract integer keys.
 * @return New sorted array, or NULL on allocation failure.
 */
Array *arrayHeapSorted(const Array *array, key_val_func key);

/**
 * @brief Returns a new array sorted using TimSort.
 *
 * Creates a shallow copy of the input array and sorts it in-place.
 *
 * @param array Source array. May be NULL.
 * @param key   Function used to extract integer keys.
 * @return New sorted array, or NULL on allocation failure.
 */
Array *arrayTimSorted(const Array *array, key_val_func key);

/**
 * @brief Returns a new array sorted using IntroSort.
 *
 * Creates a shallow copy of the input array and sorts it in-place.
 *
 * @param array Source array. May be NULL.
 * @param key   Function used to extract integer keys.
 * @return New sorted array, or NULL on allocation failure.
 */
Array *arrayIntroSorted(const Array *array, key_val_func key);

// AVG: O(n log² n)
/**
 * @brief Returns a new array sorted using Shell Sort.
 *
 * Creates a shallow copy of the input array and sorts it in-place.
 *
 * @param array Source array. May be NULL.
 * @param key   Function used to extract integer keys.
 * @return New sorted array, or NULL on allocation failure.
 */
Array *arrayShellSorted(const Array *array, key_val_func key);
// AVG: O(n log n) ; WORST O(n²)


/**
 * @brief Returns a new array sorted using Quick Sort.
 *
 * Creates a shallow copy of the input array and sorts it in-place.
 *
 * @param array Source array. May be NULL.
 * @param key   Function used to extract integer keys.
 * @return New sorted array, or NULL on allocation failure.
 */
Array *arrayQuickSorted(const Array *array, key_val_func key);
