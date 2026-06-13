#include "rei_allocator.h"

C_TEMPLATE_IMPL_ALLOCATOR(, rei, Rei)

ReiAllocator reiAllocator_g;

void reiInitialize(const ReiAllocator* allocator)
{
    if (allocator != NULL)
        reiAllocator_g = *allocator;
    else
        reiAllocator_g = reiDefaultAllocator();
}
