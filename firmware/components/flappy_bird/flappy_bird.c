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

#define LED_STRIP_LED_COUNT (64 * 8 * 5)

uint8_t walls[LED_STRIP_LED_COUNT] = {-1};

button_t button;
int start = 0;
int delay = 250;

void button_click_single_cb(void *arg)
{
    ESP_LOGI(TAG, "Button 1 single click");
}

void button_click_medium_cb(void *arg)
{
    delay += 250;
}

void button_click_double_cb(void *arg)
{
    delay = 250;
}

void generate_parkour()
{
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
    }
}

void init_hardware(led_matrix_t *led_matrix)
{
    ESP_LOGI(TAG, "configuring led matrix");
    ESP_ERROR_CHECK(led_matrix_init(led_matrix, CONFIG_GEO_WLED_DIN_PIN, 64, 8));

    ESP_LOGI(TAG, "configuring button");
    ESP_ERROR_CHECK(button_init(&button, CONFIG_GEO_BTN_PIN, BUTTON_EDGE_FALLING, tskIDLE_PRIORITY + 2, configMINIMAL_STACK_SIZE * 4));
    ESP_ERROR_CHECK(button_add_cb(&button, BUTTON_CLICK_SINGLE, button_click_single_cb, NULL));
    ESP_ERROR_CHECK(button_add_cb(&button, BUTTON_CLICK_MEDIUM, button_click_medium_cb, NULL));
    ESP_ERROR_CHECK(button_add_cb(&button, BUTTON_CLICK_DOUBLE, button_click_double_cb, NULL));

    ESP_LOGI(TAG, "config complete");
}

void init_game(game_state_machine_t *game)
{
    game->current_state = INTRO;
    game->score = 0;
    game->max_score = get_highscore();
}

void vFlappyBirdTask(void *pvParameters)
{
    led_matrix_t led_matrix;
    init_hardware(&led_matrix);

    game_state_machine_t game;
    init_game(&game);
    generate_parkour();

    while (true)
    {
        for (uint32_t i = 0; i < LED_STRIP_LED_COUNT; i++)
        {
            if (walls[i + start] == 1)
            {
                ESP_ERROR_CHECK(led_matrix_set_pixel(&led_matrix, i, 0, 0, 0));
            }
            else if (walls[i + start] == 0)
            {
                ESP_ERROR_CHECK(led_matrix_set_pixel(&led_matrix, i, 10, 10, 10));
            }
            else
            {
                ESP_ERROR_CHECK(led_matrix_set_pixel(&led_matrix, i, 0, 0, 0));
            }
        }
        ESP_ERROR_CHECK(led_matrix_set_pixel(&led_matrix, 13, 0, 10, 0));
        ESP_ERROR_CHECK(led_matrix_update(&led_matrix));

        vTaskDelay(pdMS_TO_TICKS(delay));
        start += 8;
    }
}
