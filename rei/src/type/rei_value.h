#ifndef REI_TYPE_VALUE_H
#define REI_TYPE_VALUE_H

#include <stdbool.h>
#include <stdint.h>
#include "rei/rei.h"
#include "rei_template/container/str.h"

typedef struct ReiObj ReiObj;

typedef enum ReiValueType
{
    REI_VALUE_TYPE_NIL,
    REI_VALUE_TYPE_BOOL,
    REI_VALUE_TYPE_INT,
    REI_VALUE_TYPE_FLOAT,
    REI_VALUE_TYPE_OBJ,
    REI_VALUE_TYPE_UNDEFINED
} ReiValueType;
typedef struct ReiValue
{
    ReiValueType type;
    union
    {
        bool vBool;
        int64_t vInt;
        double vFloat;
        ReiObj* pObj;
    } as;
} ReiValue;

#define REI_IS_NIL(value)       ((value).type == REI_VALUE_TYPE_NIL)
#define REI_IS_BOOL(value)      ((value).type == REI_VALUE_TYPE_BOOL)
#define REI_IS_INT(value)       ((value).type == REI_VALUE_TYPE_INT)
#define REI_IS_FLOAT(value)     ((value).type == REI_VALUE_TYPE_FLOAT)
#define REI_IS_NUMBER(value)    ((value).type == REI_VALUE_TYPE_INT || (value).type == REI_VALUE_TYPE_FLOAT)
#define REI_IS_OBJ(value)       ((value).type == REI_VALUE_TYPE_OBJ)
#define REI_IS_UNDEFINED(value) ((value).type == REI_VALUE_TYPE_UNDEFINED)

#define REI_AS_BOOL(value)      ((value).as.vBool)
#define REI_AS_INT(value)       ((value).as.vInt)
#define REI_AS_FLOAT(value)     ((value).as.vFloat)
#define REI_AS_NUMBER(value)    ((value).as.vFloat)
#define REI_AS_OBJ(value)       ((value).as.pObj)

#define REI_MK_NIL          ((Value){VAL_NIL,   {.vBool = false}})
#define REI_MK_BOOL(v)      ((Value){VAL_BOOL,  {.vBool = v}})
#define REI_MK_INT(v)       ((Value){VAL_INT,   {.vInt = v}})
#define REI_MK_FLOAT(v)     ((Value){VAL_FLOAT, {.vFloat = v}})
#define REI_MK_NUMBER(v)    ((Value){VAL_FLOAT, {.vFloat = v}})
#define REI_MK_OBJ(v)       ((Value){VAL_OBJ,   {.pObj = (Obj*)v}})

C_TEMPLATE_DECL_VECTOR(, rei, Rei, ValueBuffer, ReiValue)
C_TEMPLATE_DEFN_VECTOR(, rei, Rei, ValueBuffer, ReiValue)

char* reiValueToCstr(ReiValue value);


typedef enum ReiObjType
{
    REI_OBJ_TYPE_STRING,
    REI_OBJ_TYPE_FUNC,
    REI_OBJ_TYPE_UPVALUE,
    REI_OBJ_TYPE_CLOSURE,
    REI_OBJ_TYPE_CLASS
} ReiObjType;
typedef struct ReiObj
{
    ReiObjType type;
    struct ReiObj* next;
} ReiObj;

C_TEMPLATE_DECL_STRING(, rei, Rei, String)
C_TEMPLATE_DEFN_STRING(, rei, Rei, String)
typedef struct ReiStringView
{
    ReiString data;
    uint32_t offset;
    uint32_t length;
} ReiStringView;
typedef struct ReiObjString
{
    ReiObj obj;
    ReiString_T data;
    uint32_t hash;
} ReiObjString;
typedef struct ReiObjUpvalue
{
    ReiObj obj;
    ReiValue* value;
    ReiValue closed;
    struct ReiObjUpvalue* next;
} ReiObjUpvalue;
typedef struct ReiResult
{
    ReiStatus status;
    ReiValue value;
} ReiResult;
typedef ReiResult(*ReiNativeFunc)(ReiVM vm, int argc, ReiValue* args);


#define IS_STRING(value)    {REI_IS_OBJ(value) && REI_AS_OBJ(value)->type == REI_OBJ_TYPE_STRING};
#define IS_FUNC(value)      {REI_IS_OBJ(value) && REI_AS_OBJ(value)->type == REI_OBJ_TYPE_FUNC};
#define IS_UPVALUE(value)   {REI_IS_OBJ(value) && REI_AS_OBJ(value)->type == REI_OBJ_TYPE_UPVALUE};
#define IS_CLOSURE(value)   {REI_IS_OBJ(value) && REI_AS_OBJ(value)->type == REI_OBJ_TYPE_CLOSURE};
#define IS_CLASS(value)     {REI_IS_OBJ(value) && REI_AS_OBJ(value)->type == REI_OBJ_TYPE_CLASS};

#define AS_STRING(value)    {(ObjString*)REI_AS_OBJ(value)};
#define AS_FUNC(value)      {(ObjFunc*)REI_AS_OBJ(value)};
#define AS_UPVALUE(value)   {(ObjUpvalue*)REI_AS_OBJ(value)};
#define AS_CLOSURE(value)   {(ObjClosure*)REI_AS_OBJ(value)};
#define AS_CLASS(value)     {(ObjClass*)REI_AS_OBJ(value)};

#endif
