#pragma once

#include <stddef.h>
#include "bds_list_core.h"


size_t listIdxOf(const List *list, filter_func key);  // Finds First

unsigned int listCount(const List *list, filter_func key);  // Count of elements matching filter

size_t listMinIdx(const List *list, key_val_func key);  // Returns index of fist occurrence of key value in array, or SIZE_MAX if none

size_t listMaxIdx(const List *list, key_val_func key);  // Returns index of fist occurrence of key value in array, or SIZE_MAX if none



