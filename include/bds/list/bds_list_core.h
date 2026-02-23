#pragma once

#include "../bds_types.h"

#include <stddef.h>   // size_t
#include <stdbool.h>  // bool

typedef struct bds_list_node {
    void *data;
    struct bds_list_node *next;
} ListNode;

typedef struct bds_list {
    ListNode *head;
    size_t length;
} List;

//// Lifecycle ////

ListNode *listNodeNew(void *data);

List *listNew();

List *listShallowCopy(const List *list);

void listNodeFreeWith(ListNode *node, deleter_func deleter);  // Frees payloads according to func

void listNodeFree(ListNode *node);  // Just frees itself

void listFreeWith(List *list, deleter_func deleter);  // Frees payloads according to func

void listFree(List *list);  // Just frees itself

//// Helper ////

static inline bool listExists(const List *list) {
    return !!list;
}

//// Info ////

static inline size_t listLength(const List *list) {
    return listExists(list) ? list->length : 0;
}

static inline bool listIsEmpty(const List *list) {
    return list->length == 0;
}

//// Access (read-only to `void **data[i]`) ////

void *listGet(const List *list, const size_t index);
















//
//
// static inline void *arrayGet(const List *array, const size_t index) {
//     return index < arrayLength(array) ? array->data[index] : NULL;
// }
//
// static inline void *arrayFirst(const List *array) {
//     if (arrayIsEmpty(array)) return NULL;
//     return array->data[0];
// }
//
// static inline void *arrayLast(const List *array) {
//     if (arrayIsEmpty(array)) return NULL;
//     return array->data[array->length - 1];
// }
//
// //// Change ////
//
// // sets array[index] = data
// static inline void arraySet(List *array, const size_t index, void *data) {
//     if (!arrayExists(array) || index >= arrayLength(array)) return;
//     array->data[index] = data;
// }
