#include "rei/internal/rei_allocation_callbacks.hxx"

extern "C"
{
void* reiAllocationCallbacksDefaultMalloc(void* ctx, size_t size)
{
    (void)ctx;
    return malloc(size);
}
void* reiAllocationCallbacksDefaultRealloc(void* ctx, void* ptr, size_t size)
{
    (void)ctx;
    return realloc(ptr, size);
}
void reiAllocationCallbacksDefaultFree(void* ctx, void* ptr)
{
    (void)ctx;
    free(ptr);
}
ReiAllocationCallbacks reiAllocationCallbacks_g =
    {
        .context = NULL,
        .malloc = reiAllocationCallbacksDefaultMalloc,
        .realloc = reiAllocationCallbacksDefaultRealloc,
        .free = reiAllocationCallbacksDefaultFree
    };
}
