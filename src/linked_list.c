#include "linked_list.h"
#include <stdlib.h>

LinkedList *list_create(void) {
    LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
    if (list == NULL) {
        return NULL;
    }
    
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void list_destroy(LinkedList *list) {
    if (list == NULL) {
        return;
    }
    
    list_clear(list);
    free(list);
}

int list_push_front(LinkedList *list, void *data) {
    if (list == NULL) {
        return -1;
    }
    
    ListNode *node = (ListNode *)malloc(sizeof(ListNode));
    if (node == NULL) {
        return -1;
    }
    
    node->data = data;
    node->next = list->head;
    list->head = node;
    
    if (list->tail == NULL) {
        list->tail = node;
    }
    
    list->size++;
    return 0;
}

int list_push_back(LinkedList *list, void *data) {
    if (list == NULL) {
        return -1;
    }
    
    ListNode *node = (ListNode *)malloc(sizeof(ListNode));
    if (node == NULL) {
        return -1;
    }
    
    node->data = data;
    node->next = NULL;
    
    if (list->tail == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }
    
    list->size++;
    return 0;
}

void *list_pop_front(LinkedList *list) {
    if (list == NULL || list->head == NULL) {
        return NULL;
    }
    
    ListNode *node = list->head;
    void *data = node->data;
    
    list->head = node->next;
    if (list->head == NULL) {
        list->tail = NULL;
    }
    
    free(node);
    list->size--;
    return data;
}

void *list_front(LinkedList *list) {
    if (list == NULL || list->head == NULL) {
        return NULL;
    }
    return list->head->data;
}

void *list_back(LinkedList *list) {
    if (list == NULL || list->tail == NULL) {
        return NULL;
    }
    return list->tail->data;
}

size_t list_size(LinkedList *list) {
    if (list == NULL) {
        return 0;
    }
    return list->size;
}

int list_is_empty(LinkedList *list) {
    return (list == NULL || list->head == NULL);
}

void list_clear(LinkedList *list) {
    if (list == NULL) {
        return;
    }
    
    ListNode *current = list->head;
    while (current != NULL) {
        ListNode *next = current->next;
        free(current);
        current = next;
    }
    
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}
