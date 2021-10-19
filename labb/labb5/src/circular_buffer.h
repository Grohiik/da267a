#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H


/* 
 * Data structure used to hold a circular buffer.
 */
struct circularBuffer{
  u_int32_t * data;
  int head;
  int tail;
  int maxLength;
  u_int32_t size;
};

int inc(int maxLength, int val);

/*
 * Initialize an empty buffer.
 */
void initCircularBuffer(struct circularBuffer* bufferPtr, uint32_t* data, int maxLen);


/* 
 * This function should check if the buffer pointed to by bufferPtr
 * contains one or more elements with the value specified by the 
 * 'value' argument.
 *
 * The function should return:
 *  - 'value' if the an element with the argument value was found in the queue.
 *  - INT_MIN (defined in limits.h) if value was not found.
 */ 
u_int32_t contains(struct circularBuffer* bufferPtr, u_int32_t value);

/*
 * This function should add the value specified by the 'value' 
 * argument at the tail of the buffer.
 *
 * The function should return:
 *  - 'value' if the value was successfully added to the queue.
 *  - INT_MIN (defined in limits.h) if the value was not added.
 */
u_int32_t addElement(struct circularBuffer* bufferPtr, u_int32_t value);



/* 
 * This function should remove all elements in the buffer whose
 * value matches the 'value' argument.
 * 
 * The function should return:
 *  The argument value: if at least one element was removed from queue.
 *  INT_MIN (defined in limits.h) if no element was removed. 
 */
u_int32_t removeValue(struct circularBuffer* bufferPtr, u_int32_t value);


/* 
 * Remove the oldest element, which is at the head of the queue. 
 * 
 * The function should return:
 *   - 'value' if the head element was successfully removed
 *   - INT_MIN (defined in limits.h) if no element was removed (i.e., the
 *     queue was empty when the function was called.       
 */
u_int32_t removeHead(struct circularBuffer* bufferPtr);

u_int32_t getsize(struct circularBuffer* bufferPtr);
/* 
 * Print the elements in the buffer from head to tail. 
 */
void printBuffer(struct circularBuffer* bufferPtr);

#endif

