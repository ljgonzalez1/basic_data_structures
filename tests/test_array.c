#include "../include/bds/array/bds_array.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>  // size_t, SIZE_MAX

// ======================================================
// Mini framework de tests
// ======================================================

static int g_tests_run    = 0;
static int g_tests_failed = 0;

#define TEST_ASSERT(cond)                                                   \
    do {                                                                    \
        g_tests_run++;                                                      \
        if (!(cond)) {                                                      \
            g_tests_failed++;                                               \
            fprintf(stderr, "FAIL: %s:%d: %s\n",                            \
                    __FILE__, __LINE__, #cond);                             \
        }                                                                   \
    } while (0)

#define TEST_ASSERT_EQ_INT(expected, got)                                   \
    do {                                                                    \
        g_tests_run++;                                                      \
        int _exp = (expected);                                              \
        int _got = (got);                                                   \
        if (_exp != _got) {                                                 \
            g_tests_failed++;                                               \
            fprintf(stderr, "FAIL: %s:%d: expected %d, got %d\n",           \
                    __FILE__, __LINE__, _exp, _got);                        \
        }                                                                   \
    } while (0)

#define TEST_ASSERT_EQ_SIZE(expected, got)                                  \
    do {                                                                    \
        g_tests_run++;                                                      \
        size_t _exp = (expected);                                           \
        size_t _got = (got);                                                \
        if (_exp != _got) {                                                 \
            g_tests_failed++;                                               \
            fprintf(stderr, "FAIL: %s:%d: expected %zu, got %zu\n",         \
                    __FILE__, __LINE__, _exp, _got);                        \
        }                                                                   \
    } while (0)

#define TEST_ASSERT_EQ_UINT(expected, got)                                  \
    do {                                                                    \
        g_tests_run++;                                                      \
        unsigned int _exp = (expected);                                     \
        unsigned int _got = (got);                                          \
        if (_exp != _got) {                                                 \
            g_tests_failed++;                                               \
            fprintf(stderr, "FAIL: %s:%d: expected %u, got %u\n",           \
                    __FILE__, __LINE__, _exp, _got);                        \
        }                                                                   \
    } while (0)

// ======================================================
// Data test
// ======================================================

typedef struct DummyPayload {
    int important_value;  // What we will use as key
    int dummy1;
    int dummy2;
} DummyPayload;

#define STRUCT_ARR_LEN 56u
#define INT12_LEN      12u
#define INT32_LEN      32u

static DummyPayload g_struct_data_56[STRUCT_ARR_LEN];
static int          g_int_data_12[INT12_LEN];
static int          g_int_data_32[INT32_LEN];

static void init_test_data(void) {
    // 56 structs with pseudo-random in important_value
    for (size_t i = 0; i < STRUCT_ARR_LEN; ++i) {
        const int base = (int)((i * 13u) % 101u);  // 0..100 pseudo-random
        g_struct_data_56[i].important_value = base - 50;  // -50..50
        g_struct_data_56[i].dummy1 = (int)i;
        g_struct_data_56[i].dummy2 = (int)(i * 2);
    }

    // 12 fixed ints, negative and positives
    for (size_t i = 0; i < INT12_LEN; ++i) {
        const int tmp12[INT12_LEN] = { -5, 3, 0, -2, 8, -1, 10, 7, -9, 4, -3, 2 };
        g_int_data_12[i] = tmp12[i];
    }

    // 32 fixed ints with repeated values
    const int tmp32[INT32_LEN] = {
        1, 5, 3, 5, 2, 1, 4, 3,
        6, 2, 7, 5, 8, 3, 1, 6,
        4, 2, 7, 8, 5, 3, 1, 4,
        6, 2, 7, 5, 8, 3, 1, 4
    };

    for (size_t i = 0; i < INT32_LEN; ++i) {
        g_int_data_32[i] = tmp32[i];
    }
}

// Build the arraws pointing to this data (they don't free them)
static Array *build_struct_array_56(void) {
    Array *a = arrayNew(STRUCT_ARR_LEN);

    if (!a) return NULL;

    for (size_t i = 0; i < STRUCT_ARR_LEN; ++i) {
        arraySet(a, i, &g_struct_data_56[i]);
    }

    return a;
}

static Array *build_int_array_12(void) {
    Array *a = arrayNew(INT12_LEN);

    if (!a) return NULL;

    for (size_t i = 0; i < INT12_LEN; ++i) {
        arraySet(a, i, &g_int_data_12[i]);
    }

    return a;
}

static Array *build_int_array_32(void) {
    Array *a = arrayNew(INT32_LEN);

    if (!a) return NULL;

    for (size_t i = 0; i < INT32_LEN; ++i) {
        arraySet(a, i, &g_int_data_32[i]);
    }

    return a;
}

// ======================================================
// key_val_func, filter_func, deleter_func
// ======================================================

static int key_int(const void *elem) {
    if (!elem) return 0;
    return *(const int *)elem;
}

static int key_dummy_payload(const void *elem) {
    if (!elem) return 0;

    const DummyPayload *p = (const DummyPayload *)elem;

    return p->important_value;
}

static bool filter_int_is_even(const void *elem) {
    if (!elem) return false;
    const int v = *(const int *)elem;
    return v % 2 == 0;
}

static bool filter_int_is_negative(const void *elem) {
    if (!elem) return false;

    const int v = *(const int *)elem;

    return v < 0;
}

static bool filter_struct_important_positive(const void *elem) {
    if (!elem) return false;

    const DummyPayload *p = (const DummyPayload *)elem;

    return p->important_value > 0;
}

// For testing arrayFreeWith()
static unsigned int g_deleter_calls = 0;

static void int_heap_deleter(void *elem) {
    if (!elem) return;

    g_deleter_calls++;

    free(elem);
}

// ======================================================
// Assertion helpers
// ======================================================

static void assert_array_sorted_by_key(const Array *a, key_val_func key) {
    const size_t len = arrayLength(a);
    if (len == 0) return;

    for (size_t i = 1; i < len; ++i) {
        const int prev = key(arrayGet(a, i - 1));
        const int curr = key(arrayGet(a, i));
        TEST_ASSERT(prev <= curr);
    }
}

// ======================================================
// Basic tests: lifecycle, access, copy
// ======================================================

static void test_array_new_and_free(void) {
    Array *a = arrayNew(5);
    TEST_ASSERT(a != NULL);
    if (!a) return;

    TEST_ASSERT_EQ_SIZE(5u, arrayLength(a));
    TEST_ASSERT(arrayIsEmpty(a) == false);

    for (size_t i = 0; i < arrayLength(a); ++i) {
        TEST_ASSERT(arrayGet(a, i) == NULL);
    }

    arrayFree(a);

    // length = 0
    Array *b = arrayNew(0);
    TEST_ASSERT(b != NULL);
    if (!b) return;

    TEST_ASSERT_EQ_SIZE(0u, arrayLength(b));
    TEST_ASSERT(arrayIsEmpty(b) == true);
    arrayFree(b);
}

static void test_array_set_get(void) {
    Array *a = arrayNew(3);
    TEST_ASSERT(a != NULL);
    if (!a) return;

    int v0 = 10, v1 = -20, v2 = 30;

    arraySet(a, 0, &v0);
    arraySet(a, 1, &v1);
    arraySet(a, 2, &v2);

    TEST_ASSERT(arrayGet(a, 0) == &v0);
    TEST_ASSERT(arrayGet(a, 1) == &v1);
    TEST_ASSERT(arrayGet(a, 2) == &v2);

    // acceso fuera de rango → NULL
    TEST_ASSERT(arrayGet(a, 3) == NULL);

    // first / last
    TEST_ASSERT(arrayFirst(a) == &v0);
    TEST_ASSERT(arrayLast(a)  == &v2);

    arrayFree(a);
}

static void test_array_shallow_copy(void) {
    Array *a = arrayNew(4);
    TEST_ASSERT(a != NULL);
    if (!a) return;

    int vals[4] = { 5, 1, 9, 3 };

    for (size_t i = 0; i < 4; ++i) {
        arraySet(a, i, &vals[i]);
    }

    Array *b = arrayShallowCopy(a);

    TEST_ASSERT(b != NULL);

    if (!b) {
        arrayFree(a);
        return;
    }

    TEST_ASSERT_EQ_SIZE(arrayLength(a), arrayLength(b));

    // Same pointers, different array
    for (size_t i = 0; i < 4; ++i) {
        TEST_ASSERT(arrayGet(a, i) == arrayGet(b, i));
    }

    TEST_ASSERT(a != b);

    arrayFree(a);
    arrayFree(b);
}

// ======================================================
// Find and count tests
// ======================================================

static void test_array_find_and_count(void) {
    // ---- 12 ints (negative + positive) ----
    Array *a12 = build_int_array_12();

    TEST_ASSERT(a12 != NULL);

    if (a12) {
        // primer negative: index 0
        const size_t idx_neg = arrayIdxOf(a12, filter_int_is_negative);
        TEST_ASSERT_EQ_SIZE(0u, idx_neg);

        // total negatives: -5, -2, -1, -9, -3 → 5
        const unsigned int neg_count = arrayCount(a12, filter_int_is_negative);
        TEST_ASSERT_EQ_UINT(5u, neg_count);

        // minimum and maximum
        const size_t min_idx = arrayMinIdx(a12, key_int);  // -9 in index 8
        const size_t max_idx = arrayMaxIdx(a12, key_int);  // 10 in index 6
        TEST_ASSERT_EQ_SIZE(8u, min_idx);
        TEST_ASSERT_EQ_SIZE(6u, max_idx);

        arrayFree(a12);
    }

    // ---- 32 ints repeated ----
    Array *a32 = build_int_array_32();
    TEST_ASSERT(a32 != NULL);

    if (a32) {
        // First pair: value 2 at index 4
        const size_t first_even_idx = arrayIdxOf(a32, filter_int_is_even);
        TEST_ASSERT_EQ_SIZE(4u, first_even_idx);

        // Pair number (we calculate them here)
        unsigned int expected_even = 0;

        for (size_t i = 0; i < INT32_LEN; ++i) {
            if (g_int_data_32[i] % 2 == 0) {
                expected_even++;
            }
        }

        unsigned int even_count = arrayCount(a32, filter_int_is_even);
        TEST_ASSERT_EQ_UINT(expected_even, even_count);

        arrayFree(a32);
    }

    // ---- 56 structs with important_value ----
    Array *as = build_struct_array_56();
    TEST_ASSERT(as != NULL);

    if (as) {
        unsigned int expected_positive = 0;
        size_t expected_min_idx = 0;
        size_t expected_max_idx = 0;
        size_t expected_first_positive_idx = SIZE_MAX;

        int min_val = key_dummy_payload(&g_struct_data_56[0]);
        int max_val = min_val;

        for (size_t i = 0; i < STRUCT_ARR_LEN; ++i) {
            DummyPayload *p = &g_struct_data_56[i];
            const int v = key_dummy_payload(p);

            if (filter_struct_important_positive(p)) {
                if (expected_first_positive_idx == SIZE_MAX) {
                    expected_first_positive_idx = i;
                }

                expected_positive++;
            }

            if (v < min_val) {
                min_val = v;
                expected_min_idx = i;
            }

            if (v > max_val) {
                max_val = v;
                expected_max_idx = i;
            }
        }

        const unsigned int count_positive =
            arrayCount(as, filter_struct_important_positive);
        TEST_ASSERT_EQ_UINT(expected_positive, count_positive);

        const size_t idx_first_pos =
            arrayIdxOf(as, filter_struct_important_positive);
        TEST_ASSERT_EQ_SIZE(expected_first_positive_idx, idx_first_pos);

        const size_t min_idx_s = arrayMinIdx(as, key_dummy_payload);
        const size_t max_idx_s = arrayMaxIdx(as, key_dummy_payload);
        TEST_ASSERT_EQ_SIZE(expected_min_idx, min_idx_s);
        TEST_ASSERT_EQ_SIZE(expected_max_idx, max_idx_s);

        arrayFree(as);
    }
}

// ======================================================
// Tests de arrayFreeWith + deleter
// ======================================================

static void test_array_free_with_deleter(void) {
    const size_t n = 10u;

    Array *a = arrayNew(n);
    TEST_ASSERT(a != NULL);
    if (!a) return;

    for (size_t i = 0; i < n; ++i) {
        int *v = (int *)malloc(sizeof(int));
        TEST_ASSERT(v != NULL);
        if (!v) continue;
        *v = (int)i;
        arraySet(a, i, v);
    }

    g_deleter_calls = 0;
    arrayFreeWith(a, int_heap_deleter);
    TEST_ASSERT_EQ_UINT((unsigned int)n, g_deleter_calls);

    // arrayFreeWith over NULL should not call the deleter
    g_deleter_calls = 0;
    arrayFreeWith(NULL, int_heap_deleter);
    TEST_ASSERT_EQ_UINT(0u, g_deleter_calls);
}

// ======================================================
// Sorting tests
// ======================================================

typedef void (*sort_inplace_func)(Array *array, key_val_func key);
typedef Array *(*sort_newarray_func)(const Array *array, key_val_func key);

static void test_one_sort_inplace(sort_inplace_func sorter) {
    // 56 structs
    {
        Array *a = build_struct_array_56();
        TEST_ASSERT(a != NULL);

        if (a) {
            sorter(a, key_dummy_payload);
            assert_array_sorted_by_key(a, key_dummy_payload);
            arrayFree(a);
        }
    }

    // 12 ints
    {
        Array *a = build_int_array_12();
        TEST_ASSERT(a != NULL);

        if (a) {
            sorter(a, key_int);
            assert_array_sorted_by_key(a, key_int);
            arrayFree(a);
        }
    }

    // 32 ints with repeated numbers
    {
        Array *a = build_int_array_32();
        TEST_ASSERT(a != NULL);

        if (a) {
            sorter(a, key_int);
            assert_array_sorted_by_key(a, key_int);
            arrayFree(a);
        }
    }
}

static void test_one_sort_newarray(const sort_newarray_func sorter) {
    // 56 structs
    {
        Array *orig = build_struct_array_56();
        TEST_ASSERT(orig != NULL);

        if (orig) {
            Array *sorted = sorter(orig, key_dummy_payload);
            TEST_ASSERT(sorted != NULL);

            if (sorted) {
                assert_array_sorted_by_key(sorted, key_dummy_payload);
                arrayFree(sorted);
            }
            arrayFree(orig);
        }
    }

    // 12 ints
    {
        Array *orig = build_int_array_12();
        TEST_ASSERT(orig != NULL);

        if (orig) {
            Array *sorted = sorter(orig, key_int);
            TEST_ASSERT(sorted != NULL);

            if (sorted) {
                assert_array_sorted_by_key(sorted, key_int);
                arrayFree(sorted);
            }
            arrayFree(orig);
        }
    }

    // 32 ints with most likely repeated numbers
    {
        Array *orig = build_int_array_32();
        TEST_ASSERT(orig != NULL);

        if (orig) {
            Array *sorted = sorter(orig, key_int);
            TEST_ASSERT(sorted != NULL);

            if (sorted) {
                assert_array_sorted_by_key(sorted, key_int);
                arrayFree(sorted);
            }

            arrayFree(orig);
        }
    }
}

static void test_array_sort_inplace(void) {
    test_one_sort_inplace(arrayBubbleSort);
    test_one_sort_inplace(arrayInsertionSort);
    test_one_sort_inplace(arraySelectionSort);
    test_one_sort_inplace(arrayCocktailShakerSort);
    test_one_sort_inplace(arrayGnomeSort);
    test_one_sort_inplace(arrayMergeSort);
    test_one_sort_inplace(arrayTimSort);
    test_one_sort_inplace(arrayIntroSort);
    test_one_sort_inplace(arrayShellSort);
    test_one_sort_inplace(arrayQuickSort);
}

static void test_array_sort_new_arrays(void) {
    test_one_sort_newarray(arrayBubbleSorted);
    test_one_sort_newarray(arrayInsertionSorted);
    test_one_sort_newarray(arraySelectionSorted);
    test_one_sort_newarray(arrayCocktailShakerSorted);
    test_one_sort_newarray(arrayGnomeSorted);
    test_one_sort_newarray(arrayMergeSorted);
    test_one_sort_newarray(arrayTimSorted);
    test_one_sort_newarray(arrayIntroSorted);
    test_one_sort_newarray(arrayShellSorted);
    test_one_sort_newarray(arrayQuickSorted);
}

// ======================================================
// main
// ======================================================

int main(void) {
    printf("==> Running array tests\n");

    init_test_data();

    test_array_new_and_free();
    test_array_set_get();
    test_array_shallow_copy();
    test_array_find_and_count();
    test_array_free_with_deleter();
    test_array_sort_inplace();
    test_array_sort_new_arrays();

    printf("Tests run:    %d\n", g_tests_run);
    printf("Tests failed: %d\n", g_tests_failed);

    if (g_tests_failed == 0) {
        printf("All tests PASSED.\n");
        return EXIT_SUCCESS;

    }

    printf("Some tests FAILED.\n");
    return EXIT_FAILURE;
}
