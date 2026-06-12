#ifndef REI_REI_H
#define REI_REI_H

#define REI_C_TEMPLATE_LIB_CONTAINER_LIST_DOUBLY_H  "c_template/container/list.doubly.h"
#define REI_C_TEMPLATE_LIB_CONTAINER_LIST_SINGLY_H  "c_template/container/list.singly.h"
#define REI_C_TEMPLATE_LIB_CONTAINER_STR_H          "c_template/container/str.h"
#define REI_C_TEMPLATE_LIB_CONTAINER_VECTOR_H       "c_template/container/vector.h"
#define REI_C_TEMPLATE_LIB_PLATFORM_CONSOLE_H       "c_template/platform/console.h"
#define REI_C_TEMPLATE_LIB_PLATFORM_KUA_H           "c_template/platform/kua.h"
#define REI_C_TEMPLATE_LIB_TYPE_UUID_H              "c_template/type/uuid.h"
#define REI_C_TEMPLATE_LIB_UTILS_RANDOM_GEN_H       "c_template/utils/random_gen.h"
#define REI_C_TEMPLATE_LIB_ALLOCATOR_H              "c_template/allocator.h"
#define REI_C_TEMPLATE_LIB_VERSION_H                "c_template/version.h"

#define REI_MAX_IDENTIFIER_NAME_LEN 255
#define REI_NOT_USE_UPPERCASE

#include "stdint.h"
#include "stdbool.h"
#include REI_C_TEMPLATE_LIB_ALLOCATOR_H

C_TEMPLATE_DECL_ALLOCATOR(static inline, rei, Rei)
C_TEMPLATE_DEFN_ALLOCATOR(static inline, rei, Rei)
C_TEMPLATE_IMPL_ALLOCATOR(static inline, rei, Rei)
extern ReiAllocator reiAllocator_g;

typedef int8_t ReiBytecode;
typedef struct ReiVM_T* ReiVM;
typedef struct ReiObj ReiObj;

typedef enum ReiStatus
{
    REI_STATUS_SUCCESS,
    REI_STATUS_LEXER_ERROR,
    REI_STATUS_PARSER_ERROR,
    REI_STATUS_SEMANTIC_ERROR,
    REI_STATUS_CODEGEN_ERROR,
    REI_STATUS_RUNTIME_ERROR,
    REI_STATUS_UNDEFINED
} ReiStatus;

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

#define REI_MK_NIL          ((ReiValue){REI_VALUE_TYPE_NIL,     {.vBool = false}})
#define REI_MK_BOOL(v)      ((ReiValue){REI_VALUE_TYPE_BOOL,    {.vBool = v}})
#define REI_MK_INT(v)       ((ReiValue){REI_VALUE_TYPE_INT,     {.vInt = v}})
#define REI_MK_FLOAT(v)     ((ReiValue){REI_VALUE_TYPE_FLOAT,   {.vFloat = v}})
#define REI_MK_NUMBER(v)    ((ReiValue){REI_VALUE_TYPE_FLOAT,   {.vFloat = v}})
#define REI_MK_OBJ(p)       ((ReiValue){REI_VALUE_TYPE_OBJ,     {.pObj = (ReiObj*)p}})

typedef struct ReiResult
{
    ReiStatus status;
    ReiValue value;
} ReiResult;
typedef ReiResult(*ReiNativeFunc)(ReiVM vm, int argc, ReiValue* args);


ReiVM reiVMCreate(void);
void reiVMDestroy(ReiVM me);

ReiResult reiVMCompileModule(ReiVM me, const char* source);
ReiResult reiVMLoadModule(ReiVM me, const ReiBytecode* code);
ReiResult reiVMRunModule(ReiVM me);
const ReiBytecode* reiVMCacheModule(ReiVM me);

#endif
