#pragma once

#include "led_strip.h"

typedef struct
{
    led_strip_handle_t led_strip;
} led_matrix_t;

led_matrix_t led_matrix_init(uint8_t gpio_pin, uint32_t max_leds);

esp_err_t led_matrix_clear(led_matrix_t led_matrix);
esp_err_t led_matrix_refresh(led_matrix_t led_matrix);
esp_err_t led_matrix_set_pixel(led_matrix_t led_matrix, uint32_t index, uint32_t red, uint32_t green, uint32_t blue);
