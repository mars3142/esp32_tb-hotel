#pragma once

#include "esp_err.h"

#ifdef __cplusplus
extern "C"
{
#endif
    esp_err_t persistance_init();
#ifdef __cplusplus
}
#endif

void set_highscore(uint32_t highscore);
uint32_t get_highscore();
