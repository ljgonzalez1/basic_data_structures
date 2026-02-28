#pragma once

#include "bds_list_core.h"

/// Sorting (SAME sorted array)
void listMergesort(List *list, key_val_func key);

/// Sorting (returns NEW sorted array)
List *listMergeSorted(List *list, key_val_func key);

