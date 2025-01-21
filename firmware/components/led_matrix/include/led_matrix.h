#pragma once

#include "led_strip.h"

typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} led_data_t;

typedef struct
{
    led_strip_handle_t led_strip;
    led_data_t *data;
    uint32_t size;
} led_matrix_t;

esp_err_t led_matrix_init(led_matrix_t *led_matrix, uint8_t gpio_pin, uint32_t width, uint32_t height);
esp_err_t led_matrix_clear(led_matrix_t *led_matrix);
esp_err_t led_matrix_refresh(led_matrix_t *led_matrix);
esp_err_t led_matrix_update(led_matrix_t *led_matrix);
esp_err_t led_matrix_set_pixel(led_matrix_t *led_matrix, uint32_t index, led_data_t led_data);
led_data_t led_matrix_get_pixel(led_matrix_t *led_matrix, uint32_t index);
