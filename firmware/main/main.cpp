#include "freertos/FreeRTOS.h"
#include "flappy_bird.h"
#include "ota_ble.h"
#include "persistance.h"

extern "C"
{
    void app_main()
    {
        ESP_ERROR_CHECK(persistance_init());

        xTaskCreate(vFlappyBirdTask, "flappy", 4048, NULL, tskIDLE_PRIORITY + 1, NULL);
        xTaskCreate(vOTATask, "ota", 4048, NULL, tskIDLE_PRIORITY + 1, NULL);
    }
}