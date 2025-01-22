#include "GC/wled.h"

namespace gc
{
    constexpr std::uint32_t WLED::toInteger() const
    {
        return static_cast<std::uint32_t>((red << 24) | (green << 16) | (blue << 8) | 255);
    }
}
