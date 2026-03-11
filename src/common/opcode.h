#ifndef LUNA_COMMON_OPCODE_H
#define LUNA_COMMON_OPCODE_H
#include <stdint.h>
#include "common.h"

typedef enum luna_Opcode
{
    LUNA_OP_CONSTANT,
    LUNA_OP_RETURN, // return
    LUNA_OP_NIL,    //  nil
    LUNA_OP_TRUE,   // true
    LUNA_OP_FALSE,  // false
    /* 数学运算 */
    LUNA_OP_NEG, // -
    LUNA_OP_ADD, // +
    LUNA_OP_SUB, // -
    LUNA_OP_MUL, // *
    LUNA_OP_DIV, // /
    LUNA_OP_MOD, // %
    LUNA_OP_POW, // **
    /* 位运算 */
    LUNA_OP_BIT_AND,  // &
    LUNA_OP_BIT_OR,   // |
    LUNA_OP_BIT_XOR,  // ^
    LUNA_OP_BIT_XNOR, // `
    LUNA_OP_BIT_NOT,  // ~
    LUNA_OP_BIT_SHL,  // <<
    LUNA_OP_BIT_SHR,  // >>
    /* 比较运算 */
    LUNA_OP_EQ, // ==
    LUNA_OP_NE, // !=
    LUNA_OP_LT, // <
    LUNA_OP_LE, // <=
    LUNA_OP_GT, // >
    LUNA_OP_GE, // >=
    /* 逻辑运算 */
    LUNA_OP_AND, // &&
    LUNA_OP_OR,  // ||
    LUNA_OP_NOT, // !
    /* 保留词 */
    LUNA_OP_PRINT, // print
    LUNA_OP_JUMP,
    LUNA_OP_JUMP_IF_FALSE,
    LUNA_OP_LOOP,
    LUNA_OP_POP,
    LUNA_OP_DEFINE_GLOBAL,
    LUNA_OP_GET_GLOBAL,
    LUNA_OP_SET_GLOBAL,
    LUNA_OP_GET_LOCAL,
    LUNA_OP_SET_LOCAL,
    LUNA_OP_CALL
} luna_Opcode;

typedef struct luna_OpcodeArray
{
    luna_Byte *data;
    luna_Position *poss;
    size_t size;
    size_t capacity;
} luna_OpcodeArray;
void lunaOpcodeArray_init(luna_OpcodeArray *self);
void lunaOpcodeArray_free(luna_OpcodeArray *self);
void lunaOpcodeArry_write(luna_OpcodeArray *self, luna_Byte element, luna_Position pos);

#endif