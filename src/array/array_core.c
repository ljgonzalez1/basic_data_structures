#include "../../include/bds/array/bds_array_core.h"
#include <stdlib.h>

/// Lifecycle
Array *arrayNew(size_t length) {
	Array *arr = (Array *)malloc(sizeof(Array));
	if (!arr) return NULL;

	arr->length = length;
	arr->data = (void **)calloc(length, sizeof(void*));

	if (!arr->data && length > 0) {
		free(arr);
		return NULL;
	}

	return arr;
}

Array *arrayShallowCopy(const Array *array) {
	Array *new_arr = arrayNew(array->length);
	if (!new_arr) return NULL;

	for (size_t i = 0; i < array->length; i++) {
		void *datapoint = arrayGet(array, i);
		arraySet(new_arr, i, datapoint);
	}

	return new_arr;
}

void arrayFreeWith(Array *array, const deleter_func deleter) {
	for (size_t i = 0; i < array->length; i++) {
		void *datapoint = arrayGet(array, i);
		deleter(datapoint);
	}

	arrayFree(array);
}

// Just frees itself
void arrayFree(Array *array) {
	free(array->data);
	free(array);
}

/// Info
size_t arrayLength(const Array *array) {
	return array ? array->length : 0;
}

bool arrayIsEmpty(const Array *array) {
	return array->length == 0;
}

/// Access (read-only to `void **payload[i]`)
void *arrayGet(const Array *array, size_t index) {
	if (index >= array->length) return NULL;

	return array->data[index];
}

void *arrayFirst(const Array *array) {
	if (array->length == 0) return NULL;
	return array->data[0];
}

void *arrayLast(const Array *array) {
	if (array->length == 0) return NULL;
	return array->data[array->length - 1];
}

/// Change
// sets payload[index] = data
bool arraySet(Array *array, size_t index, void *data) {
	if (index >= array->length) return false;

	array->data[index] = data;
	return true;
}

