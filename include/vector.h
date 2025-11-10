#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

typedef struct {
    void **data;
    size_t size;
    size_t capacity;
} Vector;

/* Create a new vector with initial capacity */
Vector *vector_create(size_t initial_capacity);

/* Destroy vector and free memory */
void vector_destroy(Vector *vec);

/* Add element to end of vector */
int vector_push_back(Vector *vec, void *element);

/* Remove and return last element */
void *vector_pop_back(Vector *vec);

/* Get element at index */
void *vector_get(Vector *vec, size_t index);

/* Set element at index */
int vector_set(Vector *vec, size_t index, void *element);

/* Get current size */
size_t vector_size(Vector *vec);

/* Check if vector is empty */
int vector_is_empty(Vector *vec);

/* Clear all elements */
void vector_clear(Vector *vec);

#endif /* VECTOR_H */
