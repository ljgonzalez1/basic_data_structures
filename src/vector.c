#include "vector.h"
#include <stdlib.h>
#include <string.h>

#define VECTOR_INITIAL_CAPACITY 8
#define VECTOR_GROWTH_FACTOR 2

Vector *vector_create(size_t initial_capacity) {
    Vector *vec = (Vector *)malloc(sizeof(Vector));
    if (vec == NULL) {
        return NULL;
    }
    
    if (initial_capacity == 0) {
        initial_capacity = VECTOR_INITIAL_CAPACITY;
    }
    
    vec->data = (void **)malloc(sizeof(void *) * initial_capacity);
    if (vec->data == NULL) {
        free(vec);
        return NULL;
    }
    
    vec->size = 0;
    vec->capacity = initial_capacity;
    return vec;
}

void vector_destroy(Vector *vec) {
    if (vec == NULL) {
        return;
    }
    free(vec->data);
    free(vec);
}

static int vector_resize(Vector *vec, size_t new_capacity) {
    void **new_data = (void **)realloc(vec->data, sizeof(void *) * new_capacity);
    if (new_data == NULL) {
        return -1;
    }
    vec->data = new_data;
    vec->capacity = new_capacity;
    return 0;
}

int vector_push_back(Vector *vec, void *element) {
    if (vec == NULL) {
        return -1;
    }
    
    if (vec->size >= vec->capacity) {
        size_t new_capacity = vec->capacity * VECTOR_GROWTH_FACTOR;
        if (vector_resize(vec, new_capacity) != 0) {
            return -1;
        }
    }
    
    vec->data[vec->size] = element;
    vec->size++;
    return 0;
}

void *vector_pop_back(Vector *vec) {
    if (vec == NULL || vec->size == 0) {
        return NULL;
    }
    
    vec->size--;
    return vec->data[vec->size];
}

void *vector_get(Vector *vec, size_t index) {
    if (vec == NULL || index >= vec->size) {
        return NULL;
    }
    return vec->data[index];
}

int vector_set(Vector *vec, size_t index, void *element) {
    if (vec == NULL || index >= vec->size) {
        return -1;
    }
    vec->data[index] = element;
    return 0;
}

size_t vector_size(Vector *vec) {
    if (vec == NULL) {
        return 0;
    }
    return vec->size;
}

int vector_is_empty(Vector *vec) {
    return (vec == NULL || vec->size == 0);
}

void vector_clear(Vector *vec) {
    if (vec != NULL) {
        vec->size = 0;
    }
}
