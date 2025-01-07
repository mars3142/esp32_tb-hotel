#include "ota.h"

#include "freertos/FreeRTOS.h"

void vOTATask(void *pvParameters)
{
    while (true)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
