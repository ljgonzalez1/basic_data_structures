#pragma once

#include "bds_list_core.h"

/// Sorting (SAME sorted array)
void listMergesort(List *list, key_val_func key);
void listTimSort(List *list, key_val_func key);
void listCountingSort(List *list, key_val_func key);

/// Sorting (returns NEW sorted array)
List *listMergeSorted(List *list, key_val_func key);
List *listTimSorted(List *list, key_val_func key);
List *listCountingSorted(List *list, key_val_func key);

