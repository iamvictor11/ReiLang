#ifndef REI_REI_H
#define REI_REI_H

#define REI_STATIC_BUILD 0
// 第三方库
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
// 风格
#define REI_USE_UPPERCASE 1
// 调试
#define REI_DEBUG_LEVEL_FATAL 1
#define REI_DEBUG_LEVEL_ERROR 2
#define REI_DEBUG_LEVEL_WARNING 3
#define REI_DEBUG_LEVEL_INFO 4
#define REI_DEBUG_LEVEL_TRACE 5
#define REI_ENABLE_DEBUG REI_DEBUG_LEVEL_TRACE
// 极值
#define REI_MAX_BUFFER_SIZE 256
#define REI_MAX_IDENTIFIER_NAME_LEN REI_MAX_BUFFER_SIZE

#if defined(_WIN32) || defined(_WIN64)
    #if REI_STATIC_BUILD
        #define REI_API
        #define REI_API_CALL
    #else
        #ifdef REI_LIB_INTERNAL
            #define REI_API __declspec(dllexport)
        #else
            #define REI_API __declspec(dllimport)
        #endif
        #ifdef _MSC_VER
            #define REI_API_CALL __cdecl
        #elif defined(__GNUC__) || defined(__clang__)
            #define REI_API_CALL __attribute__((cdecl))
        #else
            #define REI_API_CALL
        #endif
    #endif
#elif defined(__linux__) || defined(__APPLE__)
    #if REI_STATIC_BUILD
        #define REI_API
    #else
        #ifdef REI_LIB_INTERNAL
            #define REI_API __attribute__((visibility("default")))
        #else
            #define REI_API
        #endif
    #endif
    #define REI_API_CALL
#else
    #define REI_API
    #define REI_API_CALL
#endif

#include "stdint.h"
#include "stdbool.h"
#include REI_C_TEMPLATE_LIB_VERSION_H
#include REI_C_TEMPLATE_LIB_ALLOCATOR_H

C_TEMPLATE_VERSION_STRUCT(,rei, Rei)
#define REI_VERSION \
(ReiVersion) { \
    .name = "REI", \
    .major = 1, \
    .minor = 1, \
    .patch = 1, \
    .user = NULL \
}
#define REI_VERSION_TO_UINT(MAJOR, MINOR, PATCH) C_TEMPLATE_VERSION_TO_UINT(MAJOR, MINOR, PATCH)
#define REI_VERSION_CAST_UINT(VERSION) C_TEMPLATE_VERSION_CAST_UINT(VERSION)
#define REI_UINT_TO_MAJOR(VERSION) C_TEMPLATE_UINT_TO_MAJOR(VERSION)
#define REI_UINT_TO_MINOR(VERSION) C_TEMPLATE_UINT_TO_MINOR(VERSION)
#define REI_UINT_TO_PATCH(VERSION) C_TEMPLATE_UINT_TO_PATCH(VERSION)

C_TEMPLATE_DECL_ALLOCATOR(REI_API, rei, Rei)
C_TEMPLATE_DEFN_ALLOCATOR(REI_API, rei, Rei, REI_API_CALL)

typedef int8_t ReiBytecode;

typedef enum ReiResult
{
    REI_RESULT_SUCCESS,
    REI_RESULT_LEXER_ERROR,
    REI_RESULT_PARSER_ERROR,
    REI_RESULT_SEMANTIC_ERROR,
    REI_RESULT_CODEGEN_ERROR,
    REI_RESULT_RUNTIME_ERROR,
    REI_RESULT_MAX_COUNT
} ReiResult;

typedef struct ReiCallbacks
{
    void* context;
    bool (REI_API_CALL *debug)(void* ctx, ReiResult res, const char* msg);
} ReiCallbacks;

REI_API void reiInitialize(const ReiAllocator* allocator, const ReiCallbacks* callbacks);

typedef struct ReiVM_T* ReiVM;

REI_API ReiVM reiVMCreate(void);
REI_API void reiVMDestroy(ReiVM me);

REI_API ReiResult reiVMCompileModule(ReiVM me, const char* source);
REI_API ReiResult reiVMLoadModule(ReiVM me, const ReiBytecode* code);
REI_API ReiResult reiVMRunModule(ReiVM me);
REI_API const ReiBytecode* reiVMCacheModule(ReiVM me);

typedef void (*ReiNativeFn)(ReiVM vm);
typedef void* (*ReiNewInstanceFn) (ReiVM* vm);
typedef void (*ReiDelInstanceFn) (ReiVM* vm, void* user);

#endif
