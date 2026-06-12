#include "rei_allocator.h"

void reiInitGlobalAllocator(void)
{
    reiAllocator_g = reiDefaultAllocator();
}
