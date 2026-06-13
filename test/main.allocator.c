#include "rei/rei.h"
#include REI_C_TEMPLATE_LIB_PLATFORM_CONSOLE_H
#include <stdio.h>

static void* REI_API_CALL reiCustomAllocatorDefaultMalloc(void* ctx, size_t size)
{
    (void)ctx;
    printf("call malloc(%zu)\n", size);
    return reiAllocatorDefaultMalloc(ctx, size);
}
static void* REI_API_CALL reiCustomAllocatorDefaultRealloc(void* ctx, void* ptr, size_t size)
{
    (void)ctx;
    printf("call realloc(%p, %zu)\n", ptr, size);
    return reiAllocatorDefaultRealloc(ctx, ptr, size);
}
static void REI_API_CALL reiCustomAllocatorDefaultFree(void* ctx, void* ptr)
{
    (void)ctx;
    printf("call free(%p)\n", ptr);
    reiAllocatorDefaultFree(ctx, ptr);
}

int main()
{
    osSetConsoleOutputCP_UTF8();
    ReiAllocator allocator = {NULL, reiCustomAllocatorDefaultMalloc, reiCustomAllocatorDefaultRealloc, reiCustomAllocatorDefaultFree};
    reiInitialize(&allocator, NULL);

    ReiVM vm = reiVMCreate();
    reiVMDestroy(vm);

    osGetChar();
    return EXIT_SUCCESS;
}
