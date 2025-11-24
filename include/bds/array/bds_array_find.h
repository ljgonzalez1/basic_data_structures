#pragma once

#include <stddef.h>
#include "bds_array_core.h"

/// Search and count
size_t arrayIdxOf(const Array *array, filter_func key);
unsigned int arrayCount(const Array *array, filter_func key);
size_t arrayMinIdx(const Array *array, key_val_func key);
size_t arrayMaxIdx(const Array *array, key_val_func key);
