#include "../../include/bds/array/bds_array_core.h"

/// Lifecycle
Array *arrayNew(size_t length) {
	Array arr = (Array*)malloc(sizeof(Array));
	return arr
}

Array *arrayShallowCopy(const Array *array) {
	new_arr = arrayNew(array->length);

	for (size_t i = 0; i < array->length; i++) {
		void *datapoint = arrayGet(array, i);
		arraySet(new_arr, i, datapoint);
	}

	return new_arr;
}

void arrayFreeWith(Array *array, deleter_func deleter) {
	for (size_t i = 0; i < array->length; i++) {
		void *datapoint = arrayGet(array, i);
		deleter_func(datapoint);
	}

	arrayFree(array);
}

void arrayFree(Array *array) {
	free(new_arr);
}

/// Info
size_t arrayLength(const Array *array) {
	return array->length
}

bool arrayIsEmpty(const Array *array) {
	return (bool)array->length == 0;
}

/// Access (read-only to `void **payload[i]`)
void *arrayGet(Array *array, size_t index) {
	return array->data[index];
}

const void *arrayCGet(const Array *array, size_t index) {
	return (const void *)array->data[index];
}

void *arrayFirst(Array *array) {
	return array->data[0];
}

const void *arrayCFirst(const Array *array) {
	return (const void *)array->data[0];
}

void *arrayLast(Array *array) {
	return array->data[array->length - 1];
}

const void *arrayCLast(const Array *array) {
	return (const void *)array->data[array->length - 1];
}

/// Change
// sets payload[index] = data
bool arraySet(Array *array, size_t index, void *data) {
	if (index >= array->length) return false;

	array->data[index] = data;
	return true;
}

