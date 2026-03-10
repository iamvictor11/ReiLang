#include "log.h"
#include <stdio.h>

void _lunaValueArray_debugInfo(luna_ValueArray *self);
size_t _lunaValueArray_printValue(luna_Value *self, size_t offset);
void _lunaOpcodeArray_debugInfo(luna_OpcodeArray *self);
size_t _lunaOpcodeArray_printOpcode(luna_OpcodeArray *self, size_t offset);
size_t _lunaOpcodeArray_printGeneral(luna_OpcodeArray *self, size_t offset, const char *name);
size_t _lunaOpcodeArray_printConstant(luna_OpcodeArray *self, size_t offset, const char *name);

void lunaChunk_debugInfo(luna_Chunk *self, const char *name)
{
    printf("Chunk: %s\n", name);
    _lunaValueArray_debugInfo(&self->constants);
    _lunaOpcodeArray_debugInfo(&self->codes);
}
#pragma region ValueArray
void _lunaValueArray_debugInfo(luna_ValueArray *self)
{
    size_t offset;
    printf("ValueArray: size: %zu\n", self->size);
    for (offset = 0; offset < self->size;)
        offset = _lunaValueArray_printValue(self->data+offset, offset);
}
size_t _lunaValueArray_printValue(luna_Value *self, size_t offset)
{
    printf("[\033[1m\033[92m%04zu\033[0m] %f\n", offset, *self);
    return offset + 1;
}
#pragma endregion
#pragma region OpcodeArray
void _lunaOpcodeArray_debugInfo(luna_OpcodeArray *self)
{
    size_t offset;
    printf("OpcodeArray: size: %zu\n", self->size);
    for (offset = 0; offset < self->size;)
        offset = _lunaOpcodeArray_printOpcode(self, offset);
}
size_t _lunaOpcodeArray_printOpcode(luna_OpcodeArray *self, size_t offset)
{
    luna_Byte code = self->data[offset];
    switch (code)
    {
    case LUNA_OP_CONSTANT: return _lunaOpcodeArray_printConstant(self, offset, "CONSTANT");
    case LUNA_OP_RETURN: return _lunaOpcodeArray_printGeneral(self, offset, "RETURN");
    case LUNA_OP_NEG: return _lunaOpcodeArray_printGeneral(self, offset, "NEG");
    case LUNA_OP_ADD: return _lunaOpcodeArray_printGeneral(self, offset, "ADD");
    case LUNA_OP_SUB: return _lunaOpcodeArray_printGeneral(self, offset, "SUB");
    case LUNA_OP_MUL: return _lunaOpcodeArray_printGeneral(self, offset, "MUL");
    case LUNA_OP_DIV: return _lunaOpcodeArray_printGeneral(self, offset, "DIV");
    case LUNA_OP_MOD: return _lunaOpcodeArray_printGeneral(self, offset, "MOD");
    case LUNA_OP_POW: return _lunaOpcodeArray_printGeneral(self, offset, "POW");
    default: printf("未知的 Opcode %04d\n", code);
    }
    return offset + 1;
}
size_t _lunaOpcodeArray_printGeneral(luna_OpcodeArray *self, size_t offset, const char *name)
{
    luna_Position pos = self->poss[offset];
    printf("[\033[1m\033[92m%04zu\033[0m][\033[1m\033[36m%04zu:%04zu\033[0m] %-8s\n",
        offset, pos.line, pos.column, name);
    return offset + 1;
}
size_t _lunaOpcodeArray_printConstant(luna_OpcodeArray *self, size_t offset, const char *name)
{
    luna_Byte i = self->data[offset + 1];
    luna_Position pos = self->poss[offset];
    printf("[\033[1m\033[92m%04zu\033[0m][\033[1m\033[36m%04zu:%04zu\033[0m] %-8s %4d\n",
        offset, pos.line, pos.column, name, i);
    return offset + 2;
}
#pragma endregion
#pragma region Value
void lunaValue_print(luna_Value *self)
{
    printf("%f", *self);
}
#pragma endregion
