#include "persistance.h"

#include "nvs_flash.h"

nvs_handle_t persistance_handle;

esp_err_t persistance_init()
{
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    ESP_ERROR_CHECK(nvs_open("storage", NVS_READWRITE, &persistance_handle));

    return ESP_OK;
}

void set_highscore(uint32_t highscore)
{
    ESP_ERROR_CHECK(nvs_set_u32(persistance_handle, "highscore", highscore));
    ESP_ERROR_CHECK(nvs_commit(persistance_handle));
}

uint32_t get_highscore()
{
    uint32_t highscore;
    esp_err_t err = nvs_get_u32(persistance_handle, "highscore", &highscore);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND)
    {
        ESP_ERROR_CHECK(err);
    }
    return highscore;
}
