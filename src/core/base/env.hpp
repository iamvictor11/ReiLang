#pragma once
#include "common.hpp"

namespace rei
{
    namespace Value
    {
        enum Lifetime : Bytecode
        {
            VLT_HOST,
            VLT_GLOBAL,
            VLT_LOCAL,
            VLT_UPVALUE
        };
        struct Coord final
        {
            Lifetime lifetime = VLT_GLOBAL;
            Bytecode uplevel = 0;
            Bytecode slot = REI_BYTECODE_MAX;
            bool isValid() const { return slot != REI_BYTECODE_MAX; }
        };
    }
    enum ProcessStage
    {
        PS_COMPILE,
        PS_RUNTIME
    };
    template<ProcessStage PS>
    class Env;
}
