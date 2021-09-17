#include <esp32/rom/ets_sys.h>
#include <circular_buffer.h>
#include <unity.h>

#define BUFFER_SIZE 10
struct circularBuffer buffer;
int *buffer_data;

void test_add_remove(void) {
    addElement(&buffer, 4);
    TEST_ASSERT_EQUAL(4, removeHead(&buffer));
}

void test_remove_order() {
    addElement(&buffer, 3);
    addElement(&buffer, 5);
    int val1 = removeHead(&buffer);
    int val2 = removeHead(&buffer);
    TEST_ASSERT_TRUE(val1 == 3 && val2 == 5);
}

void test_fill_check_order() {
    int expected[BUFFER_SIZE-1];
    for(int i = 0; i < BUFFER_SIZE-1; i++) {
        addElement(&buffer, i);
        expected[i] = i;
    }
    int vals[BUFFER_SIZE-1];
    for(int i = 0; i < BUFFER_SIZE-1; i++) {
        vals[i] = removeHead(&buffer);
    }
    TEST_ASSERT_EQUAL_MEMORY(&expected, &vals, sizeof(expected));
}

void test_overfill() {
    for(int i = 0; i < BUFFER_SIZE; i++) {
        addElement(&buffer, i);
    }
    TEST_ASSERT_TRUE(addElement(&buffer, 10) == INT_MIN && removeHead(&buffer) == 0);
}

void test_fill_and_empty() {
    for(int i = 0; i < BUFFER_SIZE; i++) {
        addElement(&buffer, 4);
        removeHead(&buffer);
    }
    addElement(&buffer, 4);
    TEST_ASSERT_EQUAL(4, removeHead(&buffer));
}

void test_empty_contains() {
    TEST_ASSERT_EQUAL(INT_MIN, contains(&buffer, 5));
}

void test_contains() {
    addElement(&buffer, 5);
    TEST_ASSERT_EQUAL(5, contains(&buffer, 5));
}

void test_contains_second() {
    addElement(&buffer, 1);
    addElement(&buffer, 2);
    TEST_ASSERT_EQUAL(2, contains(&buffer, 2));
}

void test_fill_contains_last() {
    for(int i = 0; i < BUFFER_SIZE; i++) {
        addElement(&buffer, i);
    }
    TEST_ASSERT_EQUAL(BUFFER_SIZE-2, contains(&buffer, BUFFER_SIZE-2));
}

void white_init() {
    TEST_ASSERT_TRUE(buffer.maxLength == BUFFER_SIZE && buffer.head == 0 && buffer.tail == 0);
}

void white_addelement() {
    addElement(&buffer, 5);
    TEST_ASSERT_TRUE(buffer.data[0] == 5);
}
void white_addelement2(){
    buffer.tail = BUFFER_SIZE-2;
    addElement(&buffer, 5);
    TEST_ASSERT_TRUE(buffer.data[BUFFER_SIZE-2] == 5);
}
void white_addelement3(){
    buffer.head = BUFFER_SIZE-1;
    buffer.tail = BUFFER_SIZE-1;
    addElement(&buffer, 5);
    addElement(&buffer, 10);
    TEST_ASSERT_TRUE(buffer.data[0] == 10 && buffer.tail == 1);
}
void white_addelement4(){
    buffer.head = 0;
    buffer.tail = BUFFER_SIZE-1;
    for(int i = 0; i < BUFFER_SIZE-1; i++)
    {
        buffer.data[i] = i;   
    }
    addElement(&buffer, 15);
    TEST_ASSERT_TRUE(buffer.head == 0 && buffer.tail == BUFFER_SIZE-1 && buffer.data[BUFFER_SIZE-2] == BUFFER_SIZE-2);
}
void test_remove_one() {
    addElement(&buffer, 5);
    removeHead(&buffer);
    TEST_ASSERT_EQUAL(buffer.head, buffer.tail);
}

void test_white_remove_val() {
    buffer.head = BUFFER_SIZE-3;
    buffer.tail = BUFFER_SIZE-3;
    addElement(&buffer, 1);
    addElement(&buffer, 3);
    addElement(&buffer, 1);
    addElement(&buffer, 5);
    removeValue(&buffer, 1);

    TEST_ASSERT_EQUAL(BUFFER_SIZE-3, buffer.head);
    TEST_ASSERT_EQUAL(BUFFER_SIZE-1, buffer.tail);
    TEST_ASSERT_EQUAL(3, buffer.data[BUFFER_SIZE-3]);
    TEST_ASSERT_EQUAL(5, buffer.data[BUFFER_SIZE-2]);
}

void test_white_remove_val_empty() {
    TEST_ASSERT_EQUAL(INT_MIN, removeValue(&buffer, 10));
}

void test_white_removeHead1() {
    addElement(&buffer, 5);
    TEST_ASSERT_EQUAL(5, removeHead(&buffer));
    TEST_ASSERT_EQUAL(1, buffer.head);
    TEST_ASSERT_EQUAL(1, buffer.tail);
}

void test_white_removeHead2() {
    buffer.head=BUFFER_SIZE-2;
    buffer.tail=BUFFER_SIZE-2;
    addElement(&buffer, 3);
    addElement(&buffer, 5);
    TEST_ASSERT_EQUAL(3, removeHead(&buffer));
    TEST_ASSERT_EQUAL(0, buffer.tail);
    TEST_ASSERT_EQUAL(BUFFER_SIZE-1, buffer.head);
}

void test_white_removeHead3() {
    buffer.head=BUFFER_SIZE-2;
    buffer.tail=BUFFER_SIZE-2;
    addElement(&buffer, 10);
    addElement(&buffer, 5);
    addElement(&buffer, 3);
    TEST_ASSERT_EQUAL(10, removeHead(&buffer));
    TEST_ASSERT_EQUAL(1, buffer.tail);
    TEST_ASSERT_EQUAL(BUFFER_SIZE-1, buffer.head);
}

void test_white_removeHead4() {
    TEST_ASSERT_EQUAL(INT_MIN, removeHead(&buffer));
}

void setUp() {
    // Some code to help you get started
    buffer_data = (int *)malloc(BUFFER_SIZE * sizeof(int));
    initCircularBuffer(&buffer, buffer_data, BUFFER_SIZE);
}
void tearDown() {
    free(buffer_data);
}

void app_main() {
    ets_delay_us(2000000ul);

    UNITY_BEGIN();

    RUN_TEST(test_add_remove);
    RUN_TEST(test_remove_order);
    RUN_TEST(test_fill_check_order);
    RUN_TEST(test_overfill);
    RUN_TEST(test_fill_and_empty);
    RUN_TEST(test_empty_contains);
    RUN_TEST(test_contains);
    RUN_TEST(test_contains_second);
    RUN_TEST(test_fill_contains_last);

    RUN_TEST(white_init);
    RUN_TEST(white_addelement);
    RUN_TEST(white_addelement2);
    RUN_TEST(white_addelement3);
    RUN_TEST(white_addelement4);
    RUN_TEST(test_remove_one);
    RUN_TEST(test_white_remove_val);
    RUN_TEST(test_white_remove_val_empty);
    RUN_TEST(test_white_removeHead1);
    RUN_TEST(test_white_removeHead2);
    RUN_TEST(test_white_removeHead3);
    RUN_TEST(test_white_removeHead4);

    UNITY_END();
}