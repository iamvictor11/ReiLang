#pragma once
#include "common.hpp"

namespace luna
{
    enum class Opcode : uint8_t
    {
        OP_CONSTANT = 0,
        /* 字面量 */
        OP_NIL,   //  nil
        OP_TRUE,  // true
        OP_FALSE, // false
        /* 数学运算 */
        OP_NEG, // -
        OP_ADD, // +
        OP_SUB, // -
        OP_MUL, // *
        OP_DIV, // /
        OP_MOD, // %
        OP_POW, // **
        /* 位运算 */
        OP_BIT_AND,  // &
        OP_BIT_OR,   // |
        OP_BIT_XOR,  // ^
        OP_BIT_XNOR, // `
        OP_BIT_NOT,  // ~
        OP_BIT_SHL,  // <<
        OP_BIT_SHR,  // >>
        /* 比较运算 */
        OP_EQ, // ==
        OP_NE, // !=
        OP_LT, // <
        OP_LE, // <=
        OP_GT, // >
        OP_GE, // >=
        /* 逻辑运算 */
        OP_AND, // &&
        OP_OR,  // ||
        OP_NOT, // !
        /* 保留词 */
        OP_PRINT,   // print
        OP_PRINTLN, // println
        OP_RETURN,  // return
        OP_JUMP,
        OP_JUMP_IF_FALSE,
        OP_LOOP,
        OP_POP,
        OP_DEFINE_GLOBAL,
        OP_GET_GLOBAL,
        OP_SET_GLOBAL,
        OP_GET_LOCAL,
        OP_SET_LOCAL,
        OP_CALL
    };
}