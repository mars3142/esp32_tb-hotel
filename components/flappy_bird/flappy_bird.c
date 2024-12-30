#include "flappy_bird.h"

#include <esp_random.h>

#include "freertos/FreeRTOS.h"
#include "led_strip.h"
#include "esp_log.h"
#include "esp_err.h"
#include "time.h"

#include "led_matrix.h"

static const char *TAG = "flappy";

uint8_t poles[LED_STRIP_LED_COUNT];

void generate_poles() {
    for (int i = 0; i < LED_STRIP_LED_COUNT; i++) {
        const uint8_t min = 0;
        const uint8_t max = 7;
        const uint8_t value = min + (esp_random() % (max - min + 1));
        poles[i] = value;
    }
}

void vFlappyTask(void *pvParameters) {
    generate_poles();
    led_strip_handle_t led_strip = configure_led();
    bool led_on_off = false;

    ESP_LOGI(TAG, "Start blinking LED strip");
    while (true) {
        if (led_on_off) {
            uint32_t pos = 0;
            for (uint32_t i = 0; i < LED_STRIP_LED_COUNT; i++) {
                const uint8_t pole = poles[pos];
                if (pole == i % 8) {
                    ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, i, 5, 5, 5));
                    pos++;
                }
            }
            ESP_ERROR_CHECK(led_strip_refresh(led_strip));
            ESP_LOGI(TAG, "LED ON!");
        } else {
            ESP_ERROR_CHECK(led_strip_clear(led_strip));
            ESP_LOGI(TAG, "LED OFF!");
        }

        led_on_off = !led_on_off;
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
