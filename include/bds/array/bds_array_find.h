#pragma once

#include <stddef.h>
#include "bds_array_core.h"

//// Search and count ////
///
size_t arrayIdxOf(const Array *array, filter_func key);

unsigned int arrayCount(const Array *array, filter_func key);

// Returns index of fist occurrence of key value in array, or SIZE_MAX if none
size_t arrayMinIdx(const Array *array, key_val_func key);

// Returns index of fist occurrence of key value in array, or SIZE_MAX if none
size_t arrayMaxIdx(const Array *array, key_val_func key);
