#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stddef.h>

typedef struct ListNode {
    void *data;
    struct ListNode *next;
} ListNode;

typedef struct {
    ListNode *head;
    ListNode *tail;
    size_t size;
} LinkedList;

/* Create a new linked list */
LinkedList *list_create(void);

/* Destroy list and free memory */
void list_destroy(LinkedList *list);

/* Add element to front of list */
int list_push_front(LinkedList *list, void *data);

/* Add element to back of list */
int list_push_back(LinkedList *list, void *data);

/* Remove and return first element */
void *list_pop_front(LinkedList *list);

/* Get first element without removing */
void *list_front(LinkedList *list);

/* Get last element without removing */
void *list_back(LinkedList *list);

/* Get size of list */
size_t list_size(LinkedList *list);

/* Check if list is empty */
int list_is_empty(LinkedList *list);

/* Clear all elements */
void list_clear(LinkedList *list);

#endif /* LINKED_LIST_H */
