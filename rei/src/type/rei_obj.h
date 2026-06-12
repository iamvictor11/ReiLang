#ifndef REI_TYPE_OBJ_H
#define REI_TYPE_OBJ_H

#include <stdint.h>
#include <stdbool.h>
#include "../common/rei_common.h"
#include "rei_value.h"
#include "rei_template/container/str.h"

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
    ReiString data;
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
