#include "rei_allocator.h"
#include "rei_callbacks.h"

C_TEMPLATE_IMPL_ALLOCATOR(, rei, Rei)

ReiAllocator reiAllocator_g = {0};
ReiCallbacks reiCallbacks_g = {0};

void reiInitialize(const ReiAllocator* allocator, const ReiCallbacks* callbacks)
{
    if (allocator)
        reiAllocator_g = *allocator;
    else
        reiAllocator_g = reiDefaultAllocator();
    if (callbacks)
        reiCallbacks_g = *callbacks;
}
