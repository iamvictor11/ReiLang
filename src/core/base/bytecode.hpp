#pragma once
#include "rei/config.hpp"

namespace rei
{
    using Bytecode = REI_BYTECODE_TYPE;
    using Byteint = REI_BYTECODE_INT;
    enum Opcode : Bytecode
    {
        OP_CONSTANT = 0,
        OP_CLONE,
        OP_POP,
        OP_KEEP,
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
        /* 初始化 */
        OP_INIT_ARRAY,
        /* 操作 */
        OP_ENTER,
        OP_EXIT,
        OP_PRINT,   // print
        OP_PRINTLN, // println
        OP_JUMP,
        OP_JMPT,
        OP_JMPF,
        OP_HOST_GET,
        OP_HOST_SET,
        OP_HOST_SSET,
        OP_GLOBAL_DEF,
        OP_GLOBAL_GET,
        OP_GLOBAL_SET,
        OP_GLOBAL_SSET,
        OP_LOCAL_DEF,
        OP_LOCAL_GET,
        OP_LOCAL_SET,
        OP_LOCAL_SSET,
        OP_ONCE_GET,
        OP_ONCE_SET,
        OP_ONCE_SSET,
        OP_CALL,
        OP_RETURN,
        OP_INDEX_GET,
        OP_INDEX_SET,
        OP_INDEX_SSET,
        /* midi */
        OP_CHANNEL,
        OP_PROGRAM,
        OP_VOLUME,
        OP_VELOCITY,
        OP_PLAY,
        OP_UNPLAY,
        OP_WAIT,
        /* 休止 */
        OP_HALT
    };
}