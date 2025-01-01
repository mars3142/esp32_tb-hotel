#include "freertos/FreeRTOS.h"
#include "flappy_bird.h"

void app_main()
{
    xTaskCreate(vFlappyBirdTask, "flappy", 4048, NULL, tskIDLE_PRIORITY + 1, NULL);
}
