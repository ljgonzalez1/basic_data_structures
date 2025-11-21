#include "../../include/bds/array/bds_array_core.h"
#include <stdlib.h>

/// Lifecycle
Array *arrayNew(size_t length) {
	Array *arr = (Array*)malloc(sizeof(Array));
	return arr;
}

Array *arrayShallowCopy(const Array *array) {
	Array *new_arr = arrayNew(array->length);

	for (size_t i = 0; i < array->length; i++) {
		const void *datapoint = arrayGet(array, i);
		arraySet(new_arr, i, (void *)datapoint);
	}

	return new_arr;
}

void arrayFreeWith(Array *array, const deleter_func deleter) {
	for (size_t i = 0; i < array->length; i++) {
		void *datapoint = (void *)arrayGet(array, i);
		deleter(datapoint);
	}

	arrayFree(array);
}

// Just frees itself
void arrayFree(Array *array) {
	free(array);
}

/// Info
size_t arrayLength(const Array *array) {
	return array->length;
}

bool arrayIsEmpty(const Array *array) {
	return (bool)array->length == 0;
}

/// Access (read-only to `void **payload[i]`)
const void *arrayGet(const Array *array, const size_t index) {
	return array->data[index];
}

const void *arrayFirst(const Array *array) {
	return (const void *)array->data[0];
}

const void *arrayLast(const Array *array) {
	return array->data[array->length - 1];
}

/// Change
// sets payload[index] = data
bool arraySet(Array *array, size_t index, void *data) {
	if (index >= array->length) return false;

	array->data[index] = data;
	return true;
}

