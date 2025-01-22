#pragma once

#include <cstdint>

namespace gc
{
    class WLED
    {
    public:
        WLED(const std::uint8_t red, const std::uint8_t green, const std::uint8_t blue) : red(red), green(green), blue(blue)
        {
        }

        [[nodiscard]] constexpr std::uint32_t toInteger() const;

    private:
        std::uint8_t red;
        std::uint8_t green;
        std::uint8_t blue;
    };
}
