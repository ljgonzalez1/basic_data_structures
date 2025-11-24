#pragma once

#include "bds_array_core.h"

/**
 * @brief Compares two elements using a key extraction function.
 *
 * Evaluates both elements through the given key function and returns
 * -1, 0, or 1 depending on whether the first element's key is less than,
 * equal to, or greater than the second's.
 *
 * @param datapoint_1 Pointer to the first element.
 * @param datapoint_2 Pointer to the second element.
 * @param key         Function used to extract an integer key from each element.
 * @return -1 if key(datapoint_1) < key(datapoint_2),
 *          0 if they are equal,
 *          1 otherwise.
 */
static inline int arrayKeyCompare(
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

/**
 * @brief Swaps two elements in an Array.
 *
 * Exchanges the elements at the specified indices. If both indices are
 * equal, the function performs no action.
 *
 * @param array Pointer to an Array. May be NULL.
 * @param idx1  First index.
 * @param idx2  Second index.
 */
static inline void arraySwap(Array *array, const size_t idx1, const size_t idx2) {
    if (idx1 == idx2) return;

    void *temp = arrayGet(array, idx1);
    arraySet(array, idx1, arrayGet(array, idx2));
    arraySet(array, idx2, temp);
}

