#include "base.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "vm.h"

#pragma region Var
bool lunaVar_IsNil(luna_Var v)
{
    return v.type == LUNA_TYPE_NIL;
}
bool lunaVar_IsTrivial(luna_Var v)
{
    return v.type & LUNA_TYPE_TRIVIAL;
}
bool lunaVar_IsNumber(luna_Var v)
{
    return v.type & LUNA_TYPE_NUMBER;
}
bool lunaVar_IsObject(luna_Var v)
{
    return v.type & LUNA_TYPE_OBJ;
}
luna_Var lunaVar_ToString(luna_Var v)
{
    luna_VM *vm = v.vm;
    luna_Var res_v = {.vm = vm, .type = LUNA_TYPE_STRING, .data.r = LUNA_POOL_ALLOC(vm->strings)};
    luna_String *s = LUNA_POOL_AT(vm->strings, luna_String, res_v.data.r);
    lunaString_Malloc(s, 32);
    char buf[32];
    switch (v.type)
    {
    case LUNA_TYPE_NIL:
        lunaString_AppendLen(s, "nil", 3);
        break;
    case LUNA_TYPE_INT:
        sprintf(buf, "%d", v.data.i);
        lunaString_Append(s, buf);
        break;
    case LUNA_TYPE_FLOAT:
        sprintf(buf, "%g", v.data.f);
        lunaString_Append(s, buf);
        break;
    case LUNA_TYPE_STRING:
        lunaString_AppendObj(s, LUNA_POOL_AT(vm->strings, luna_String, v.data.r));
        break;
    case LUNA_TYPE_TABLE:
        lunaString_AppendLen(s, "[table]", 7);
        break;
    case LUNA_TYPE_FUNC:
        lunaString_AppendLen(s, "[func]", 6);
        break;
    default:
        lunaString_AppendLen(s, "[unknown]", 9);
        break;
    }
    return res_v;
}
void lunaVar_Assign(luna_Var *v, const luna_Var other)
{
    lunaObj_Retain(other);
    lunaObj_Release(*v);
    *v = other;
}
void lunaVar_Free(luna_Var *v)
{
    luna_VM *vm = v->vm;
    if (v->data.r == LUNA_NULL_REF)
        return;
    switch (v->type)
    {
    case LUNA_TYPE_STRING:
        LUNA_POOL_FREE(vm->strings, luna_String, lunaString_Free, v->data.r);
        v->data.r = LUNA_NULL_REF;
        break;
    case LUNA_TYPE_TABLE:
    {
        luna_Table *t = LUNA_POOL_AT(vm->tables, luna_Table, v->data.r);
        if (t->data)
            for (size_t i = 0; i < t->size; i++)
                lunaObj_Release(t->data[i]);
        LUNA_POOL_FREE(vm->tables, luna_Table, lunaTable_Free, v->data.r);
        v->data.r = LUNA_NULL_REF;
        break;
    }
    case LUNA_TYPE_FUNC:
        LUNA_POOL_FREE(vm->funcs, luna_Func, lunaFunc_Free, v->data.r);
        v->data.r = LUNA_NULL_REF;
        break;
    default:
        break;
    }
    v->data.r = LUNA_NULL_REF;
    v->type = LUNA_TYPE_NIL;
}
#pragma endregion
#pragma region Obj
void lunaObj_Retain(luna_Var v)
{
    if (!lunaVar_IsObject(v) || v.data.r == LUNA_NULL_REF)
        return;
    luna_VM *vm = v.vm;
    luna_ObjHeader *header = NULL;
    switch (v.type)
    {
    case LUNA_TYPE_STRING:
        header = &LUNA_POOL_AT(vm->strings, luna_String, v.data.r)->header;
        break;
    case LUNA_TYPE_TABLE:
        header = &LUNA_POOL_AT(vm->tables, luna_Table, v.data.r)->header;
        break;
    case LUNA_TYPE_FUNC:
        header = &LUNA_POOL_AT(vm->funcs, luna_Func, v.data.r)->header;
        break;
    default:
        return;
    }
    if (header)
        header->ref_count++;
}
void lunaObj_Release(luna_Var v)
{
    if (!lunaVar_IsObject(v) || v.data.r == LUNA_NULL_REF)
        return;
    luna_VM *vm = v.vm;
    luna_ObjHeader *header = NULL;
    switch (v.type)
    {
    case LUNA_TYPE_STRING:
        header = &LUNA_POOL_AT(vm->strings, luna_String, v.data.r)->header;
        break;
    case LUNA_TYPE_TABLE:
        header = &LUNA_POOL_AT(vm->tables, luna_Table, v.data.r)->header;
        break;
    case LUNA_TYPE_FUNC:
        header = &LUNA_POOL_AT(vm->funcs, luna_Func, v.data.r)->header;
        break;
    default:
        return;
    }
    if (header && header->ref_count > 0)
    {
        header->ref_count--;
        if (header->ref_count == 0)
            lunaVar_Free(&v);
    }
}
#pragma endregion
#pragma region String
void lunaString_Malloc(luna_String *s, luna_UInt cap)
{
    s->data = malloc(cap * sizeof(char));
    s->len = 0;
    s->capacity = cap;
}
bool lunaString_IsEmpty(const luna_String *s)
{
    return !s->data || s->len == 0;
}
void lunaString_Append(luna_String *s, const char *str)
{
    lunaString_AppendLen(s, str, strlen(str));
}
void lunaString_AppendLen(luna_String *s, const char *str, luna_UInt len)
{
    if (str == NULL || len == 0)
        return;
    luna_UInt total_needed = s->len + len;
    if (total_needed + 1 > s->capacity)
    {
        luna_UInt new_cap = (s->capacity == 0) ? 16 : s->capacity * 2;
        if (new_cap < total_needed + 1)
            new_cap = total_needed + 1;
        char *new_data = (char *)realloc(s->data, new_cap);
        if (new_data == NULL)
        {
            // TODO: 内存分配失败
            return;
        }
        s->data = new_data;
        s->capacity = new_cap;
    }
    memcpy(s->data + s->len, str, len);
    s->len = total_needed;
    s->data[s->len] = '\0';
}
void lunaString_AppendObj(luna_String *s, const luna_String *other)
{
    if (other == NULL || other->len == 0)
        return;
    lunaString_AppendLen(s, other->data, other->len);
}
luna_Var lunaString_ToInt(luna_String *s)
{
    if (lunaString_IsEmpty(s))
        return LUNA_NIL_VAR;
    char *endptr;
    long val = strtol(s->data, &endptr, 10);
    if (*endptr == '\0')
    {
        luna_Var v;
        v.type = LUNA_TYPE_INT;
        v.data.i = (luna_Int)val;
        return v;
    }
    return LUNA_NIL_VAR;
}
luna_Var lunaString_ToFloat(luna_String *s)
{
    if (lunaString_IsEmpty(s))
        return LUNA_NIL_VAR;
    char *endptr;
    float val = strtof(s->data, &endptr);
    if (*endptr == '\0')
    {
        luna_Var v;
        v.type = LUNA_TYPE_FLOAT;
        v.data.f = (luna_Float)val;
        return v;
    }
    return LUNA_NIL_VAR;
}
void lunaString_Free(luna_String *s)
{
    if (s->data)
        free(s->data);
    s->data = NULL;
    s->len = s->capacity = 0;
}
#pragma endregion
#pragma region Table
void lunaTable_Malloc(luna_Table *t, luna_UInt cap)
{
    t->data = malloc(cap * sizeof(luna_Var));
    t->size = 0;
    t->capacity = cap;
}
luna_Var lunaTable_At(luna_Table *t, luna_UInt i)
{
    if (i >= t->size)
        return LUNA_NIL_VAR;
    return t->data[i];
}
void lunaTable_Put(luna_Table *t, luna_UInt i, luna_Var v)
{
    if (i > LUNA_MAX_TABLE_SIZE)
        return;
    if (i >= t->capacity)
    {
        luna_UInt new_cap = (t->capacity == 0) ? 8 : t->capacity * 2;
        if (new_cap <= i)
            new_cap = i + 1;
        luna_Var *new_data = (luna_Var *)realloc(t->data, sizeof(luna_Var) * new_cap);
        if (new_data == NULL)
        {
            // TODO: 内存分配失败
            return;
        }
        t->data = new_data;
        t->capacity = new_cap;
    }
    if (i >= t->size)
    {
        for (luna_UInt j = t->size; j < i; j++)
            t->data[j] = LUNA_NIL_VAR;
        t->size = i + 1;
    }
    lunaObj_Release(t->data[i]);
    t->data[i] = v;
    lunaObj_Retain(v);
}
void lunaTable_Insert(luna_Table *t, luna_UInt i, luna_Var v)
{
    if (i >= t->size)
    {
        lunaTable_Put(t, i, v);
        return;
    }
    if (t->size + 1 > t->capacity)
    {
        luna_UInt new_cap = (t->capacity == 0) ? 8 : t->capacity * 2;
        luna_Var *new_data = (luna_Var *)realloc(t->data, sizeof(luna_Var) * new_cap);
        if (!new_data)
            return;
        t->data = new_data;
        t->capacity = new_cap;
    }
    memmove(t->data + i + 1, t->data + i, sizeof(luna_Var) * (t->size - i));
    t->data[i] = v;
    lunaObj_Retain(v);
    t->size++;
}
void lunaTable_Remove(luna_Table *t, luna_UInt i)
{
    if (i >= t->size)
        return;
    lunaObj_Release(t->data[i]);
    if (i < t->size - 1)
        memmove(t->data + i, t->data + i + 1, sizeof(luna_Var) * (t->size - i - 1));
    t->size--;
}
void lunaTable_Free(luna_Table *t)
{
    free(t->data);
    t->data = NULL;
    t->size = t->capacity = 0;
}
#pragma endregion
#pragma region Func
void lunaFunc_Free(luna_Func *f)
{
    // TODO: 函数释放
    if (f)
    {
        
    }
}
#pragma endregion
