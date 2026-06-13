#ifndef REI_TYPE_VALUE_H
#define REI_TYPE_VALUE_H

#include "rei_internal.h"
#include <stdbool.h>
#include <stdint.h>
#include REI_C_TEMPLATE_LIB_CONTAINER_STR_H

typedef struct ReiObject        ReiObject;
typedef struct ReiObjString     ReiObjString;   // "" ''
typedef struct ReiObjRange      ReiObjRange;    // i..n
typedef struct ReiObjModule     ReiObjModule;
typedef struct ReiObjFunction   ReiObjFunction;
typedef struct ReiObjClosure    ReiObjClosure;
typedef struct ReiObjUpvalue    ReiObjUpvalue;
typedef struct ReiObjMethod     ReiObjMethod;
typedef struct ReiObjClass      ReiObjClass;
typedef struct ReiObjInstance   ReiObjInstance;

typedef enum ReiValueType
{
    REI_VALUE_TYPE_NIL,
    REI_VALUE_TYPE_BOOL,
    REI_VALUE_TYPE_INT,
    REI_VALUE_TYPE_FLOAT,
    REI_VALUE_TYPE_ENUM,
    REI_VALUE_TYPE_FLAG,
    REI_VALUE_TYPE_OBJECT,
    REI_VALUE_TYPE_UNDEFINED,
    REI_VALUE_TYPE_MAX_COUNT
} ReiValueType;
typedef struct ReiValue
{
    ReiValueType type;
    union
    {
        bool vBool;
        int64_t vInt;
        double vFloat;
        ReiObject* pObject;
    } as;
} ReiValue;

#define REI_IS_NIL(value)       ((value).type == REI_VALUE_TYPE_NIL)
#define REI_IS_BOOL(value)      ((value).type == REI_VALUE_TYPE_BOOL)
#define REI_IS_INT(value)       ((value).type == REI_VALUE_TYPE_INT)
#define REI_IS_FLOAT(value)     ((value).type == REI_VALUE_TYPE_FLOAT)
#define REI_IS_NUMBER(value)    ((value).type == REI_VALUE_TYPE_INT || (value).type == REI_VALUE_TYPE_FLOAT)
#define REI_IS_ENUM(value)      ((value).type == REI_VALUE_TYPE_ENUM)
#define REI_IS_FLAG(value)      ((value).type == REI_VALUE_TYPE_FLAG)
#define REI_IS_OBJECT(value)    ((value).type == REI_VALUE_TYPE_OBJECT)
#define REI_IS_UNDEFINED(value) ((value).type == REI_VALUE_TYPE_UNDEFINED)

#define REI_AS_BOOL(value)      ((value).as.vBool)
#define REI_AS_INT(value)       ((value).as.vInt)
#define REI_AS_FLOAT(value)     ((value).as.vFloat)
#define REI_AS_NUMBER(value)    ((value).as.vFloat)
#define REI_AS_ENUM(value)      ((value).as.vInt)
#define REI_AS_FLAG(value)      ((value).as.vInt)
#define REI_AS_OBJECT(value)    ((value).as.pObject)

#define REI_MK_NIL          ((ReiValue){REI_VALUE_TYPE_NIL,     {.vBool = false}})
#define REI_MK_BOOL(v)      ((ReiValue){REI_VALUE_TYPE_BOOL,    {.vBool = v}})
#define REI_MK_INT(v)       ((ReiValue){REI_VALUE_TYPE_INT,     {.vInt = v}})
#define REI_MK_FLOAT(v)     ((ReiValue){REI_VALUE_TYPE_FLOAT,   {.vFloat = v}})
#define REI_MK_NUMBER(v)    ((ReiValue){REI_VALUE_TYPE_FLOAT,   {.vFloat = v}})
#define REI_MK_ENUM(v)      ((ReiValue){REI_VALUE_TYPE_ENUM,    {.vInt = (int64_t)v}})
#define REI_MK_FLAG(v)      ((ReiValue){REI_VALUE_TYPE_FLAG,    {.vInt = (int64_t)v}})
#define REI_MK_OBJECT(p)    ((ReiValue){REI_VALUE_TYPE_OBJECT,  {.pObject = (ReiObject*)p}})

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
    REI_OBJ_TYPE_LIST,
    REI_OBJ_TYPE_MAP,
    REI_OBJ_TYPE_RANGE,
    REI_OBJ_TYPE_MODULE,
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
#define REI_IS_MODULE(value)    (REI_IS_OBJECT(value) && REI_AS_OBJECT(value)->type == REI_OBJ_TYPE_MODULE)
#define REI_IS_FUNCTION(value)  (REI_IS_OBJECT(value) && REI_AS_OBJECT(value)->type == REI_OBJ_TYPE_FUNCTION)
#define REI_IS_CLOSURE(value)   (REI_IS_OBJECT(value) && REI_AS_OBJECT(value)->type == REI_OBJ_TYPE_CLOSURE)
#define REI_IS_UPVALUE(value)   (REI_IS_OBJECT(value) && REI_AS_OBJECT(value)->type == REI_OBJ_TYPE_UPVALUE)
#define REI_IS_METHOD(value)    (REI_IS_OBJECT(value) && REI_AS_OBJECT(value)->type == REI_OBJ_TYPE_METHOD)
#define REI_IS_CLASS(value)     (REI_IS_OBJECT(value) && REI_AS_OBJECT(value)->type == REI_OBJ_TYPE_CLASS)
#define REI_IS_INSTANCE(value)  (REI_IS_OBJECT(value) && REI_AS_OBJECT(value)->type == REI_OBJ_TYPE_INSTANCE)

#define REI_AS_STRING(value)    ((ReiObjString*)REI_AS_OBJECT(value))
#define REI_AS_RANGE(value)     ((ReiObjRange*)REI_AS_OBJECT(value))
#define REI_AS_MODULE(value)    ((ReiObjModule*)REI_AS_OBJECT(value))
#define REI_AS_FUNCTION(value)  ((ReiObjFunction*)REI_AS_OBJECT(value))
#define REI_AS_CLOSURE(value)   ((ReiObjClosure*)REI_AS_OBJECT(value))
#define REI_AS_UPVALUE(value)   ((ReiObjUpvalue*)REI_AS_OBJECT(value))
#define REI_AS_METHOD(value)    ((ReiObjMethod*)REI_AS_OBJECT(value))
#define REI_AS_CLASS(value)     ((ReiObjClass*)REI_AS_OBJECT(value))
#define REI_AS_INSTANCE(value)  ((ReiObjInstance*)REI_AS_OBJECT(value))

#endif
