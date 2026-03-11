#ifndef LUNA_COMMON_PRECEDENCE_H
#define LUNA_COMMON_PRECEDENCE_H

typedef enum luna_Precedence
{
    LUNA_PREC_NONE,
    LUNA_PREC_ASSIGN, // = x= :=
    LUNA_PREC_LOR,    // ||
    LUNA_PREC_LAND,   // &&
    LUNA_PREC_BOR,    // |
    LUNA_PREC_BXOR,   // ^ `
    LUNA_PREC_BAND,   // &
    LUNA_PREC_EQUAL,  // == !=
    LUNA_PREC_COMPAR, // < > <= >=
    LUNA_PREC_SHIFT,  // << >>
    LUNA_PREC_TERM,   // + -
    LUNA_PREC_FACTOR, // * / %
    LUNA_PREC_POW,    // **
    LUNA_PREC_UNARY,  // - ~ !
    LUNA_PREC_CALL,   // . ()
    LUNA_PREC_PRIMARY
} luna_Precedence;

#endif