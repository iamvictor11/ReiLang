#ifndef REI_TYPE_OBJ_H
#define REI_TYPE_OBJ_H

#include <stdint.h>
#include <stdbool.h>
#include "../common/common.h"
#include "value.h"

typedef struct ReiObjClass ReiObjClass;

typedef enum ReiObjCategory
{
    REI_OBJ_CATEGORY_ARRAY,
    REI_OBJ_CATEGORY_BOUND_METHOD,
    REI_OBJ_CATEGORY_CLASS,
    REI_OBJ_CATEGORY_CLOSURE,
    REI_OBJ_CATEGORY_DICTIONARY,
    REI_OBJ_CATEGORY_ENTRY,
    REI_OBJ_CATEGORY_EXCEPTION,
    REI_OBJ_CATEGORY_FILE,
    REI_OBJ_CATEGORY_FRAME,
    REI_OBJ_CATEGORY_FUNCTION,
    REI_OBJ_CATEGORY_GENERATOR,
    REI_OBJ_CATEGORY_INSTANCE,
    REI_OBJ_CATEGORY_ITERATOR,
    REI_OBJ_CATEGORY_METHOD,
    REI_OBJ_CATEGORY_MODULE,
    REI_OBJ_CATEGORY_NAMESPACE,
    REI_OBJ_CATEGORY_NATIVE_FUNCTION,
    REI_OBJ_CATEGORY_NATIVE_METHOD,
    REI_OBJ_CATEGORY_NODE,
    REI_OBJ_CATEGORY_PROMISE,
    REI_OBJ_CATEGORY_RANGE,
    REI_OBJ_CATEGORY_RECORD,
    REI_OBJ_CATEGORY_STRING,
    REI_OBJ_CATEGORY_TIMER,
    REI_OBJ_CATEGORY_TYPE,
    REI_OBJ_CATEGORY_UPVALUE,
    REI_OBJ_CATEGORY_VALUE_INSTANCE,
    REI_OBJ_CATEGORY_VOID
} ReiObjCategory;

typedef struct ReiObj
{
    uint64_t objectID;
    int shapeID;
    ReiObjCategory category;
    ReiObjClass* klass;
    bool isMarked;
    ReiLifeCycle lifeCycle;
    struct ReiObj* next;
} ReiObj;
typedef struct ReiObjInstance
{
    ReiObj obj;
    ReiValueVector fields;
} ReiObjInstance;

#endif
