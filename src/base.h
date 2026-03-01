#ifndef LUNA_BASE_H
#define LUNA_BASE_H
#include <stdint.h>
#include <stdbool.h>

typedef struct luna_VM luna_VM;

#define LUNA_MAX_VAR_COUNT 4096
#define LUNA_MAX_OBJ_POOL_SIZE 4096
#define LUNA_MAX_TABLE_SIZE 256
#define LUNA_INI_STRING_CAP 2
#define LUNA_INI_TABLE_CAP 2

typedef uint32_t luna_UInt, luna_Ref;
typedef int32_t luna_Int;
typedef float luna_Float;
typedef uint8_t luna_Byte;
#define LUNA_NULL_REF (luna_Ref)0xFFFFFFFF
typedef enum luna_VarType
{
    LUNA_TYPE_NIL = 0,
    LUNA_TYPE_INT = 1U << 0,
    LUNA_TYPE_FLOAT = 1U << 1,
    LUNA_TYPE_TRIVIAL = LUNA_TYPE_NIL | LUNA_TYPE_INT | LUNA_TYPE_FLOAT,
    LUNA_TYPE_NUMBER = LUNA_TYPE_INT | LUNA_TYPE_FLOAT,
    LUNA_TYPE_STRING = 1U << 2,
    LUNA_TYPE_TABLE = 1U << 3,
    LUNA_TYPE_FUNC = 1U << 4,
    LUNA_TYPE_OBJ = LUNA_TYPE_STRING | LUNA_TYPE_TABLE | LUNA_TYPE_FUNC,
    LUNA_TYPE_ANY = LUNA_TYPE_TRIVIAL | LUNA_TYPE_OBJ
} luna_VarType;
typedef union luna_VarData
{
    luna_Int i;
    luna_Float f;
    luna_Ref r;
} luna_VarData;
typedef struct luna_Var
{
    luna_VM *vm;
    luna_VarType type;
    luna_VarData data;
} luna_Var;
#define LUNA_NIL_VAR \
    (luna_Var) { .vm = NULL, .type = LUNA_TYPE_NIL, .data.i = 0 }
typedef struct luna_ObjHeader
{
    luna_UInt ref_count;
} luna_ObjHeader;
#define LUNA_NULL_OBJ_HEADER \
    (luna_ObjHeader) { .ref_count = 0 }
typedef struct luna_String
{
    luna_ObjHeader header;
    char *data;
    luna_UInt len;
    luna_UInt capacity;
} luna_String;
#define LUNA_NULL_STRING \
    (luna_String) { .header = LUNA_NULL_OBJ_HEADER, .data = NULL, .len = 0, .capacity = 0 }
typedef struct luna_Table
{
    luna_ObjHeader header;
    luna_Var *data;
    luna_UInt size;
    luna_UInt capacity;
} luna_Table;
#define LUNA_NULL_TABLE \
    (luna_Table) { .header = LUNA_NULL_OBJ_HEADER, .data = NULL, .size = 0 }
typedef struct luna_Func
{
    luna_ObjHeader header;
    luna_String *name;
    uint8_t arg_count;
    luna_Byte *bytecode;
    int bytecode_count;
} luna_Func;
#define LUNA_NULL_FUNC \
    (luna_Func) { .header = LUNA_NULL_OBJ_HEADER, .name = NULL, .arg_count = 0, .bytecode = NULL, .bytecode_count = 0 }

bool lunaVar_IsNil(luna_Var v);
bool lunaVar_IsTrivial(luna_Var v);
bool lunaVar_IsNumber(luna_Var v);
bool lunaVar_IsObject(luna_Var v);
luna_Var lunaVar_ToString(luna_Var v);
void lunaVar_Assign(luna_Var *v, const luna_Var other);
void lunaVar_Free(luna_Var *v);

void lunaObj_Retain(luna_Var v);
void lunaObj_Release(luna_Var v);

void lunaString_Malloc(luna_String *s, luna_UInt cap);
bool lunaString_IsEmpty(const luna_String *s);
void lunaString_Append(luna_String *s, const char *str);
void lunaString_AppendLen(luna_String *s, const char *str, luna_UInt len);
void lunaString_AppendObj(luna_String *s, const luna_String *other);
luna_Var lunaString_ToInt(luna_String *s);
luna_Var lunaString_ToFloat(luna_String *s);
void lunaString_Free(luna_String *s);

void lunaTable_Malloc(luna_Table *t, luna_UInt cap);
luna_Var lunaTable_At(luna_Table *t, luna_UInt i);
void lunaTable_Put(luna_Table *t, luna_UInt i, luna_Var v);
void lunaTable_Insert(luna_Table *t, luna_UInt i, luna_Var v);
void lunaTable_Remove(luna_Table *t, luna_UInt i);
void lunaTable_Free(luna_Table *t);

void lunaFunc_Free(luna_Func *f);

#endif
