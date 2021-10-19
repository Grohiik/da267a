#include <driver/i2c.h>
#include <esp_sleep.h>
#include <esp_task_wdt.h>
#include "readAccel.h"
#include <esp_pm.h>
#include <math.h>
#include "circular_buffer.h"
#include <stdio.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

#define LED_PIN    26
#define BUTTON_PIN 14
SemaphoreHandle_t xSemaphore = NULL;
#define STEPS_GOAL 10
// acuracy of a bit over 50%

// the reason why I have chosen this sampling frequency is because you run
// roughly 170 steps/min so 300 will allow for over 3 steps a second or 180 per
// min but I want to have over double that to notice the periodity, I chose 100
// to really make sure it is enough without going WAY over board
#define SAMPLING_PERIOD 100
// I have chosen to run the algorithm every YYY because for the algorythm to
// work nicelly i need a few periods, 15 seemed good
#define ALGO_PERIOD 4500
// number of samples to he held inside the buffer: 50 this is because i store
// data every 100Ms and empty it every 4500Ms, 4500/100 = 45. that is only 32*45
// bits or like 1,5Kbs. my board has 520KB of SRAM so it should fit well even
// with some extra space
#define BUFF_SIZE 50
// minimum SD to avoid converging to 0
#define MIN_SD 500
// constant applied to SD to detect steps
#define K 1.1
// minimum time between steps, this value is chosen because... I the min steping
// delay i'm expecting is 300MS ish
#define MIN_INTRA_STEP_TIME 300

struct circularBuffer buffer;
int step_count = 0;

static void sampling_task(void *arg) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while (1) {
        // get the acceleration
        // compute the magnitude
        u_int32_t temp = getaccelmagnitude();
        // place the magnitude into the buffer
        addElement(&buffer, temp);
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(SAMPLING_PERIOD));
    }
}

static void algo_task(void *arg) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while (1) {
        u_int32_t size = getsize(&buffer);
        u_int32_t tail = buffer.tail;
        // get size of the buffer
        if (size > 0) {
            // compute mean, here do NOT empty the queue when reading it!
            double mean = 0;
            for (int i = buffer.head; i != tail; i = inc(buffer.maxLength, i)) {
                mean += buffer.data[i];
            }
            mean = mean / (double)size;
            // compute SD, here queue must not be emptied yet
            double sd = 0;
            for (int i = buffer.head; i != tail; i = inc(buffer.maxLength, i)) {
                sd += pow((buffer.data[i] - mean), 2);
            }
            sd = sqrt(sd / (double)size);
            if (sd < MIN_SD) sd = MIN_SD;
            // now do the step counting, while also emptying the queue
            u_int32_t sample;
            u_int32_t LastStep = MIN_INTRA_STEP_TIME;
            for (int i = 0; i < size; i++) {
                // get sample, removing it from queue
                sample = removeHead(&buffer);
                // if sample > mean + K * sd
                if (sample > mean + K * sd) {
                    if (LastStep + i * SAMPLING_PERIOD - LastStep >
                        MIN_INTRA_STEP_TIME) {
                        // AND if time between last step and this sample is >
                        // MIN_INTRA_STEP_TIME step found! step_count++;

                        step_count++;
                        LastStep += i * SAMPLING_PERIOD;
                    }
                }
            }
        }
        printf("step counter: %d\n", step_count);
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(ALGO_PERIOD));
    }
}

// button pressed ISR
void button_isr_handler(void *arg) {
    // give semaphore (pay attention: this is in an ISR!)
    xSemaphoreGiveFromISR(xSemaphore, NULL);
}

void led_task(void *arg) {
    while (1) {
        // wait for semaphore
        if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
            // flash LED with sequence depending on if step_count > STEPS_GOAL
            if (step_count > STEPS_GOAL) {
                gpio_set_level(LED_PIN, 1);
                vTaskDelay(pdMS_TO_TICKS(500));
                gpio_set_level(LED_PIN, 0);
                vTaskDelay(pdMS_TO_TICKS(500));
                gpio_set_level(LED_PIN, 1);
                vTaskDelay(pdMS_TO_TICKS(500));
                gpio_set_level(LED_PIN, 0);
            }
        }
    }
}

void app_main() {  // configure light sleep mode with esp_pm_configure()
    esp_pm_config_esp32_t MYsleep_config = {
        .max_freq_mhz = 80,
        .min_freq_mhz = 20,
        .light_sleep_enable = 1,
    };
    esp_err_t error = esp_pm_configure(&MYsleep_config);
    ESP_ERROR_CHECK(error);

    // button and led init
    // create the binary semaphore
    xSemaphore = xSemaphoreCreateBinary();
    // start the task that will handle the button
    xTaskCreate(led_task, "led_task", 2048, NULL, 10, NULL);
    // configure button and led pins
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    // setup the btn with an internal pullup
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);
    // enable interrupt on falling (1->0) edge for button pin
    gpio_set_intr_type(BUTTON_PIN, GPIO_INTR_NEGEDGE);
    // install ISR service with default configuration
    gpio_install_isr_service(0);
    // attach the interrupt service routine
    gpio_isr_handler_add(BUTTON_PIN, button_isr_handler, NULL);

    // initialise the buffer
    u_int32_t *data = (uint32_t *)malloc(BUFF_SIZE * sizeof(uint32_t));
    initCircularBuffer(&buffer, data, BUFF_SIZE);

    // initialise the I2C bus and the MPU6050
    init();
    // create sampling task
    xTaskCreate(sampling_task, "sampling", 2048, NULL, 1, NULL);
    xTaskCreate(algo_task, "algo", 2048, NULL, 0, NULL);
}