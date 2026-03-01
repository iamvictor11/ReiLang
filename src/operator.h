#ifndef LUNA_OPERATOR_H
#define LUNA_OPERATOR_H

typedef enum luna_Operator
{
    /*数学运算*/
    LUNA_OPERATOR_ADD,      // +
    LUNA_OPERATOR_SUB,      // -
    LUNA_OPERATOR_MUL,      // *
    LUNA_OPERATOR_DIV,      // /
    LUNA_OPERATOR_MOD,      // %
    LUNA_OPERATOR_POW,      // **
    LUNA_OPERATOR_NEG,      // -
    LUNA_OPERATOR_SELF_ADD, // +=
    LUNA_OPERATOR_SELF_SUB, // -=
    LUNA_OPERATOR_SELF_MUL, // *=
    LUNA_OPERATOR_SELF_DIV, // /=
    LUNA_OPERATOR_SELF_MOD, // %=
    LUNA_OPERATOR_SELF_POW, // **=
    /*位运算*/
    LUNA_OPERATOR_BIT_AND,       // &
    LUNA_OPERATOR_BIT_OR,        // |
    LUNA_OPERATOR_BIT_XOR,       // ^
    LUNA_OPERATOR_BIT_XNOR,      // `
    LUNA_OPERATOR_BIT_NOT,       // ~
    LUNA_OPERATOR_BIT_SHL,       // <<
    LUNA_OPERATOR_BIT_SHR,       // >>
    LUNA_OPERATOR_SELF_BIT_AND,  // &=
    LUNA_OPERATOR_SELF_BIT_OR,   // |=
    LUNA_OPERATOR_SELF_BIT_XOR,  // ^=
    LUNA_OPERATOR_SELF_BIT_XNOR, // `=
    LUNA_OPERATOR_SELF_BIT_NOT,  // ~=
    LUNA_OPERATOR_SELF_BIT_SHL,  // <<=
    LUNA_OPERATOR_SELF_BIT_SHR,  // >>=
    /*比较运算*/
    LUNA_OPERATOR_EQ, // ==
    LUNA_OPERATOR_NE, // !=
    LUNA_OPERATOR_LT, // <
    LUNA_OPERATOR_LE, // <=
    LUNA_OPERATOR_GT, // >
    LUNA_OPERATOR_GE, // >=
    /*逻辑运算*/
    LUNA_OPERATOR_NOT, // !
    LUNA_OPERATOR_AND, // &&
    LUNA_OPERATOR_OR,  // ||
} luna_Operator;

#endif