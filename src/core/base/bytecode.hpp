#pragma once
#include "vic/config.hpp"

namespace vic
{
    using Bytecode = VIC_BYTECODE_TYPE;
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
        OP_RETURN,  // return
        OP_JUMP,
        OP_JMPT,
        OP_JMPF,
        OP_LOOP,
        OP_DEF_VAR,
        OP_GET_VAR,
        OP_SET_VAR,
        OP_CALL
    };
}