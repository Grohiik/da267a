#include <stdio.h>
#include <stdlib.h>
#include <esp32/rom/ets_sys.h>
#include <circular_buffer.h>

/*
 * Change the value of BUFFER_SIZE if you want to change the size of the buffer. 
 */ 
#define BUFFER_SIZE 4

void app_main() {

    // Some code to help you get started
    struct circularBuffer buffer;
    int *buffer_data = (int*) malloc(BUFFER_SIZE * sizeof(int));
    initCircularBuffer(&buffer, buffer_data, BUFFER_SIZE);


    printf("\n\n\n");
    printf("For example, output the results of your tests here.");
    
    printf("\n\n");   

    printf("\nPerhaps with some delay...");

    printf("\n\n\n");

    ets_delay_us(2000000ul);
    printf("\nbetween your printouts!");


    printf("\n\n\n");

    free(buffer_data);
}