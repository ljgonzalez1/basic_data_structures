#pragma once

#include <stddef.h>
#include "bds_array_core.h"

/// Search and count
const size_t arrayIdxOf(const Array *array, filter_func key);
const unsigned int arrayCount(const Array *array, filter_func key);
const size_t arrayMinIdx(const Array *array, key_val_func key);
const size_t arrayMaxIdx(const Array *array, key_val_func key);
