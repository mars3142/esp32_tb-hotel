#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "include/led_matrix.h"

constexpr unsigned int WINDOW_WIDTH = 810;
constexpr unsigned int WINDOW_HEIGHT = 450;

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;

const sim::LedMatrix led_matrix(32, 8);

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD) == false)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't initialize SDL!", SDL_GetError(), nullptr);
        return SDL_APP_FAILURE;
    }

    if (constexpr uint32_t window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN; SDL_CreateWindowAndRenderer(
        "Geocache - TB-Hotel", WINDOW_WIDTH, WINDOW_HEIGHT, window_flags, &window, &renderer) == false)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't create window/renderer!", SDL_GetError(), nullptr);
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderVSync(renderer, 1);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    switch (event->type)
    {
    case SDL_EVENT_QUIT:
        // end the program, reporting success to the OS
        return SDL_APP_SUCCESS;

    case SDL_EVENT_KEY_DOWN:
        if (event->key.key == SDLK_ESCAPE)
        {
            // end the program on ESC key,
            // returning success to the OS
            return SDL_APP_SUCCESS;
        }

    default:
        //SDL_Log("Unused event: %d", event->type);
        break;
    }

    // return continue to continue
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
    led_matrix.draw(renderer);

    SDL_RenderPresent(renderer);

    // return continue to continue
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    // SDL will clean up the window/renderer for us.
}
