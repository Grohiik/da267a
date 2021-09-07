#include "pins.h"
#include "driver/gpio.h"
/* initialises the 4 pins */
void initPins()
{
    // init the 2 LEDs pins as output and the 2 buttons' pins as input
    // you will need to use gpio_config()
}

/* switches LED A on if level!=0 or off if level==0*/
void setLEDA(uint8_t level)
{
    if (level)
    {
        // set the pin of LED A to ON
        // you probably need to use gpio_set_level()
    }
    else
    {
        // set the pin of LED A to OFF
    }
}

/* switches LED B on if level!=0 or off if level==0*/
void setLEDB(uint8_t level)
{
    // same as setLEDA()
}

/* tells if button A is currently being pressed */
uint8_t isButtonAPressed()
{
    // read the value of button A
    // if using a pull-up, the button is pressed when the pin is LOW
    return 0; // return 1 if A is pressed, 0 otherwise
}

/* tells if button A is currently being pressed */
uint8_t isButtonBPressed()
{
    // same as with button A, but another pin
    return 0;
}