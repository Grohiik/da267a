#include "pins.h"
#include <esp_task_wdt.h>
#include "random.h"

#define ever                                                                   \
    ;                                                                          \
    ;

/*
 Waits for "millis" milliseconds without upsetting the watchdog timer
*/
void waitMs(unsigned int millis) {
    TickType_t delay = millis / portTICK_PERIOD_MS;
    vTaskDelay(delay);
}

void app_main() {
    initPins();
    for (ever) {
        int r = getRandommsecs(100, 500);
        printf("random value: %d\n", r);
        setLEDA(1);
        setLEDB(0);
        waitMs(r);
        setLEDA(0);
        setLEDB(1);
        waitMs(r);
    }
}
