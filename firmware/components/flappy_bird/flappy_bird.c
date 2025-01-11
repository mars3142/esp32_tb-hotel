#include "flappy_bird.h"

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <esp_random.h>
#include <esp_log.h>
#include <esp_err.h>
#include <driver/gpio.h>

#include "sdkconfig.h"
#include "led_matrix.h"
#include "persistance.h"
#include "state_machine.h"

static const char *TAG = "flappy";

#define LED_STRIP_LED_COUNT (8 * 8 * 4)

uint8_t walls[LED_STRIP_LED_COUNT * 8] = {-1};

QueueHandle_t buttonQueue;
int value = 0;

uint8_t *generate_parkour()
{
    int index = 0;
    for (int i = 0; i < LED_STRIP_LED_COUNT; i++)
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
    return walls;
}

static void IRAM_ATTR button_isr_handler(void *arg)
{
    value = 1 - value;
    xQueueSendFromISR(buttonQueue, &value, NULL);
}

void init_hardware(led_matrix_t *led_matrix)
{
    ESP_ERROR_CHECK(led_matrix_init(led_matrix, CONFIG_GEO_WLED_DIN_PIN, LED_STRIP_LED_COUNT));

    gpio_reset_pin(CONFIG_GEO_BTN_PIN);
    gpio_set_direction(CONFIG_GEO_BTN_PIN, GPIO_MODE_INPUT);
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

    uint8_t *wall = generate_parkour();
    bool led_on_off = true;

    while (true)
    {
        // if (xQueueReceive(buttonQueue, &value, 0))
        // {
        //     ESP_LOGI(TAG, "Button pressed!");
        // }

        if (led_on_off)
        {
            for (uint32_t i = 0; i < LED_STRIP_LED_COUNT; i++)
            {
                if (walls[i] == 1)
                {
                    ESP_ERROR_CHECK(led_matrix_set_pixel(&led_matrix, i, 0, 0, 0));
                }
                else if (walls[i] == 0)
                {
                    ESP_ERROR_CHECK(led_matrix_set_pixel(&led_matrix, i, 1, 1, 1));
                }
                else
                {
                    ESP_ERROR_CHECK(led_matrix_set_pixel(&led_matrix, i, 0, 0, 0));
                }
            }
            ESP_ERROR_CHECK(led_matrix_refresh(&led_matrix));
            ESP_LOGI(TAG, "LED ON!");
        }
        else
        {
            ESP_ERROR_CHECK(led_matrix_clear(&led_matrix));
            ESP_LOGI(TAG, "LED OFF!");
        }

        // led_on_off = !led_on_off;
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
