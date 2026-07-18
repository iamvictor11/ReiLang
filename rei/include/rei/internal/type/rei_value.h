#ifndef REI_TYPE_VALUE_H
#define REI_TYPE_VALUE_H

#include "rei/rei.h"
#include "rei/internal/utils/rei_string.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct ReiObject ReiObject;
#define REI_DECL_OBJECT(NAME) typedef struct ReiObj##NAME ReiObj##NAME;
REI_DECL_OBJECT(String)     // "" ''
REI_DECL_OBJECT(Range)      // i..n
REI_DECL_OBJECT(Function)
REI_DECL_OBJECT(Closure)
REI_DECL_OBJECT(Upvalue)
REI_DECL_OBJECT(Class)
REI_DECL_OBJECT(Instance)
REI_DECL_OBJECT(Method)
REI_DECL_OBJECT(Module)
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

#define REI_IS_NIL(VALUE)       ((VALUE).type == REI_VALUE_TYPE_NIL)
#define REI_IS_BOOLEAN(VALUE)   ((VALUE).type == REI_VALUE_TYPE_BOOLEAN)
#define REI_IS_INTEGER(VALUE)   ((VALUE).type == REI_VALUE_TYPE_INTEGER)
#define REI_IS_FLOATING(VALUE)  ((VALUE).type == REI_VALUE_TYPE_FLOATING)
#define REI_IS_NUMBER(VALUE)    ((VALUE).type == REI_VALUE_TYPE_INTEGER || (VALUE).type == REI_VALUE_TYPE_FLOATING)
#define REI_IS_OBJECT(VALUE)    ((VALUE).type == REI_VALUE_TYPE_OBJECT)
#define REI_IS_NULL(VALUE)      ((VALUE).type == REI_VALUE_TYPE_OBJECT && (VALUE).pObject == NULL)
#define REI_IS_UNDEFINED(VALUE) ((VALUE).type == REI_VALUE_TYPE_UNDEFINED)

#define REI_AS_BOOLEAN(VALUE)   ((VALUE).as.vBoolean)
#define REI_AS_INTEGER(VALUE)   ((VALUE).as.vInteger)
#define REI_AS_FLOATING(VALUE)  ((VALUE).as.vFloating)
#define REI_AS_NUMBER(VALUE)    ((VALUE).as.vFloating)
#define REI_AS_OBJECT(VALUE)    ((VALUE).as.pObject)

#define REI_MK_NIL              ((ReiValue){REI_VALUE_TYPE_NIL,         {.vBoolean = false}})
#define REI_MK_BOOLEAN(VAL)     ((ReiValue){REI_VALUE_TYPE_BOOLEAN,     {.vBoolean = VAL}})
#define REI_MK_INTEGER(VAL)     ((ReiValue){REI_VALUE_TYPE_INTEGER,     {.vInteger = VAL}})
#define REI_MK_FLOATING(VAL)    ((ReiValue){REI_VALUE_TYPE_FLOATING,    {.vFloating = VAL}})
#define REI_MK_NUMBER(VAL)      ((ReiValue){REI_VALUE_TYPE_FLOATING,    {.vFloating = VAL}})
#define REI_MK_OBJECT(PTR)      ((ReiValue){REI_VALUE_TYPE_OBJECT,      {.pObject = (ReiObject*)(PTR)}})

REI_DECL_VECTOR(, rei, Rei, ByteBuffer, ReiBytecode)
REI_DEFN_VECTOR(, rei, Rei, ByteBuffer, ReiBytecode)
REI_DECL_VECTOR(, rei, Rei, UIntBuffer, uint32_t)
REI_DEFN_VECTOR(, rei, Rei, UIntBuffer, uint32_t)
REI_DECL_VECTOR(, rei, Rei, ValueBuffer, ReiValue)
REI_DEFN_VECTOR(, rei, Rei, ValueBuffer, ReiValue)
REI_DECL_STRING(, rei, Rei, String)
REI_DEFN_STRING(, rei, Rei, String)

typedef enum ReiObjType
{
    REI_OBJ_TYPE_STRING,
    REI_OBJ_TYPE_RANGE,
    REI_OBJ_TYPE_FUNCTION,
    REI_OBJ_TYPE_CLOSURE,
    REI_OBJ_TYPE_UPVALUE,
    REI_OBJ_TYPE_CLASS,
    REI_OBJ_TYPE_INSTANCE,
    REI_OBJ_TYPE_METHOD,
    REI_OBJ_TYPE_MODULE,
    REI_OBJ_TYPE_MAX_COUNT
} ReiObjType;
typedef struct ReiObject
{
    ReiObjType type;
    bool isMarked;
    ReiObject* next;
} ReiObject;

#define REI_IS_STRING(VALUE)    (REI_IS_OBJECT(VALUE) && REI_AS_OBJECT(VALUE)->type == REI_OBJ_TYPE_STRING)
#define REI_IS_RANGE(VALUE)     (REI_IS_OBJECT(VALUE) && REI_AS_OBJECT(VALUE)->type == REI_OBJ_TYPE_RANGE)
#define REI_IS_FUNCTION(VALUE)  (REI_IS_OBJECT(VALUE) && REI_AS_OBJECT(VALUE)->type == REI_OBJ_TYPE_FUNCTION)
#define REI_IS_CLOSURE(VALUE)   (REI_IS_OBJECT(VALUE) && REI_AS_OBJECT(VALUE)->type == REI_OBJ_TYPE_CLOSURE)
#define REI_IS_UPVALUE(VALUE)   (REI_IS_OBJECT(VALUE) && REI_AS_OBJECT(VALUE)->type == REI_OBJ_TYPE_UPVALUE)
#define REI_IS_CLASS(VALUE)     (REI_IS_OBJECT(VALUE) && REI_AS_OBJECT(VALUE)->type == REI_OBJ_TYPE_CLASS)
#define REI_IS_INSTANCE(VALUE)  (REI_IS_OBJECT(VALUE) && REI_AS_OBJECT(VALUE)->type == REI_OBJ_TYPE_INSTANCE)
#define REI_IS_METHOD(VALUE)    (REI_IS_OBJECT(VALUE) && REI_AS_OBJECT(VALUE)->type == REI_OBJ_TYPE_METHOD)
#define REI_IS_MODULE(VALUE)    (REI_IS_OBJECT(VALUE) && REI_AS_OBJECT(VALUE)->type == REI_OBJ_TYPE_MODULE)

#define REI_AS_STRING(VALUE)    ((ReiObjString*)REI_AS_OBJECT(VALUE))
#define REI_AS_RANGE(VALUE)     ((ReiObjRange*)REI_AS_OBJECT(VALUE))
#define REI_AS_FUNCTION(VALUE)  ((ReiObjFunction*)REI_AS_OBJECT(VALUE))
#define REI_AS_CLOSURE(VALUE)   ((ReiObjClosure*)REI_AS_OBJECT(VALUE))
#define REI_AS_UPVALUE(VALUE)   ((ReiObjUpvalue*)REI_AS_OBJECT(VALUE))
#define REI_AS_CLASS(VALUE)     ((ReiObjClass*)REI_AS_OBJECT(VALUE))
#define REI_AS_INSTANCE(VALUE)  ((ReiObjInstance*)REI_AS_OBJECT(VALUE))
#define REI_AS_METHOD(VALUE)    ((ReiObjMethod*)REI_AS_OBJECT(VALUE))
#define REI_AS_MODULE(VALUE)    ((ReiObjModule*)REI_AS_OBJECT(VALUE))

#endif
