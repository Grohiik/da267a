#include <stdio.h>
#include <stdlib.h>
#include <esp32/rom/ets_sys.h>
#include <single_linked_list.h>
#include <unity.h>

singleLinkedList* list;

void setUp() {
    initSingleLinkedList(list);
}

void tearDown() {
    int blub = 1;
    while (blub != INT_MIN) {
        blub = removeFirstElementSingleLinkedList(list);
    }
}

void add_test1() {
    addElementSingleLinkedList(list, 5);
    TEST_ASSERT_EQUAL(5, removeFirstElementSingleLinkedList(list));
}

void add_remove_test2() {
    addelementsinglelinkedlist(list, 5);
    addelementsinglelinkedlist(list, 2);
    test_assert_equal(5, removelastelementsinglelinkedlist(list));
    addelementsinglelinkedlist(list, 10);
    test_assert_equal(2, removefirstelementsinglelinkedlist(list));
}

void remove_empty() {
    TEST_ASSERT_EQUAL(INT_MIN, removeFirstElementSingleLinkedList(list));
    TEST_ASSERT_EQUAL(INT_MIN, removeLastElementSingleLinkedList(list));
}

void app_main() {
    ets_delay_us(2000000ul);
    list = malloc(sizeof(singleLinkedList));

    UNITY_BEGIN();

    RUN_TEST(add_test1);
    RUN_TEST(add_remove_test2);
    RUN_TEST(remove_empty);

    UNITY_END();
}