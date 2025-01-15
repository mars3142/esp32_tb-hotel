#include "led_matrix.h"

#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "esp_err.h"

#define LED_STRIP_GPIO_PIN 8
#define LED_STRIP_RMT_RES_HZ (10 * 1000 * 1000)

static const char *TAG = "led_matrix";

led_strip_handle_t led_strip_init(uint8_t gpio_pin, uint32_t width, uint32_t height)
{
    led_strip_config_t strip_config = {
        .strip_gpio_num = gpio_pin,
        .max_leds = width * height,
        .led_model = LED_MODEL_WS2812,
        .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB,
        .flags = {
            .invert_out = false,
        }};

    led_strip_rmt_config_t rmt_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = LED_STRIP_RMT_RES_HZ,
        .mem_block_symbols = 64,
        .flags = {
            .with_dma = false,
        }};

    led_strip_handle_t led_strip;
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
    return led_strip;
}

esp_err_t led_matrix_init(led_matrix_t *led_matrix, uint8_t gpio_pin, uint32_t width, uint32_t height)
{
    led_matrix->led_strip = led_strip_init(gpio_pin, width, height);
    led_matrix->size = width * height;
    led_matrix->data = malloc(led_matrix->size * sizeof(uint8_t[3]));

    if (led_matrix->data == NULL)
    {
        ESP_LOGE(TAG, "Failed to allocate memory for LED matrix data");
        return ESP_ERR_NO_MEM;
    }

    for (uint32_t i = 0; i < led_matrix->size; i++)
    {
        led_data_t led_data = {
            .red = 0,
            .green = 0,
            .blue = 0,
        };
        led_matrix->data[i] = led_data;
    }

    ESP_LOGI(TAG, "LED matrix initialized");
    return ESP_OK;
}

esp_err_t led_matrix_clear(led_matrix_t *led_matrix)
{
    return led_strip_clear(led_matrix->led_strip);
}

esp_err_t led_matrix_refresh(led_matrix_t *led_matrix)
{
    return led_strip_refresh(led_matrix->led_strip);
}

esp_err_t led_matrix_update(led_matrix_t *led_matrix)
{
    for (uint32_t i = 0; i < led_matrix->size; i++)
    {
        led_data_t led_data = led_matrix->data[i];
        led_strip_set_pixel(led_matrix->led_strip, i, led_data.red, led_data.green, led_data.blue);
    }
    return led_strip_refresh(led_matrix->led_strip);
}

esp_err_t led_matrix_set_pixel(led_matrix_t *led_matrix, uint32_t index, uint8_t red, uint8_t green, uint8_t blue)
{
    if (index >= led_matrix->size)
    {
        // ESP_LOGE(TAG, "Index %" PRIu32 " out of bounds %" PRIu32 "", index, led_matrix->size);
        return ESP_OK;
    }
    led_data_t led_data = {
        .red = red,
        .green = green,
        .blue = blue,
    };
    led_matrix->data[index] = led_data;

    return ESP_OK;
}
