#pragma once
#include "bytecode.hpp"
#include <vector>

namespace rei
{
    struct Chunk final
    {
        std::vector<OpCode> code;
    };
}
