#include "led_matrix.h"

#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "esp_err.h"

// GPIO assignment
#define LED_STRIP_GPIO_PIN 8
// 10MHz resolution, 1 tick = 0.1us (led strip needs a high resolution)
#define LED_STRIP_RMT_RES_HZ (10 * 1000 * 1000)

static const char *TAG = "led_matrix";

led_strip_handle_t led_strip_init(uint8_t gpio_pin, uint32_t max_leds)
{
    // LED strip general initialization, according to your LED board design
    led_strip_config_t strip_config = {
        .strip_gpio_num = gpio_pin,                                  // The GPIO that connected to the LED strip's data line
        .max_leds = max_leds,                                        // The number of LEDs in the strip,
        .led_model = LED_MODEL_WS2812,                               // LED strip model
        .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB, // The color order of the strip: GRB
        .flags = {
            .invert_out = false, // don't invert the output signal
        }};

    // LED strip backend configuration: RMT
    led_strip_rmt_config_t rmt_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,        // different clock source can lead to different power consumption
        .resolution_hz = LED_STRIP_RMT_RES_HZ, // RMT counter clock frequency
        .mem_block_symbols = 64,               // the memory size of each RMT channel, in words (4 bytes)
        .flags = {
            .with_dma = false, // DMA feature is available on chips like ESP32-S3/P4
        }};

    // LED Strip object handle
    led_strip_handle_t led_strip;
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
    ESP_LOGI(TAG, "Created LED strip object with RMT backend");
    return led_strip;
}

led_matrix_t led_matrix_init(uint8_t gpio_pin, uint32_t max_leds)
{
    led_matrix_t led_matrix =
        {
            .led_strip = led_strip_init(gpio_pin, max_leds),
        };
    return led_matrix;
}

esp_err_t led_matrix_clear(led_matrix_t led_matrix)
{
    return led_strip_clear(led_matrix.led_strip);
}

esp_err_t led_matrix_refresh(led_matrix_t led_matrix)
{
    return led_strip_refresh(led_matrix.led_strip);
}

esp_err_t led_matrix_set_pixel(led_matrix_t led_matrix, uint32_t index, uint32_t red, uint32_t green, uint32_t blue)
{
    return led_strip_set_pixel(led_matrix.led_strip, index, red, green, blue);
}
