#pragma once
#include "rei/config.hpp"

namespace rei
{
    using Bytecode = REI_BYTECODE_TYPE;
    using Byteint = REI_BYTECODE_INT;
    enum Opcode : Bytecode
    {
        OP_CONSTANT = 0,
        OP_POP,
        /* 字面量 */
        OP_NIL,   //  nil
        OP_TRUE,  // true
        OP_FALSE, // false
        /* 一元运算 */
        OP_NEG,     // -
        OP_BIT_NOT, // ~
        OP_NOT,     // !
        /* 二元运算 */
        OP_ADD,      // +
        OP_SUB,      // -
        OP_MUL,      // *
        OP_DIV,      // /
        OP_MOD,      // %
        OP_POW,      // **
        OP_BIT_AND,  // &
        OP_BIT_OR,   // |
        OP_BIT_XOR,  // ^
        OP_BIT_XNOR, // `
        OP_BIT_SHL,  // <<
        OP_BIT_SHR,  // >>
        OP_EQ,       // ==
        OP_NE,       // !=
        OP_LT,       // <
        OP_LE,       // <=
        OP_GT,       // >
        OP_GE,       // >=
        OP_AND,      // &&
        OP_OR,       // ||
        /* 操作 */
        OP_BEG,
        OP_END,
        OP_PRINT,   // print
        OP_PRINTLN, // println
        OP_JUMP,
        OP_JMPT,
        OP_JMPF,
        OP_GET_HOST,
        OP_SET_HOST,
        OP_DEF_GLOBAL,
        OP_GET_GLOBAL,
        OP_SET_GLOBAL,
        OP_DEF_LOCAL,
        OP_GET_LOCAL,
        OP_SET_LOCAL,
        // OP_DEF_UPVAL,
        // OP_GET_UPVAL,
        // OP_SET_UPVAL,
        OP_CALL,
        OP_RETURN,
        OP_HALT
    };
}