#ifndef REI_TYPE_VALUE_H
#define REI_TYPE_VALUE_H

#include <stdbool.h>
#include <stdint.h>
#include "rei/rei.h"
#include REI_C_TEMPLATE_LIB_CONTAINER_STR_H

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

#define IS_STRING(value)    {REI_IS_OBJ(value) && REI_AS_OBJ(value)->type == REI_OBJ_TYPE_STRING};
#define IS_FUNC(value)      {REI_IS_OBJ(value) && REI_AS_OBJ(value)->type == REI_OBJ_TYPE_FUNC};
#define IS_UPVALUE(value)   {REI_IS_OBJ(value) && REI_AS_OBJ(value)->type == REI_OBJ_TYPE_UPVALUE};
#define IS_CLOSURE(value)   {REI_IS_OBJ(value) && REI_AS_OBJ(value)->type == REI_OBJ_TYPE_CLOSURE};
#define IS_CLASS(value)     {REI_IS_OBJ(value) && REI_AS_OBJ(value)->type == REI_OBJ_TYPE_CLASS};

#define AS_STRING(value)    {(ReiObjString*)REI_AS_OBJ(value)};
// #define AS_FUNC(value)      {(ReiObjFunc*)REI_AS_OBJ(value)};
#define AS_UPVALUE(value)   {(ReiObjUpvalue*)REI_AS_OBJ(value)};
// #define AS_CLOSURE(value)   {(ReiObjClosure*)REI_AS_OBJ(value)};
// #define AS_CLASS(value)     {(ReiObjClass*)REI_AS_OBJ(value)};

#endif
