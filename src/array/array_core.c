#include "../../include/bds/array/bds_array_core.h"

#include <stdlib.h>

/// Lifecycle
Array *arrayNew(const size_t length) {
	Array *arr = (Array *)bds_malloc(sizeof(Array));
	if (!arr) return NULL;

	arr->length = length;
	arr->data = (void **)bds_calloc(length, sizeof(void*));

	if (!arr->data && length > 0) {
		bds_free(arr);
		return NULL;
	}

	return arr;
}

Array *arrayShallowCopy(const Array *array) {
	Array *new_arr = arrayNew(arrayLength(array));
	if (!new_arr) return NULL;

	for (size_t i = 0; i < arrayLength(array); i++) {
		void *datapoint = arrayGet(array, i);
		arraySet(new_arr, i, datapoint);
	}

	return new_arr;
}

void arrayFreeWith(Array *array, const deleter_func deleter) {
	for (size_t i = 0; i < arrayLength(array); i++) {
		void *datapoint = arrayGet(array, i);
		deleter(datapoint);
	}

	arrayFree(array);
}

// Just frees itself
void arrayFree(Array *array) {
	bds_free(array->data);
	bds_free(array);
}

