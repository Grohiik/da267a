#include "random.h"

int getRandommsecs(int min, int max) {
    // 0 <= A%B > 
    return esp_random() % (max - min + 1) + min;
}