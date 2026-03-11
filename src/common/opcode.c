#include "opcode.h"
#include "memory.h"

void lunaOpcodeArray_init(luna_OpcodeArray *self)
{
    self->data = NULL;
    self->poss = NULL;
    self->size = 0;
    self->capacity = 0;
}
void lunaOpcodeArray_free(luna_OpcodeArray *self)
{
    LUNA_FREE_ARRAY(luna_Byte, self->data);
    LUNA_FREE_ARRAY(luna_Position, self->poss);
    lunaOpcodeArray_init(self);
}
void lunaOpcodeArry_write(luna_OpcodeArray *self, luna_Byte element, luna_Position pos)
{
    if (self->size >= self->capacity)
    {
        self->capacity = LUNA_GROW_CAPACITY(self->capacity);
        self->data = LUNA_GROW_ARRAY(luna_Byte, self->data, self->capacity);
        self->poss = LUNA_GROW_ARRAY(luna_Position, self->poss, self->capacity);
    }
    self->data[self->size] = element;
    self->poss[self->size] = pos;
    self->size++;
}