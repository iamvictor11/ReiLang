#ifndef LUNA_COMMON_VALUE_H
#define LUNA_COMMON_VALUE_H
#include <stdint.h>
#include <stdbool.h>
#include "common.h"
#include "vm/opcode.h"

typedef int64_t luna_Integer;
typedef double luna_Number;
typedef bool luna_Boolean;
typedef enum luna_ObjectType
{
	LUNA_OT_STRING,
	LUNA_OT_FUNCTION,
	LUNA_OT_NATIVE
} luna_ObjectType;
typedef struct luna_Object
{
    luna_ObjectType type;
    struct luna_Object* next;
} luna_Object;
typedef enum luna_ValueType
{
	LUNA_VT_NIL,
	LUNA_VT_INTEGER,
	LUNA_VT_NUMBER,
	LUNA_VT_BOOLEAN,
	LUNA_VT_OBJECT
} luna_ValueType;
typedef struct luna_Value
{
	luna_ValueType type;
	union
	{
		luna_Integer i;
		luna_Number n;
		luna_Boolean b;
		luna_Object *o;
	} data;
} luna_Value;
typedef struct luna_ValueArray
{
	luna_Value* data;
	size_t size;
	size_t capacity;
} luna_ValueArray;
void lunaValueArray_init(luna_ValueArray *self);
void lunaValueArray_free(luna_ValueArray *self);
void lunaValueArray_write(luna_ValueArray *self, luna_Value element);
typedef struct luna_String
{
    luna_Object obj;
    char* data;
    size_t len;
    luna_Hash hash;
} luna_String;
typedef struct luna_Chunk
{
    luna_ValueArray constants;
    luna_OpcodeArray codes;
} luna_Chunk;
typedef struct luna_Function
{
    luna_Object obj;
    int arity;
    luna_Chunk chunk;
    luna_String* name;
} luna_Function;
typedef luna_Value (*luna_NativeFnuc)(int argc, luna_Value* args);

typedef struct luna_Native
{
    luna_Object obj;
    luna_NativeFnuc f;
} luna_Native;

#define LUNA_NIL_VAL() (luna_Value){LUNA_VT_NIL, 0}

#define LUNA_AS_INTEGER(value) ((value).data.i)
#define LUNA_AS_NUMBER(value) ((value).data.n)
#define LUNA_AS_BOOLEAN(value) ((value).data.b)
#define LUNA_AS_OBJECT(value) ((value).data.o)
#define LUNA_AS_STRING(value) ((luna_String*)LUNA_AS_OBJECT(value))
#define LUNA_AS_CSTRING(value) (LUNA_AS_STRING(value)->data)
#define LUNA_AS_FUNCTION(value) ((luna_Function*)LUNA_AS_OBJECT(value))
#define LUNA_AS_NATIVE(value) (((VmNative*)LUNA_AS_OBJECT(value))->f)

#define LUNA_IS_BOOL(value) ((value).type == LUNA_VT_BOOL)
#define LUNA_IS_NIL(value) ((value).type == LUNA_VT_NIL)
#define LUNA_IS_NUMBER(value) ((value).type == LUNA_VT_NUMBER)
#define LUNA_IS_OBJECT(value) ((value).type == LUNA_VT_OBJECT)
#define LUNA_IS_STRING(value) (LUNA_IS_OBJECT(value) && (LUNA_AS_OBJECT(value))->type == LUNA_OT_STRING)
#define LUNA_IS_FUNCTION(value) (LUNA_IS_OBJECT(value) && (LUNA_AS_OBJECT(value))->type == LUNA_OT_FUNCTION)
#define LUNA_IS_NATIVE(value) (LUNA_IS_OBJECT(value) && (LUNA_AS_OBJECT(value))->type == LUNA_OT_NATIVE)

#endif