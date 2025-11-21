#pragma once

#include <stdbool.h>

// Key functions
typedef int (*key_val_func)(const void *elem);
typedef void (*deleter_func)(void *elem);
typedef bool (*filter_func)(const void *elem);  // for counters
