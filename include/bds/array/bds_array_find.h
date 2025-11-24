#pragma once

#include <stddef.h>
#include "bds_array_core.h"

//// Search and count ////

/**
 * @brief Returns the index of the first element satisfying a condition.
 *
 * Iterates through the array and applies the given filter function to
 * each element. The index of the first element for which the filter
 * returns true is returned.
 *
 * @param array Pointer to an Array. May be NULL.
 * @param key   Predicate function used to test each element.
 * @return The index of the first matching element, or SIZE_MAX if none match
 *         or if the array is NULL.
 */
size_t arrayIdxOf(const Array *array, filter_func key);

/**
 * @brief Counts how many elements satisfy a given condition.
 *
 * Applies the provided filter function to each element in the array
 * and increments the counter for every element for which the filter
 * returns true.
 *
 * @param array Pointer to an Array. May be NULL.
 * @param key   Predicate function used to test each element.
 * @return The number of elements matching the condition. Returns 0 if
 *         the array is NULL or no elements satisfy the filter.
 */
unsigned int arrayCount(const Array *array, filter_func key);

/**
 * @brief Returns the index of the element with the minimum key value.
 *
 * Iterates through the array, evaluating each element with the provided
 * key function, and returns the index of the element with the smallest
 * key. If the array is NULL or empty, SIZE_MAX is returned.
 *
 * @param array Pointer to an Array. May be NULL.
 * @param key   Function used to extract an integer key from each element.
 * @return The index of the element with the minimum key, or SIZE_MAX if
 *         the array is NULL or contains no elements.
 */
size_t arrayMinIdx(const Array *array, key_val_func key);

/**
 * @brief Returns the index of the element with the maximum key value.
 *
 * Iterates through the array, evaluating each element with the provided
 * key function, and returns the index of the element with the largest
 * key. If the array is NULL or empty, SIZE_MAX is returned.
 *
 * @param array Pointer to an Array. May be NULL.
 * @param key   Function used to extract an integer key from each element.
 * @return The index of the element with the maximum key, or SIZE_MAX if
 *         the array is NULL or contains no elements.
 */
size_t arrayMaxIdx(const Array *array, key_val_func key);
