#pragma once

namespace luna
{
    enum ParserType : uint8_t
    {
        PT_RD, PT_Pratt
    };
    template<ParserType PT>
    class Parser;
}
