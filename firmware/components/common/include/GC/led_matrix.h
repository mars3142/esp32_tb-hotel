#pragma once

#include <cstdint>

namespace gc
{
    class LedMatrix
    {
    public:
        virtual ~LedMatrix() = default;
        virtual void setPixelColor(std::uint16_t n, std::uint32_t c) = 0;
        virtual void clear() = 0;
        virtual void show() = 0;
    };
}
