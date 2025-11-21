#pragma once

#include <stdlib.h>

#include "../bds_config.h"

static inline void *bds_malloc(const size_t size) {
    void *ptr = NULL;
    size_t attempts = 0;

    while (attempts < MAX_ALLOC_ATTEMPTS) {
        ptr = malloc(size);
        if (ptr != NULL) {
            return ptr;
        }
        attempts++;
    }

    return NULL;  // Allocation failed after max attempts
}

static inline void *bds_calloc(const size_t num, const size_t size) {
    void *ptr = NULL;
    size_t attempts = 0;

    while (attempts < MAX_ALLOC_ATTEMPTS) {
        ptr = calloc(num, size);
        if (ptr != NULL) {
            return ptr;
        }
        attempts++;
    }

    return NULL;  // Allocation failed after max attempts
}

static inline void *bds_realloc(void *ptr, const size_t size) {
    void *new_ptr = NULL;
    size_t attempts = 0;

    while (attempts < MAX_ALLOC_ATTEMPTS) {
        new_ptr = realloc(ptr, size);

        if (new_ptr != NULL) {
            return new_ptr;
        }

        attempts++;
    }

    return NULL;  // Reallocation failed after max attempts
}

static inline void bds_free(void *ptr) {
    free(ptr);
}
