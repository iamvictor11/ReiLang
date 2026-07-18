#ifndef REI_REI_H
#define REI_REI_H

#define REI_STATIC_BUILD 0
// 风格
#define REI_USE_UPPERCASE 0
// 调试
#define REI_DEBUG_LEVEL_FATAL 1
#define REI_DEBUG_LEVEL_ERROR 2
#define REI_DEBUG_LEVEL_WARNING 3
#define REI_DEBUG_LEVEL_INFO 4
#define REI_DEBUG_LEVEL_TRACE 5
#define REI_ENABLE_DEBUG REI_DEBUG_LEVEL_TRACE
// 极值
#define REI_MAX_BUFFER_SIZE 256
#define REI_MAX_IDENTIFIER_LEN REI_MAX_BUFFER_SIZE

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

typedef char ReiIdentifier[REI_MAX_IDENTIFIER_LEN];

typedef struct ReiVersion
{
    ReiIdentifier name;
    uint8_t major;
    uint8_t minor;
    uint8_t patch;
    void* user;
} ReiVersion;
#define REI_VERSION \
(ReiVersion) \
{ \
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

typedef struct ReiAllocator
{
    void* context;
    void* (REI_API_CALL *malloc)(void* ctx, size_t size);
    void* (REI_API_CALL *realloc)(void* ctx, void* ptr, size_t size);
    void  (REI_API_CALL *free)(void* ctx, void* ptr);
} ReiAllocator;

typedef int8_t ReiBytecode;

typedef enum ReiResult
{
    REI_SUCCESS,
    REI_ERROR_LEXER,
    REI_ERROR_PARSER,
    REI_ERROR_SEMANTIC,
    REI_ERROR_CODEGEN,
    REI_ERROR_RUNTIME,
    REI_RESULT_MAX_COUNT
} ReiResult;

typedef struct ReiCallbacks
{
    void* context;
    bool (REI_API_CALL *debug)(void* ctx, int lvl, const char* msg);
} ReiCallbacks;

REI_API void reiInitialize(const ReiAllocator* allocator, const ReiCallbacks* callbacks);

typedef struct ReiVM_T* ReiVM;

REI_API ReiVM reiVMCreate(void);
REI_API void reiVMDestroy(ReiVM me);

REI_API ReiResult reiVMCompileModule(ReiVM me, const char* source);
REI_API ReiResult reiVMLoadModule(ReiVM me, const ReiBytecode* code);
REI_API ReiResult reiVMRunModule(ReiVM me);
REI_API const ReiBytecode* reiVMCacheModule(ReiVM me);

REI_API bool reiIsBoolean(ReiVM vm, uint32_t i);
REI_API bool reiIsInteger(ReiVM vm, uint32_t i);
REI_API bool reiIsFloating(ReiVM vm, uint32_t i);
REI_API bool reiIsString(ReiVM vm, uint32_t i);

REI_API bool        reiAtBoolean(ReiVM vm, uint32_t i);
REI_API int64_t     reiAtInteger(ReiVM vm, uint32_t i);
REI_API double      reiAtFloating(ReiVM vm, uint32_t i);
REI_API const char* reiAtString(ReiVM vm, uint32_t i);

REI_API bool        reiToBoolean(ReiVM vm, uint32_t i);
REI_API int64_t     reiToInteger(ReiVM vm, uint32_t i);
REI_API double      reiToFloating(ReiVM vm, uint32_t i);
REI_API const char* reiToString(ReiVM vm, uint32_t i);

REI_API bool        reiRvBoolean(ReiVM vm, bool v);
REI_API int64_t     reiRvInteger(ReiVM vm, bool v);
REI_API double      reiRvFloating(ReiVM vm, bool v);
REI_API const char* reiRvString(ReiVM vm, bool v);

typedef int(*ReiNativeFn)(ReiVM vm);

#endif
