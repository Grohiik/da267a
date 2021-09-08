#ifndef RANDOM_H
#define RANDOM_H
#include <esp_system.h>
/* Returns a random number contained between min and max in ms.
 min: minimum number of ms
 max: maximum number of ms
*/
int getRandom(int min, int max);

#endif