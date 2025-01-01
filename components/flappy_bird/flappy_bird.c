#include "flappy_bird.h"

#include <freertos/FreeRTOS.h>
#include <esp_random.h>
#include <esp_log.h>
#include <esp_err.h>

#include "led_matrix.h"

static const char *TAG = "flappy";

#define LED_STRIP_LED_COUNT (8 * 8 * 4)

uint8_t walls[LED_STRIP_LED_COUNT] = {-1};

void generate_holes()
{
    for (int i = 0; i < LED_STRIP_LED_COUNT; i++)
    {
        const uint8_t min = 0;
        const uint8_t max = 7;
        const uint8_t value = min + (esp_random() % (max - min + 1));
        walls[i] = value;
    }
}

void vFlappyBirdTask(void *pvParameters)
{
    generate_holes();
    led_matrix_t led_matrix = led_matrix_init(8, LED_STRIP_LED_COUNT);
    bool led_on_off = false;

    ESP_LOGI(TAG, "Start blinking LED strip");
    while (true)
    {
        if (led_on_off)
        {
            uint32_t pos = 0;
            for (uint32_t i = 0; i < LED_STRIP_LED_COUNT; i++)
            {
                const uint8_t pole = walls[pos];
                if (pole != i % 8)
                {
                    ESP_ERROR_CHECK(led_matrix_set_pixel(led_matrix, i, 0, 0, 0));
                }
                else
                {
                    ESP_ERROR_CHECK(led_matrix_set_pixel(led_matrix, i, 5, 5, 5));
                    pos++;
                }
            }
            ESP_ERROR_CHECK(led_matrix_refresh(led_matrix));
            ESP_LOGI(TAG, "LED ON!");
        }
        else
        {
            ESP_ERROR_CHECK(led_matrix_clear(led_matrix));
            ESP_LOGI(TAG, "LED OFF!");
        }

        led_on_off = !led_on_off;
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
