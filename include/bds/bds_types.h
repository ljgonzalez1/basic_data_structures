#pragma once

#include <stdbool.h>

//// Key functions ////

/**
 * @brief User-provided function that extracts an integer key from an element.
 *
 * This function is used throughout the sorting and searching algorithms to
 * obtain a comparable numeric value from a stored element. The library never
 * interprets the memory pointed to by `elem`; it only forwards the pointer to
 * this callback.
 *
 * Common use cases include:
 *  - returning an integer field of a struct,
 *  - mapping a more complex object to a sortable integer key,
 *  - computing a hashable or rank value on demand.
 *
 * The returned key determines ordering in all comparison-based algorithms.
 *
 * @param elem Pointer to the element stored in the Array or Heap.
 * @return An integer key representing the element.
 */
typedef int (*key_val_func)(const void *elem);

/**
 * @brief User-provided function that destroys or frees an element.
 *
 * Used by arrayFreeWith() and similar utilities when stored elements require
 * custom deallocation. The library does not assume any ownership model; it
 * simply calls this function for each element when cleanup is requested.
 *
 * Typical uses include:
 *  - freeing dynamically allocated structs,
 *  - decrementing reference counts,
 *  - releasing associated resources.
 *
 * The function must handle NULL safely if the user's element model allows it.
 *
 * @param elem Pointer to the element to destroy.
 */
typedef void (*deleter_func)(void *elem);

/**
 * @brief Predicate function used to test whether an element matches a condition.
 *
 * Utilized by arrayIdxOf(), arrayCount(), and other filtering-related
 * utilities. The library applies this function to each element and
 * evaluates its boolean result to determine whether the element should be
 * selected or counted.
 *
 * Typical uses include:
 *  - checking if an element satisfies a property,
 *  - matching against a search criterion,
 *  - implementing custom filtering logic without exposing internal structure.
 *
 * @param elem Pointer to the element to test.
 * @return true if the element satisfies the condition, false otherwise.
 */
typedef bool (*filter_func)(const void *elem);  // for counters
