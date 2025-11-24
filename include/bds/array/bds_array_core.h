#pragma once

#include "../bds_types.h"

#include <stddef.h>   // size_t
#include <stdbool.h>  // bool

/**
 * @struct bds_array
 * @brief Generic dynamically-allocated array of pointers.
 *
 * Represents a fixed-length array of `void*` elements.
 * The array owns the `data` buffer, but does NOT own the elements
 * unless explicitly freed using `arrayFreeWith()`.
 *
 * @var bds_array::data
 *     Pointer to a dynamically-allocated buffer of `void*` elements.
 *
 * @var bds_array::length
 *     Number of elements stored in this array. Immutable after creation.
 */
typedef struct bds_array {
    void **data;
    size_t length;
} Array;

//// Lifecycle ////

/**
 * @brief Creates a new Array of the given length.
 *
 * Allocates a new Array structure and an internal buffer of `void*`
 * elements initialized to NULL. The resulting array owns the buffer and
 * must later be freed with `arrayFree()` or `arrayFreeWith()`.
 *
 * @param length
 *        Number of elements the array will contain. May be zero.
 *
 * @return Pointer to a newly allocated Array on success.
 *         Returns NULL if memory allocation fails.
 *
 * @note All items in `array->data` are initialized to NULL.
 * @note Time complexity: O(length) due to `calloc`.
 */
Array *arrayNew(size_t length);

/**
 * @brief Creates a shallow copy of an existing Array.
 *
 * Allocates a new Array with the same length as the source array and
 * copies the pointer values (void*) from the original buffer into the
 * new one. This means:
 *
 *  - The returned Array owns its own `data` buffer.
 *  - The elements inside are NOT duplicated.
 *  - Both arrays reference the same underlying payload pointers.
 *
 * This is useful for algorithms that need to reorder elements without
 * modifying the original array, while avoiding deep copies.
 *
 * @param array
 *        Pointer to the source Array. If NULL, the function returns NULL.
 *
 * @return Pointer to a new shallow-copied Array on success.
 *         Returns NULL if allocation fails or if `array` is NULL.
 *
 * @note Time complexity: O(n)
 *       (copies `void*` elements one by one).
 *
 * @warning Changes to the payload objects affect both arrays, since the
 *          pointers are shared. Only the array structure is duplicated.
 */
Array *arrayShallowCopy(const Array *array);

/**
 * @brief Frees an Array and optionally destroys its elements.
 *
 * Calls the given deleter function for each element in the array (if
 * provided), then frees the array and its internal buffer.
 *
 * @param array   Array to free. May be NULL.
 * @param deleter Function to destroy each element, or NULL to skip
 *                element destruction.
 */
void arrayFreeWith(Array *array, deleter_func deleter);  // Frees payloads according to func

/**
 * @brief Frees an Array and its internal buffer.
 *
 * Releases the memory used by the Array structure and its data buffer.
 * If the pointer is NULL, the function performs no action.
 *
 * @warning This function does not free the elements stored in the array.
 *          Use arrayFreeWith() when element destruction is required.
 *
 * @param array Pointer to the Array to free. May be NULL.
 */
void arrayFree(Array *array);  // Just frees itself

//// Helper ////

/**
 * @brief Checks whether an Array pointer is valid.
 *
 * Returns true if the given pointer is non-NULL.
 *
 * @param array Pointer to an Array.
 * @return true if the pointer is not NULL, false otherwise.
 */
static inline bool arrayExists(const Array *array) {
    return !!array;
}

//// Info ////

/**
 * @brief Returns the number of elements in an Array.
 *
 * If the pointer is NULL, the function returns 0.
 *
 * @param array Pointer to an Array. May be NULL.
 * @return The length of the array, or 0 if the pointer is NULL.
 */
static inline size_t arrayLength(const Array *array) {
    return arrayExists(array) ? array->length : 0;
}

/**
 * @brief Checks whether an Array contains no elements.
 *
 * Returns true if the array exists and its length is zero.
 * A NULL pointer is also considered empty.
 *
 * @param array Pointer to an Array. May be NULL.
 * @return true if the array is empty or NULL, false otherwise.
 */
static inline bool arrayIsEmpty(const Array *array) {
    return arrayExists(array) ? array->length == 0 : true;
}

//// Access (read-only to `void **data[i]`) ////

/**
 * @brief Retrieves an element from an Array at the given index.
 *
 * Returns the pointer stored at the specified position. If the array is
 * NULL or the index is out of bounds, the function returns NULL.
 *
 * @param array Pointer to an Array. May be NULL.
 * @param index Zero-based element index.
 * @return The element at the given index, or NULL if the array is NULL
 *         or the index is out of range.
 */
static inline void *arrayGet(const Array *array, const size_t index) {
    return (
        arrayExists(array) && index < array->length
        ) ? array->data[index] : NULL;
}

/**
 * @brief Returns the first element of an Array.
 *
 * If the array is NULL or empty, the function returns NULL.
 *
 * @param array Pointer to an Array. May be NULL.
 * @return The first element, or NULL if the array is empty or NULL.
 */
static inline void *arrayFirst(const Array *array) {
    if (arrayIsEmpty(array)) return NULL;
    return array->data[0];
}

/**
 * @brief Returns the last element of an Array.
 *
 * If the array is NULL or empty, the function returns NULL.
 *
 * @param array Pointer to an Array. May be NULL.
 * @return The last element, or NULL if the array is empty or NULL.
 */
static inline void *arrayLast(const Array *array) {
    if (arrayIsEmpty(array)) return NULL;
    return array->data[array->length - 1];
}

//// Change ////

// sets array[index] = data
/**
 * @brief Assigns a value to a specific position in an Array.
 *
 * If the array is NULL or the index is out of bounds, the function
 * performs no action.
 *
 * @param array Pointer to an Array. May be NULL.
 * @param index Zero-based index where the value will be stored.
 * @param data  Pointer to store at the given index.
 */
static inline void arraySet(Array *array, const size_t index, void *data) {
    if (!arrayExists(array) || index >= array->length) return;
    array->data[index] = data;
}
