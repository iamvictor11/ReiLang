#include "chunk.h"
#include "memory.h"

void lunaChunk_init(luna_Chunk *self)
{
    lunaValueArray_init(&self->constants);
    lunaOpcodeArray_init(&self->codes);
}
void lunaChunk_free(luna_Chunk *self)
{
    lunaValueArray_free(&self->constants);
    lunaOpcodeArray_free(&self->codes);
}
size_t lunaChunk_writeConstant(luna_Chunk *self, luna_Value element)
{
    lunaValueArray_write(&self->constants, element);
    return self->constants.size - 1;
}
void lunaChunk_writeBytecode(luna_Chunk *self, luna_Byte element, luna_Position pos)
{
    lunaOpcodeArry_write(&self->codes, element, pos);
}
