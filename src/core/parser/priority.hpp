#pragma once
#include <cstdint>
#include "core/lexer/token.hpp"

namespace luna
{
    enum Precedence : uint8_t
    {
        PREC_NONE,
        PREC_ASSIGNMENT, // = := += ...
        PREC_OR,         // ||
        PREC_AND,        // &&
        PREC_BIT_OR,     // |
        PREC_BIT_XOR,    // ^ `
        PREC_BIT_AND,    // &
        PREC_EQUALITY,   // == !=
        PREC_COMPARISON, // < > <= >=
        PREC_TERM,       // + -
        PREC_FACTOR,     // * / %
        PREC_POW,        // **
        PREC_UNARY,      // ! - ~
        PREC_PRIMARY
    };
}