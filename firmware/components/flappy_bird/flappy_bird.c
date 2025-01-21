#include "flappy_bird.h"

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <esp_random.h>
#include <esp_log.h>
#include <esp_err.h>
#include <driver/gpio.h>

#include "sdkconfig.h"
#include "button.h"
#include "led_matrix.h"
#include "persistance.h"
#include "state_machine.h"

static const char *TAG = "flappy";

#define LED_STRIP_LED_COUNT (64 * 8 * 2)

uint8_t walls[LED_STRIP_LED_COUNT];

button_t button;
int start = 0;
int delay = 500;
const int bird_start_section = 1;
volatile int bird = bird_start_section * 8 + 2;

void move_bird(void *arg)
{
    bird -= 3;
}

void button_click_medium_cb(void *arg)
{
    delay += 250;
}

void button_click_long_cb(void *arg)
{
    delay = 250;
}

void generate_parkour()
{
    for (uint32_t i = 0; i < LED_STRIP_LED_COUNT; i++)
    {
        walls[i] = 1;
    }

    int index = 0;
    for (uint32_t i = 0; i < (LED_STRIP_LED_COUNT / 8); i++)
    {
        const uint8_t min = 2;
        const uint8_t max = 5;
        const uint8_t value = min + (esp_random() % (max - min + 1));
        walls[index++] = value == 0;
        walls[index++] = value == 1 || value == 2;
        walls[index++] = value == 2 || value == 3;
        walls[index++] = value == 3 || value == 2 || value == 4;
        walls[index++] = value == 4 || value == 3 || value == 5;
        walls[index++] = value == 5 || value == 4;
        walls[index++] = value == 6 || value == 5;
        walls[index++] = value == 7;
        if (index >= LED_STRIP_LED_COUNT)
        {
            break;
        }
        index += 8 * 5;
    }
}

void init_hardware(led_matrix_t *led_matrix)
{
    ESP_LOGI(TAG, "configuring led matrix");
    ESP_ERROR_CHECK(led_matrix_init(led_matrix, CONFIG_GEO_WLED_DIN_PIN, 64, 8));

    ESP_LOGI(TAG, "configuring button");
    ESP_ERROR_CHECK(button_init(&button, CONFIG_GEO_BTN_PIN, BUTTON_EDGE_FALLING, tskIDLE_PRIORITY + 2, configMINIMAL_STACK_SIZE * 4));
    ESP_ERROR_CHECK(button_add_cb(&button, BUTTON_CLICK_SINGLE, move_bird, NULL));
    ESP_ERROR_CHECK(button_add_cb(&button, BUTTON_CLICK_DOUBLE, move_bird, NULL));
    ESP_ERROR_CHECK(button_add_cb(&button, BUTTON_CLICK_MEDIUM, button_click_medium_cb, NULL));
    ESP_ERROR_CHECK(button_add_cb(&button, BUTTON_CLICK_LONG, button_click_long_cb, NULL));

    ESP_LOGI(TAG, "config complete");
}

void init_game(game_state_machine_t *game)
{
    game->current_state = INTRO;
    game->score = 0;
    game->max_score = get_highscore();
}

int clamp(int value, int min, int max)
{
    if (value < min)
    {
        return min;
    }
    if (value > max)
    {
        return max;
    }
    return value;
}

static inline bool led_data_equals(led_data_t a, led_data_t b)
{
    return (a.red == b.red) && (a.green == b.green) && (a.blue == b.blue);
}

void vFlappyBirdTask(void *pvParameters)
{
    led_matrix_t led_matrix;
    init_hardware(&led_matrix);

    game_state_machine_t game;
    init_game(&game);
    generate_parkour();

    led_data_t led_data_empty = {
        .red = 0,
        .green = 0,
        .blue = 0,
    };
    led_data_t led_data_wall = {
        .red = 20,
        .green = 20,
        .blue = 20,
    };
    led_data_t led_data_bird = {
        .red = 0,
        .green = 255,
        .blue = 0,
    };
    led_data_t led_data_bird_death = {
        .red = 255,
        .green = 0,
        .blue = 0,
    };
    while (true)
    {
        for (uint32_t i = 0; i < LED_STRIP_LED_COUNT; i++)
        {
            if (walls[i + start] == 0)
            {
                ESP_ERROR_CHECK(led_matrix_set_pixel(&led_matrix, i, led_data_wall));
            }
            else
            {
                ESP_ERROR_CHECK(led_matrix_set_pixel(&led_matrix, i, led_data_empty));
            }
        }

        if (led_data_equals(led_matrix_get_pixel(&led_matrix, bird), led_data_wall))
        {
            ESP_ERROR_CHECK(led_matrix_set_pixel(&led_matrix, bird, led_data_bird_death));
        }
        else
        {
            ESP_ERROR_CHECK(led_matrix_set_pixel(&led_matrix, bird, led_data_bird));
            start += 8;
            bird = clamp(++bird, bird_start_section * 8, bird_start_section * 8 + 7);
        }
        ESP_ERROR_CHECK(led_matrix_update(&led_matrix));

        vTaskDelay(pdMS_TO_TICKS(delay));
    }
}
