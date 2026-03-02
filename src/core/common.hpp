#pragma once
#include <cstdint>

namespace luna
{
    struct Position final
    {
        uint64_t line = 1, column = 1;
    };
}
