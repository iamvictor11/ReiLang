#pragma once
#include <string>

namespace rei
{
    enum SymbolKind
    {
        SK_CONSTANT,
        SK_VARIABLE,
        // ...
    };
    struct Symbol final
    {
        SymbolKind kind;
        std::string name;
    };
}
