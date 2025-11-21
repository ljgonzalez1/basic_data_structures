#pragma once

#if !defined(_WIN32) && !defined(_WIN64)
    #ifndef _POSIX_C_SOURCE
        #define _POSIX_C_SOURCE 199309L
    #endif
#endif

#include <stdlib.h>
#include "../bds_config.h"

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
    static inline void bds_mem_sleep(void) {
        Sleep(10); // 10 ms
    }
#else
    #include <time.h>
    static inline void bds_mem_sleep(void) {
        struct timespec ts = {0, 10 * 1000000L}; // 10 ms
        nanosleep(&ts, NULL);
    }
#endif

static inline void *bds_malloc(const size_t size) {
    void *ptr = NULL;
    size_t attempts = 0;

    while (attempts < MAX_ALLOC_ATTEMPTS) {
        ptr = malloc(size);

        if (ptr != NULL) {
            return ptr;
        }

        bds_mem_sleep();

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

        bds_mem_sleep();

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

        bds_mem_sleep();

        attempts++;
    }

    return NULL;  // Reallocation failed after max attempts
}

static inline void bds_free(void *ptr) {
    free(ptr);
}
