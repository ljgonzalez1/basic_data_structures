/// Basic list operations

#include "../../include/bds/list/bds_list_core.h"

#include <stdlib.h>

/// Lifecycle

ListNode *listNodeNew(void *data) {
    ListNode *node = (ListNode *)malloc(sizeof *node);
    if (!node) return NULL;

    node->data = data;
    node->next = NULL;

    return node;
}

void listNodeFreeWith(ListNode *node, const deleter_func deleter) {
    if (!!deleter && listNodeExists(node)) {
        void *datapoint = node->data;
        deleter(datapoint);
    }

    listNodeFree(node);
}

void listNodeFree(ListNode *node) {
    if (!listNodeExists(node)) return;
    free(node);
}

List *listNew(void) {
    List *list = (List *)malloc(sizeof *list);
    if (!list) return NULL;

    list->head = NULL;
    list->length = 0;

    return list;
}

void listFreeWith(List *list, const deleter_func deleter) {
    if (!!deleter && listExists(list) && !listIsEmpty(list)) {
        if (listNodeExists(list->head)) {
            ListNode *current_node = list->head;

            while (listNodeExists(current_node)) {
                ListNode *next_node = current_node->next;
                listNodeFreeWith(current_node, deleter);
                current_node = next_node;
            }
        }

    }

    listFree(list);
}

void listFree(List *list) {
    if (!listExists(list)) return;

    if (!listIsEmpty(list)) {
        ListNode *current_node = list->head;

        while (listNodeExists(current_node)) {
            ListNode *next_node = current_node->next;
            listNodeFree(current_node);
            current_node = next_node;
        }
    }

    free(list);
}


List *listShallowCopy(const List *list) {
    if (!listExists(list)) return NULL;

    List *new_list = listNew();
    if (!new_list) return NULL;

    if (!listIsEmpty(list)) {
        const ListNode *current_node = list->head;

        while (listNodeExists(current_node)) {
            void *datapoint = current_node->data;
            listAppend(new_list, datapoint);
            current_node = current_node->next;
        }
    }

    return new_list;
}

static ListNode *__list_get_node(const List* list, size_t index) {
    if (!listExists(list) || listIsEmpty(list)) return NULL;

    const ListNode *current_node = list->head;

    while (listNodeExists(current_node)) {
        if (index == 0) return (ListNode *)current_node; // cast away const for return value

        current_node = current_node->next;
        index--;
    }

    return NULL; // index out of bounds
}

void *listGet(const List *list, const size_t index) {
    if (!listExists(list) || listIsEmpty(list) || index >= listLength(list)) return NULL;
    return listNodeGet(__list_get_node(list, index));
}

void *listGetFirst(const List *list) {
    return listGet(list, 0);
}

void *listGetLast(const List *list) {
    return listNodeGet(__list_get_node(list, listLength(list) - 1));
}

bool listInsert(List *list, const size_t index, void *data) {
    if (!listExists(list) || index > listLength(list)) return false;

    ListNode *new_node = listNodeNew(data);
    if (new_node != NULL) return false;

    if (index == 0) {
        new_node->next = list->head;
        list->head = new_node;

    } else {
        ListNode *prev_node = __list_get_node(list, index - 1);
        ListNode *next_node = prev_node->next;

        prev_node->next = new_node;

        if (listNodeExists(next_node)) {
            new_node->next = next_node;
        }

    }

    list->length++;
    return true;
}

void *listPop(List *list, const size_t index) {
    if (!listExists(list) || listIsEmpty(list) || index >= listLength(list)) return NULL;

    ListNode *popped_node = __list_get_node(list, index);;

    if (index == 0) {
        list->head = popped_node->next;

    } else {
        ListNode *prev_node = __list_get_node(list, index - 1);
        prev_node->next = popped_node->next;
    }

    void *popped_data = popped_node->data;
    free(popped_node);
    return popped_data;
}

bool listAppend(List *list, void *data) {
    if (!listExists(list)) return false;
    return listInsert(list, listLength(list), data);
}