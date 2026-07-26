#pragma once

extern "C"
{
#include <stdint.h>
#include <stdlib.h>

typedef struct ReiAllocationCallbacks
{
    void* context;
    void* (*malloc)(void* ctx, size_t size);
    void* (*realloc)(void* ctx, void* ptr, size_t size);
    void (*free)(void* ctx, void* ptr);
} ReiAllocationCallbacks;
#define reiMalloc(CALLBACKS, TYPE, COUNT)       ((TYPE*)(CALLBACKS).malloc((CALLBACKS).context, sizeof(TYPE) * COUNT))
#define reiRealloc(CALLBACKS, TYPE, PTR, COUNT) ((TYPE*)(CALLBACKS).realloc((CALLBACKS).context, PTR, sizeof(TYPE) * COUNT))
#define reiFree(CALLBACKS, PTR)                 ((CALLBACKS).free((CALLBACKS).context, PTR))
static inline void* reiAllocationCallbacksDefaultMalloc(void* ctx, size_t size)
{
    (void)ctx;
    return malloc(size);
}
static inline void* reiAllocationCallbacksDefaultRealloc(void* ctx, void* ptr, size_t size)
{
    (void)ctx;
    return realloc(ptr, size);
}
static inline void reiAllocationCallbacksDefaultFree(void* ctx, void* ptr)
{
    (void)ctx;
    free(ptr);
}
#define REI_DEFAULT_ALLOCATION_CALLBACKS \
    (ReiAllocationCallbacks) \
    { \
        .context = NULL, \
        .malloc = reiAllocationCallbacksDefaultMalloc, \
        .realloc = reiAllocationCallbacksDefaultRealloc, \
        .free = reiAllocationCallbacksDefaultFree \
    };
}
