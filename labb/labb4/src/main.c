#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "single_linked_list.h"
#include "double_linked_list.h"

#define TEST_ASSERT(test, ...)                                                 \
    do {                                                                       \
        printf(__VA_ARGS__);                                                   \
        if (!(test)) {                                                         \
            printf("Assertion Failed: (%s), func %s, file %s, line %d\n",      \
                   #test, __FUNCTION__, __FILE__, __LINE__);                   \
        } else {                                                               \
            printf("\tPassed\n");                                              \
        }                                                                      \
    } while (0)
#define TEST_RUN(test)                                                         \
    do {                                                                       \
        printf("TEST: %s\n", #test);                                           \
        test();                                                                \
    } while (0)

singleLinkedList* list;
doubleLinkedList* dist;

void add_test1() {
    addElementSingleLinkedList(list, 1);
	TEST_ASSERT(1==list->first->data, "expected 1 got %d", list->first->data);
    int remove = removeFirstElementSingleLinkedList(list);
    TEST_ASSERT(1 == remove, "expected %d got %d\n", 1, remove);
}

void add_remove_test2() {
    addElementSingleLinkedList(list, 5);
    addElementSingleLinkedList(list, 2);
	addElementSingleLinkedList(list, 3);
    int remove = removeLastElementSingleLinkedList(list);
	TEST_ASSERT(3==list->first->next->data && 2==list->first->data, "expected 3 2, got %d, %d", list->first->next->data, list->first->data);
    TEST_ASSERT(5 == remove, "expected %d got %d\n", 5, remove);
    addElementSingleLinkedList(list, 10);
    remove = removeFirstElementSingleLinkedList(list);
    TEST_ASSERT(2 == remove, "expected %d got %d\n", 2, remove);
    removeFirstElementSingleLinkedList(list);
    removeFirstElementSingleLinkedList(list);
}

void remove_empty() {
    int remove = removeLastElementSingleLinkedList(list);
    TEST_ASSERT(INT_MIN == remove, "expected %d got %d\n", INT_MIN, remove);
    remove = removeFirstElementSingleLinkedList(list);
    TEST_ASSERT(INT_MIN == remove, "expected %d got %d\n", INT_MIN, remove);
}

//---------------------doublelinked---------------------------
void add_test1_D() {
    addElementDoubleLinkedList(dist, 1);
    int remove = removeFirstElementDoubleLinkedList(dist);
    TEST_ASSERT(1 == remove, "expected %d got %d\n", 1, remove);
}

void add_remove_test2_D() {
    addElementDoubleLinkedList(dist, 5);
    addElementDoubleLinkedList(dist, 2);
	addElementDoubleLinkedList(dist, 3);
    int remove = removeLastElementDoubleLinkedList(dist);
	TEST_ASSERT(3==dist->last->data && 2==dist->first->data, "expected 2 3, got %d, %d", dist->first->data, dist->last->data);
    TEST_ASSERT(5 == remove, "expected %d got %d\n", 5, remove);
    addElementDoubleLinkedList(dist, 10);
    remove = removeFirstElementDoubleLinkedList(dist);
    TEST_ASSERT(2 == remove, "expected %d got %d\n", 2, remove);
    removeFirstElementDoubleLinkedList(dist);
	removeFirstElementDoubleLinkedList(dist);
}

void remove_empty_D() {
    int remove = removeLastElementDoubleLinkedList(dist);
    TEST_ASSERT(INT_MIN == remove, "expected %d got %d\n", INT_MIN, remove);
    remove = removeFirstElementDoubleLinkedList(dist);
    TEST_ASSERT(INT_MIN == remove, "expected %d got %d\n", INT_MIN, remove);
}

void app_main() {
    printf("Doubly Linked List\n");
    printf("-------------------\n");
    list = malloc(sizeof(singleLinkedList));
    initSingleLinkedList(list);
    TEST_RUN(add_test1);
    TEST_RUN(add_remove_test2);
    TEST_RUN(remove_empty);

    printf("Doubly Linked List\n");
    printf("-------------------\n");
    dist = malloc(sizeof(doubleLinkedList));
    initDoubleLinkedList(dist);
    TEST_RUN(add_test1_D);
    TEST_RUN(add_remove_test2_D);
    TEST_RUN(remove_empty_D);
}