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

#define REI_NOT_USE_UPPERCASE

#include "stdint.h"
#include REI_C_TEMPLATE_LIB_ALLOCATOR_H

C_TEMPLATE_DECL_ALLOCATOR(static inline, rei, Rei)
C_TEMPLATE_DEFN_ALLOCATOR(static inline, rei, Rei)
C_TEMPLATE_IMPL_ALLOCATOR(static inline, rei, Rei)
extern ReiAllocator reiAllocator_g;

typedef int8_t ReiBytecode;
typedef enum ReiStatus
{
    REI_RESULT_SUCCESS,
    REI_RESULT_COMPILE_ERROR,
    REI_RESULT_RUNTIME_ERROR
} ReiStatus;
typedef struct ReiVM_T ReiVM_T;
typedef struct ReiVM_T* ReiVM;

ReiVM reiVMCreate(void);
void reiVMDestroy(ReiVM me);

ReiStatus reiVMCompileModule(ReiVM me, const char* source);
ReiStatus reiVMLoadModule(ReiVM me, const ReiBytecode* code);
ReiStatus reiVMRunModule(ReiVM me);
const ReiBytecode* reiVMCacheModule(ReiVM me);

#endif
