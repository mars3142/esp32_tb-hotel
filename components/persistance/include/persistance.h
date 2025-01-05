#pragma once

#include "esp_err.h"

esp_err_t persistance_init();
void set_highscore(uint32_t highscore);
uint32_t get_highscore();
