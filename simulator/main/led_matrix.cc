#include "include/led_matrix.h"

constexpr float WLED_SIZE = 25.0f;
constexpr float WLED_SPACING = 10.0f;

namespace sim
{
    void LedMatrix::drawCircle(SDL_Renderer* renderer, const SDL_FPoint* position)
    {
        /* fade between shades of red every 3 seconds, from 0 to 255. */
        const auto r = static_cast<Uint8>((static_cast<float>(SDL_GetTicks() % 3000) / 3000.0f) * 255.0f);
        SDL_SetRenderDrawColor(renderer, r, 0, 0, 255);

        const auto rect = SDL_FRect{
            position->x,
            position->y,
            WLED_SIZE - WLED_SPACING,
            WLED_SIZE - WLED_SPACING
        };
        SDL_RenderFillRect(renderer, &rect);
    }

    void LedMatrix::draw(SDL_Renderer* renderer) const
    {
        for (int w = 0; w < width; ++w)
        {
            for (int h = 0; h < height; ++h)
            {
                auto position = SDL_FPoint(
                    w * WLED_SIZE + WLED_SPACING,
                    h * WLED_SIZE + WLED_SPACING
                );
                drawCircle(renderer, &position);
            }
        }
    }

    void LedMatrix::setPixelColor(std::uint16_t n, std::uint32_t c)
    {
    }

    void LedMatrix::clear()
    {
    }

    void LedMatrix::show()
    {
    }
}
