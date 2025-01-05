#pragma once

typedef enum
{
    INTRO,
    MENU,
    RUNNING,
    GAME_OVER
} game_state_t;

typedef enum
{
    NO_EVENT,
    UP,
    DOWN,
    CRASH,
    SELECT,
    RESTART
} game_event_t;

typedef struct
{
    game_state_t current_state;
    int score;
    int max_score;
} game_state_machine_t;
