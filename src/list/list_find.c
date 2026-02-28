#include "../../include/bds/list/bds_list_find.h"
#include <stdint.h>

size_t listIndexOf(const List *list, const filter_func key) {
    for (size_t curr = 0; curr < listLength(list); curr++) {
        const void *datapoint = listGet(list, curr);

        if (key(datapoint)) return curr;
    }

    return SIZE_MAX;
}

unsigned int listCount(const List *list, const filter_func key) {
    unsigned int count = 0;
    if (!listExists(list)) return count;

    for (size_t curr = 0; curr < listLength(list); curr++) {
        const void *datapoint = listGet(list, curr);

        if (key(datapoint)) count++;
    }

    return count;
}

size_t listMinIdx(const List *list, const key_val_func key) {
    size_t min_val_idx = SIZE_MAX;
    if (!listExists(list)) return min_val_idx;

    int min_key = INT32_MAX;

    for (size_t curr = 0; curr < listLength(list); curr++) {
        const void *datapoint = listGet(list, curr);

        const int datapoint_key = key(datapoint);

        if (datapoint_key < min_key) {
            min_key = datapoint_key;
            min_val_idx = curr;
        }
    }

    return min_val_idx;
}


size_t listMaxIdx(const List *list, const key_val_func key) {
    size_t min_val_idx = SIZE_MAX;
    if (!listExists(list)) return min_val_idx;

    int min_key = INT32_MAX;

    for (size_t curr = 0; curr < listLength(list); curr++) {
        const void *datapoint = listGet(list, curr);

        const int datapoint_key = key(datapoint);

        if (datapoint_key > min_key) {
            min_key = datapoint_key;
            min_val_idx = curr;
        }
    }

    return min_val_idx;
}