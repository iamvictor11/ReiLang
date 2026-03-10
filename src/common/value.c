#include "value.h"
#include "common/memory.h"

void lunaValueArray_init(luna_ValueArray *self)
{
    self->data = NULL;
    self->size = 0;
    self->capacity = 0;
}
void lunaValueArray_free(luna_ValueArray *self)
{
    LUNA_FREE_ARRAY(luna_Value, self->data);
    lunaValueArray_init(self);
}
void lunaValueArray_write(luna_ValueArray *self, luna_Value element)
{
    if (self->size >= self->capacity)
    {
        self->capacity = LUNA_GROW_CAPACITY(self->capacity);
        self->data = LUNA_GROW_ARRAY(luna_Value, self->data, self->capacity);
    }
    self->data[self->size] = element;
    self->size++;
}