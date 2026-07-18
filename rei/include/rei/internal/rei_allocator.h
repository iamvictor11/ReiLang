#ifndef REI_INTERNAL_ALLOCATOR_H
#define REI_INTERNAL_ALLOCATOR_H
#include "rei/rei.h"
#include <stdlib.h>

static inline void* reiAllocatorDefaultMalloc(void* ctx, size_t size)
{
    (void)ctx;
    return malloc(size);
}
static inline void* reiAllocatorDefaultRealloc(void* ctx, void* ptr, size_t size)
{
    (void)ctx;
    return realloc(ptr, size);
}
static inline void reiAllocatorDefaultFree(void* ctx, void* ptr) \
{
    (void)ctx;
    free(ptr);
}
static inline ReiAllocator reiDefaultAllocator(void)
{
    ReiAllocator allocator =
    {
        .context = NULL,
        .malloc = reiAllocatorDefaultMalloc,
        .realloc = reiAllocatorDefaultRealloc,
        .free = reiAllocatorDefaultFree
    };
    return allocator;
}

extern ReiAllocator reiAllocator_g;

#define reiMalloc(TYPE, COUNT)          ((TYPE*)reiAllocator_g.malloc(reiAllocator_g.context, sizeof(TYPE) * COUNT))
#define reiRealloc(TYPE, PTR, COUNT)    ((TYPE*)reiAllocator_g.realloc(reiAllocator_g.context, PTR, sizeof(TYPE) * COUNT))
#define reiFree(PTR)                    (reiAllocator_g.free(reiAllocator_g.context, PTR))

#endif
