#ifndef REI_TYPE_VALUE_H
#define REI_TYPE_VALUE_H

#include "rei_internal.h"
#include <stdbool.h>
#include <stdint.h>
#include REI_C_TEMPLATE_LIB_CONTAINER_STR_H

typedef struct ReiObject        ReiObject;
#define REI_DECL_OBJECT(NAME) typedef struct ReiObj##NAME ReiObj##NAME;
REI_DECL_OBJECT(String)     // "" ''
REI_DECL_OBJECT(Range)      // i..n
REI_DECL_OBJECT(Function)
REI_DECL_OBJECT(Closure)
REI_DECL_OBJECT(Upvalue)
REI_DECL_OBJECT(Method)
REI_DECL_OBJECT(Class)
REI_DECL_OBJECT(Instance)
#undef REI_DECL_OBJECT

typedef enum ReiValueType
{
    REI_VALUE_TYPE_NIL,
    REI_VALUE_TYPE_BOOLEAN,
    REI_VALUE_TYPE_INTEGER,
    REI_VALUE_TYPE_FLOATING,
    REI_VALUE_TYPE_OBJECT,
    REI_VALUE_TYPE_UNDEFINED,
    REI_VALUE_TYPE_MAX_COUNT
} ReiValueType;
typedef struct ReiValue
{
    ReiValueType type;
    union
    {
        bool vBoolean;
        int64_t vInteger;
        double vFloating;
        ReiObject* pObject;
    } as;
} ReiValue;

#define REI_IS_NIL(value)       ((value).type == REI_VALUE_TYPE_NIL)
#define REI_IS_BOOLEAN(value)   ((value).type == REI_VALUE_TYPE_BOOLEAN)
#define REI_IS_INTEGER(value)   ((value).type == REI_VALUE_TYPE_INTEGER)
#define REI_IS_FLOATING(value)  ((value).type == REI_VALUE_TYPE_FLOATING)
#define REI_IS_NUMBER(value)    ((value).type == REI_VALUE_TYPE_INTEGER || (value).type == REI_VALUE_TYPE_FLOATING)
#define REI_IS_OBJECT(value)    ((value).type == REI_VALUE_TYPE_OBJECT)
#define REI_IS_UNDEFINED(value) ((value).type == REI_VALUE_TYPE_UNDEFINED)

#define REI_AS_BOOLEAN(value)   ((value).as.vBoolean)
#define REI_AS_INTEGER(value)   ((value).as.vInteger)
#define REI_AS_FLOATING(value)  ((value).as.vFloating)
#define REI_AS_NUMBER(value)    ((value).as.vFloating)
#define REI_AS_OBJECT(value)    ((value).as.pObject)

#define REI_MK_NIL          ((ReiValue){REI_VALUE_TYPE_NIL,         {.vBoolean = false}})
#define REI_MK_BOOLEAN(v)   ((ReiValue){REI_VALUE_TYPE_BOOLEAN,     {.vBoolean = v}})
#define REI_MK_INTEGER(v)   ((ReiValue){REI_VALUE_TYPE_INTEGER,     {.vInteger = v}})
#define REI_MK_FLOATING(v)  ((ReiValue){REI_VALUE_TYPE_FLOATING,    {.vFloating = v}})
#define REI_MK_NUMBER(v)    ((ReiValue){REI_VALUE_TYPE_FLOATING,    {.vFloating = v}})
#define REI_MK_OBJECT(p)    ((ReiValue){REI_VALUE_TYPE_OBJECT,      {.pObject = (ReiObject*)p}})

C_TEMPLATE_DECL_VECTOR(, rei, Rei, ByteBuffer, ReiBytecode)
C_TEMPLATE_DEFN_VECTOR(, rei, Rei, ByteBuffer, ReiBytecode)
C_TEMPLATE_DECL_VECTOR(, rei, Rei, UIntBuffer, uint32_t)
C_TEMPLATE_DEFN_VECTOR(, rei, Rei, UIntBuffer, uint32_t)
C_TEMPLATE_DECL_VECTOR(, rei, Rei, ValueBuffer, ReiValue)
C_TEMPLATE_DEFN_VECTOR(, rei, Rei, ValueBuffer, ReiValue)
C_TEMPLATE_DECL_STRING(, rei, Rei, String)
C_TEMPLATE_DEFN_STRING(, rei, Rei, String)

typedef enum ReiObjType
{
    REI_OBJ_TYPE_STRING,
    REI_OBJ_TYPE_RANGE,
    REI_OBJ_TYPE_FUNCTION,
    REI_OBJ_TYPE_CLOSURE,
    REI_OBJ_TYPE_UPVALUE,
    REI_OBJ_TYPE_METHOD,
    REI_OBJ_TYPE_CLASS,
    REI_OBJ_TYPE_INSTANCE,
    REI_OBJ_TYPE_MAX_COUNT
} ReiObjType;
typedef struct ReiObject
{
    ReiObjType type;
    bool isMarked;
    ReiObject* next;
} ReiObject;

#define REI_IS_STRING(value)    (REI_IS_OBJECT(value) && REI_AS_OBJECT(value)->type == REI_OBJ_TYPE_STRING)
#define REI_IS_RANGE(value)     (REI_IS_OBJECT(value) && REI_AS_OBJECT(value)->type == REI_OBJ_TYPE_RANGE)
#define REI_IS_FUNCTION(value)  (REI_IS_OBJECT(value) && REI_AS_OBJECT(value)->type == REI_OBJ_TYPE_FUNCTION)
#define REI_IS_CLOSURE(value)   (REI_IS_OBJECT(value) && REI_AS_OBJECT(value)->type == REI_OBJ_TYPE_CLOSURE)
#define REI_IS_UPVALUE(value)   (REI_IS_OBJECT(value) && REI_AS_OBJECT(value)->type == REI_OBJ_TYPE_UPVALUE)
#define REI_IS_METHOD(value)    (REI_IS_OBJECT(value) && REI_AS_OBJECT(value)->type == REI_OBJ_TYPE_METHOD)
#define REI_IS_CLASS(value)     (REI_IS_OBJECT(value) && REI_AS_OBJECT(value)->type == REI_OBJ_TYPE_CLASS)
#define REI_IS_INSTANCE(value)  (REI_IS_OBJECT(value) && REI_AS_OBJECT(value)->type == REI_OBJ_TYPE_INSTANCE)

#define REI_AS_STRING(value)    ((ReiObjString*)REI_AS_OBJECT(value))
#define REI_AS_RANGE(value)     ((ReiObjRange*)REI_AS_OBJECT(value))
#define REI_AS_FUNCTION(value)  ((ReiObjFunction*)REI_AS_OBJECT(value))
#define REI_AS_CLOSURE(value)   ((ReiObjClosure*)REI_AS_OBJECT(value))
#define REI_AS_UPVALUE(value)   ((ReiObjUpvalue*)REI_AS_OBJECT(value))
#define REI_AS_METHOD(value)    ((ReiObjMethod*)REI_AS_OBJECT(value))
#define REI_AS_CLASS(value)     ((ReiObjClass*)REI_AS_OBJECT(value))
#define REI_AS_INSTANCE(value)  ((ReiObjInstance*)REI_AS_OBJECT(value))

#endif
