#include "freertos/FreeRTOS.h"
#include "flappy_bird.h"

void app_main() {
    vTaskPrioritySet(NULL, tskIDLE_PRIORITY);
    xTaskCreate(vFlappyTask, "flappy", 4048, NULL, tskIDLE_PRIORITY + 1, NULL);
}
