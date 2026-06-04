#ifndef REI_TYPE_VALUE_H
#define REI_TYPE_VALUE_H

#include <stdbool.h>
#include <stdint.h>
#include "template/container/vector.h"

typedef struct ReiObj ReiObj;

typedef enum ReiValueType
{
    REI_VALUE_TYPE_NIL,
    REI_VALUE_TYPE_BOOL,
    REI_VALUE_TYPE_INT,
    REI_VALUE_TYPE_FLOAT,
    REI_VALUE_TYPE_OBJ
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

C_TEMPLATE_DECL_VECTOR(, rei, Rei, ValueVector, ReiValue)
C_TEMPLATE_DEFN_VECTOR(, rei, Rei, ValueVector, ReiValue)

char* reiValueToCstr(ReiValue value);

#endif
