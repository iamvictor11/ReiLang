#include "rei/rei.h"
#include <stdio.h>
#include <stdlib.h>

static void* REI_API_CALL reiCustomAllocatorDefaultMalloc(void* ctx, size_t size)
{
    (void)ctx;
    printf("call malloc(%zu)\n", size);
    return malloc(size);
}
static void* REI_API_CALL reiCustomAllocatorDefaultRealloc(void* ctx, void* ptr, size_t size)
{
    (void)ctx;
    printf("call realloc(%p, %zu)\n", ptr, size);
    return realloc(ptr, size);
}
static void REI_API_CALL reiCustomAllocatorDefaultFree(void* ctx, void* ptr)
{
    (void)ctx;
    printf("call free(%p)\n", ptr);
    free(ptr);
}

int main()
{
    printf("\x1b%%G");

    ReiAllocator allocator = {NULL, reiCustomAllocatorDefaultMalloc, reiCustomAllocatorDefaultRealloc, reiCustomAllocatorDefaultFree};
    reiInitialize(&allocator, NULL);

    ReiVM vm = reiVMCreate();
    reiVMDestroy(vm);

    scanf("%c");
    return 0;
}
