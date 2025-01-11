#include "ota_ble.h"

#include "esp_log.h"
#include "freertos/FreeRTOS.h"

void vOTATask(void *pvParameters)
{
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    ESP_LOGI("OTA", "OTA task finished");
    vTaskDelete(NULL);
}
