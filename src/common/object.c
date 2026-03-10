#include "object.h"
#include <stdlib.h>
#include <string.h>

luna_String *lunaString_clone(const luna_String *self)
{
    size_t len = self->len;
    luna_String *res = malloc(sizeof(luna_String));
    res->obj.type = LUNA_OT_STRING;
    res->obj.next = NULL;
    res->data = malloc(len + 1);
    memcpy(res->data, self->data, len);
    res->data[len] = '\0';
    res->len = len;
    res->hash = self->hash;
    return res;
}
