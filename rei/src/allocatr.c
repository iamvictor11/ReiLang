#include "allocator.h"

void reiInitGlobalAllocator(void)
{
    reiAllocator_g = reiDefaultAllocator();
}
