/// Basic list operations

#include "../../include/bds/list/bds_list_core.h"

#include <stdlib.h>

/// Lifecycle

ListNode *listNodeNew(void *data) {
    ListNode *node = (ListNode *)malloc(sizeof(ListNode));
    if (!node) return NULL;

    node->data = data;
    node->next = NULL;

    return node;
}

List *listNew(void) {
    // FIXME: Allocated memory is leaked when called from global scope
    List *list = (List *)malloc(sizeof(List));
    if (!list) return NULL;

    list->length = 0;
    /* ListNode *self->head = NULL */
    list->head = NULL;

    return list;
}


List *listShallowCopy(const List *list) {
    List *new_list = listNew();
    if (!new_list) return NULL;

    for (size_t i = 0; i < list->length; i++) {
        // TODO: handle what happens on a malloc failure in the middle of the loop (free what we have so far and return NULL)
        ListNode *node_data = listGet(list, i);
        listAppend(node_data);
    }

    return new_list;


}

void listFreeWith(List *list, deleter_func deleter);  // Frees payloads according to func

void listFree(List *list);  // Just frees itself







void *listGet(const List *list, const size_t index) {
    const ListNode *currentNode = list->head;

    for (size_t i = 0; i < index; i++) {
        ListNode currentNode = *currentNode->next;
    }

    return currentNode->data;
}