#ifndef LUNA_VM_H
#define LUNA_VM_H
#include "pool.h"

typedef struct luna_State
{
    luna_UInt max_VarCount;
    luna_UInt max_ObjPoolSize;
    luna_UInt max_TableSize;
    luna_UInt ini_StringCap;
    luna_UInt ini_TableCap;
} luna_State;

#define LUNA_DEFAULT_STRING                        \
    (luna_State)                                   \
    {                                              \
        .max_VarCount = LUNA_MAX_VAR_COUNT,        \
        .max_ObjPoolSize = LUNA_MAX_OBJ_POOL_SIZE, \
        .max_TableSize = LUNA_MAX_TABLE_SIZE,      \
        .ini_StringCap = LUNA_INI_STRING_CAP,      \
        .ini_TableCap = LUNA_INI_TABLE_CAP         \
    }

typedef struct luna_VM
{
    luna_State state;
    luna_StringPool strings;
    luna_TablePool tables;
    luna_FuncPool funcs;
} luna_VM;

luna_VM luna_CreateVM(luna_State state);
void luna_DestroyVM(luna_VM *vm);

void lunaVM_Clear(luna_VM *vm);
// void lunaVM_GC(luna_VM *vm);

#endif
