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
extern ReiAllocationCallbacks reiAllocationCallbacks_g;
#define reiMalloc(TYPE, COUNT)          ((TYPE*)reiAllocationCallbacks_g.malloc(reiAllocationCallbacks_g.context, sizeof(TYPE) * COUNT))
#define reiRealloc(TYPE, PTR, COUNT)    ((TYPE*)reiAllocationCallbacks_g.realloc(reiAllocationCallbacks_g.context, PTR, sizeof(TYPE) * COUNT))
#define reiFree(PTR)                    (reiAllocationCallbacks_g.free(reiAllocationCallbacks_g.context, PTR))
}
