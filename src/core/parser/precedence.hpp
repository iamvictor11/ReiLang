#pragma once
#include <cstdint>

namespace luna::Precedence
{
    enum Level : uint8_t
    {
        PREC_NONE,
        PREC_ASSIGN, // = x= :=
        PREC_LOR,    // ||
        PREC_LAND,   // &&
        PREC_BOR,    // |
        PREC_BXOR,   // ^ `
        PREC_BAND,   // &
        PREC_EQUAL,  // == !=
        PREC_COMPAR, // < > <= >=
        PREC_SHIFT,  // << >>
        PREC_TERM,   // + -
        PREC_FACTOR, // * / %
        PREC_POW,    // **
        PREC_UNARY,  // - ~ !
        PREC_CALL,   // . ()
        PREC_PRIMARY
    };
}
