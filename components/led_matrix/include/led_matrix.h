#pragma once

#include "led_strip.h"

// GPIO assignment
#define LED_STRIP_GPIO_PIN  8
// Numbers of the LED in the strip
#define LED_STRIP_LED_COUNT (8 * 8 * 4)
// 10MHz resolution, 1 tick = 0.1us (led strip needs a high resolution)
#define LED_STRIP_RMT_RES_HZ  (10 * 1000 * 1000)

led_strip_handle_t configure_led();
