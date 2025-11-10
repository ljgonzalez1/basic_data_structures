#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "vector.h"

void test_vector_create_destroy(void) {
    Vector *vec = vector_create(10);
    assert(vec != NULL);
    assert(vector_size(vec) == 0);
    assert(vector_is_empty(vec) == 1);
    vector_destroy(vec);
    printf("✓ test_vector_create_destroy passed\n");
}

void test_vector_push_pop(void) {
    Vector *vec = vector_create(2);
    int nums[] = {1, 2, 3, 4, 5};
    
    for (int i = 0; i < 5; i++) {
        assert(vector_push_back(vec, &nums[i]) == 0);
    }
    
    assert(vector_size(vec) == 5);
    assert(vector_is_empty(vec) == 0);
    
    int *val = (int *)vector_pop_back(vec);
    assert(*val == 5);
    assert(vector_size(vec) == 4);
    
    vector_destroy(vec);
    printf("✓ test_vector_push_pop passed\n");
}

void test_vector_get_set(void) {
    Vector *vec = vector_create(5);
    int nums[] = {10, 20, 30, 40, 50};
    
    for (int i = 0; i < 5; i++) {
        vector_push_back(vec, &nums[i]);
    }
    
    int *val = (int *)vector_get(vec, 2);
    assert(*val == 30);
    
    int new_val = 99;
    assert(vector_set(vec, 2, &new_val) == 0);
    val = (int *)vector_get(vec, 2);
    assert(*val == 99);
    
    vector_destroy(vec);
    printf("✓ test_vector_get_set passed\n");
}

void test_vector_clear(void) {
    Vector *vec = vector_create(5);
    int nums[] = {1, 2, 3};
    
    for (int i = 0; i < 3; i++) {
        vector_push_back(vec, &nums[i]);
    }
    
    assert(vector_size(vec) == 3);
    vector_clear(vec);
    assert(vector_size(vec) == 0);
    assert(vector_is_empty(vec) == 1);
    
    vector_destroy(vec);
    printf("✓ test_vector_clear passed\n");
}

int main(void) {
    printf("Running vector tests...\n");
    test_vector_create_destroy();
    test_vector_push_pop();
    test_vector_get_set();
    test_vector_clear();
    printf("\nAll vector tests passed!\n");
    return 0;
}
