/*#include <esp32/rom/ets_sys.h>
#include <Double_linked_list.h>
#include <unity.h>
struct doubleLinkedList* list2;

void setUp() {
    list2 = malloc(sizeof(struct doubleLinkedList));
    initDoubleLinkedList(list2);
}

void tearDown() {
    int blub = 1;
    while (blub) {
        blub = removeFirstElementDoubleLinkedList(list2);
    }
}

void add_test1() {
    addElementDoubleLinkedList(list2, 5);
    TEST_ASSERT_EQUAL(5, removeFirstElementDoubleLinkedList(list2));
}

void add_remove_test2() {
    addElementDoubleLinkedList(list2, 5);
    addElementDoubleLinkedList(list2, 2);
    TEST_ASSERT_EQUAL(5, removeLastElementDoubleLinkedList(list2));
    addElementDoubleLinkedList(list2, 10);
    TEST_ASSERT_EQUAL(2, removeFirstElementDoubleLinkedList(list2));
}

void remove_empty() {
    TEST_ASSERT_EQUAL(NULL, removeFirstElementDoubleLinkedList(list2));
    TEST_ASSERT_EQUAL(NULL, removeLastElementDoubleLinkedList(list2));
}

void app_main() {
    ets_delay_us(2000000ul);

    UNITY_BEGIN();

    RUN_TEST(add_test1);
    RUN_TEST(add_remove_test2);
    RUN_TEST(remove_empty);

    UNITY_END();
}*/