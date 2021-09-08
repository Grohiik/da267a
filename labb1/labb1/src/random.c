#include "random.h"

int getRandommsecs(int min, int max) {
    int random = esp_random()%max;
    while((random) < min) {
        random = esp_random()%max;
    }
    return random;
}