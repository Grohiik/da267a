#include <driver/i2c.h>

#ifndef accel
#define accel

void init();

int16_t readaccel(uint16_t registerHigh, uint16_t registerLow);

u_int32_t getaccelmagnitude();

#endif