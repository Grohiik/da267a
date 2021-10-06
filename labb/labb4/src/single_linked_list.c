#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "single_linked_list.h"

int addElementSingleLinkedList(singleLinkedList* list, int value) {
    struct singleLinkedListElement* thing =
        malloc(sizeof(singleLinkedListElement));
    if (thing == NULL) {
        return INT_MIN;
    }
    thing->data = value;
    if (list->first == NULL) {
        thing->next = NULL;
        list->first = thing;
        return value;
    }
    singleLinkedListElement* prev;
    singleLinkedListElement* next = list->first;
    // if you insert something smaller than than the first one
    if (next->data >= thing->data) {
        thing->next = next;
        list->first = thing;
        return value;
    }
    if (next->next == NULL) {
        thing->next = NULL;
        next->next = thing;
        return value;
    }
    prev = next;
    next = next->next;

    while (next->next != NULL) {
        if (prev->data <= thing->data) {
            printf("%d  %d\n", prev->data, next->data);
            thing->next = next;
            prev->next = thing;
            return value;
        }
        prev = next;
        next = next->next;
    }
    if (next->data > thing->data) {
        thing->next = next;
        prev->next = thing;
        return value;
    }

    thing->next = NULL;
    next->next = thing;
    return value;
}

void initSingleLinkedList(singleLinkedList* list) { list->first = NULL; }

int removeFirstElementSingleLinkedList(singleLinkedList* list) {
    if (list->first == NULL) {
        return INT_MIN;
    }
    singleLinkedListElement* thing = list->first;
    int returnval = thing->data;
    list->first = thing->next;
    free(thing);
    return returnval;
}

int removeLastElementSingleLinkedList(singleLinkedList* list) {
    if (list->first == NULL) {
        return INT_MIN;
    }
    singleLinkedListElement* thing = list->first;
    singleLinkedListElement* prev = thing;

    while (thing->next != NULL) {
        prev = thing;
        thing = thing->next;
    }
    int returnval = thing->data;
    if (thing == list->first) {
        list->first = NULL;
        return returnval;
    }
    prev->next = NULL;
    free(thing);
    return returnval;
}
