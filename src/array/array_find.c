/// Find in array

#include "../../include/bds/array/bds_array_find.h"
#include <stdint.h>


size_t arrayIdxOf(const Array *array, filter_func key) {
    for (size_t curr = 0; curr < arrayLength(array); curr++) {
        const void *datapoint = arrayGet(array, curr);

        if (key(datapoint)) return curr;
    }

    return SIZE_MAX;
}

unsigned int arrayCount(const Array *array, const filter_func key) {
    unsigned int count = 0;

    for (size_t curr = 0; curr < arrayLength(array); curr++) {
        const void *datapoint = arrayGet(array, curr);

        if (key(datapoint)) count++;
    }

    return count;
}

size_t arrayMinIdx(const Array *array, const key_val_func key) {
    size_t min_val_idx = SIZE_MAX;
    int min_key = INT32_MAX;

    for (size_t curr = 0; curr < arrayLength(array); curr++) {
        const void *datapoint = arrayGet(array, curr);

        const int datapoint_key = key(datapoint);

        if (datapoint_key < min_key) {
            min_key = datapoint_key;
            min_val_idx = curr;
        }
    }

    return min_val_idx;
}

size_t arrayMaxIdx(const Array *array, const key_val_func key) {
    size_t max_val_idx = SIZE_MAX;
    int max_key = INT32_MIN;

    for (size_t curr = 0; curr < arrayLength(array); curr++) {
        const void *datapoint = arrayGet(array, curr);

        const int datapoint_key = key(datapoint);

        if (datapoint_key > max_key) {
            max_key = datapoint_key;
            max_val_idx = curr;
        }
    }

    return max_val_idx;
}

