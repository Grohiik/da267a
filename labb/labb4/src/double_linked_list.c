#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "double_linked_list.h"

int addElementDoubleLinkedList(struct doubleLinkedList* list, int value) {
    struct doubleLinkedListElement* thing =
        malloc(sizeof(doubleLinkedListElement));
    if (thing == NULL) {
        return INT_MIN;
    }
    thing->next = NULL;
    thing->previous = NULL;
    thing->data = value;
    if (list->first == NULL) {
        list->first = thing;
        list->last = thing;
        return value;
    }
    doubleLinkedListElement* prev;
    doubleLinkedListElement* next = list->first;
    // if you insert something smaller than than the first one
    if (next->data >= thing->data) {
        thing->next = next;
        next->previous = thing;
        list->first = thing;
        return value;
    }
    if (next->next == NULL) {
        thing->next = NULL;
        thing->previous = next;
        next->next = thing;
        list->last = thing;
        return value;
    }
    prev = next;
    next = next->next;

    while (next->next != NULL) {
        if (prev->data <= thing->data) {
            thing->next = next;
            thing->previous = prev;
            prev->next = thing;
            next->previous = thing;
            return value;
        }
        prev = next;
        next = next->next;
    }

    if (next->data > thing->data) {
        thing->next = next;
        thing->previous = prev;
        prev->next = thing;
        next->previous = thing;
        return value;
    }

    thing->next = NULL;
    thing->previous = next;
    next->next = thing;
    list->last = thing;
    return value;
}

void initDoubleLinkedList(struct doubleLinkedList* list) {
    list->first = NULL;
    list->last = NULL;
}

int removeFirstElementDoubleLinkedList(struct doubleLinkedList* list) {
    if (list->first == NULL || list->last == NULL) {
        return INT_MIN;
    }
    doubleLinkedListElement* thing = list->first;

    int returnval = thing->data;
    list->first = thing->next;
    if (thing->next != NULL) {
        thing->next->previous = NULL;
    }
    if (list->first == NULL) {
        list->last = NULL;
    }
    free(thing);
    return returnval;
}

int removeLastElementDoubleLinkedList(struct doubleLinkedList* list) {
    if (list->last == NULL || list->first == NULL) {
        return INT_MIN;
    }
    doubleLinkedListElement* thing = list->last;
    if (thing != NULL) {
    }
    int returnval = thing->data;
    list->last = thing->previous;
    if (list->last != NULL) {
        list->last->next = NULL;
    }
    if (list->last == NULL) {
        list->first = NULL;
    }
    free(thing);
    return returnval;
}
