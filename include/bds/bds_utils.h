#pragma once

#include <stdbool.h>

static inline bool this_struct_exists(void *this_struct) { return !!this_struct; }
