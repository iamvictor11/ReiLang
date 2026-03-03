#include "token.hpp"

namespace luna::Token
{
    std::string to_symbol(Type type)
    {
        switch (type) {
            #define LUNA_TOKEN_X(name, symbol) case TK_##name: return symbol;
            LUNA_TOKEN_TYPE_LIST
            #undef LUNA_TOKEN_X
            default:
                return "<?>";
        }
    }
    std::string to_string(Type type)
    {
        switch (type)
        {
            #define LUNA_TOKEN_X(name, symbol) case TK_##name: return #name;
            LUNA_TOKEN_TYPE_LIST
            #undef LUNA_TOKEN_X
            default:
                return "UNKNOWN";
        }
    }

    std::string Unit::toString() const
    {
        std::string res = to_string(type) + " " + to_symbol(type);
        if (type == TK_LIT_INT || type == TK_LIT_FLOAT || type == TK_LIT_STRING)
            res += " " + Value::toString(literal);
        return res;
    }
    std::string Unit::symbol() const
    {
        return to_symbol(type);
    }
}
