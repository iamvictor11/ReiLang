#ifndef LUNA_COMMON_OPCODE_H
#define LUNA_COMMON_OPCODE_H
#include <stdint.h>
#include "common/common.h"

typedef enum luna_Opcode
{
    LUNA_OP_CONSTANT,
    LUNA_OP_RETURN,
    LUNA_OP_NIL,
    LUNA_OP_TRUE,
    LUNA_OP_FALSE,
    LUNA_OP_NOT,
    LUNA_OP_EQUAL,
    LUNA_OP_GREATER,
    LUNA_OP_LESS,
    LUNA_OP_NEG,
    LUNA_OP_ADD,
    LUNA_OP_SUB,
    LUNA_OP_MUL,
    LUNA_OP_DIV,
    LUNA_OP_MOD,
    LUNA_OP_POW,
    LUNA_OP_PRINT,
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