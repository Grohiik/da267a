#include <esp_timer.h>
#include <soc/adc_channel.h>
#include <driver/adc.h>
#include <esp_task_wdt.h>
#include "sampler.h"

#define SAMPLE_SIZE 10

volatile int going_up = 1;
volatile int last_sample = 20000000;
volatile int counter;
int time_at_start;
int time_at_end;
esp_timer_handle_t timer_handler;

void callback(void* args) {
    int adc = adc1_get_raw(ADC1_CHANNEL_6);
    if (going_up) {
        if (adc < last_sample) {
            going_up = 0;
            counter++;
        }
    } else {
        if (adc > last_sample) {
            going_up = 1;
            counter++;
        }
    }
    last_sample = adc;
}
//writen with help from pratchaya
void startSampling(int freq) {
    counter = 0;
    adc_power_acquire();
    adc_gpio_init(ADC_UNIT_1, (adc_channel_t)ADC1_GPIO34_CHANNEL);
    adc1_config_width(ADC_WIDTH_12Bit);
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);

    esp_timer_create_args_t periodic_timer_args = {
        .callback = callback,
        .arg = NULL,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "sampel"};
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &timer_handler));
    ESP_ERROR_CHECK(esp_timer_start_periodic(
        timer_handler, (uint64_t)((1.0 / (float)freq) * 1000000)));

    time_at_start = esp_timer_get_time();

}

void stopSampling() {
    ESP_ERROR_CHECK(esp_timer_stop(timer_handler));
    time_at_end = esp_timer_get_time();
}


int getFreq() {
    float time2 = 2*(float)((time_at_end-time_at_start)/1000000.0);
    printf("%f, %d\n", time2, counter);
    return (int)(counter/time2);
}