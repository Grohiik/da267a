#include <esp_task_wdt.h>
#include "pins.h"

#define ever ;;


/*
 Waits for "millis" milliseconds without upsetting the watchdog timer
*/
void waitMs(unsigned int millis)
{
    TickType_t delay = millis / portTICK_PERIOD_MS;
    vTaskDelay(delay);
}

void app_main()
{
    initPins();
    for (ever)
    {
        if (isButtonAPressed())
        {
            setLEDA(1);
            setLEDB(0);
        }
        else if (isButtonBPressed())
        {
            setLEDB(1);
            setLEDA(0);
        }
        else
        {
            setLEDA(1);
            setLEDB(0);
            waitMs(500);
            setLEDA(0);
            setLEDB(1);
            waitMs(500);
        }
    }
}
