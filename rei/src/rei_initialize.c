#include "rei_allocator.h"
#include "rei_callbacks.h"

C_TEMPLATE_IMPL_ALLOCATOR(, rei, Rei)

ReiAllocator reiAllocator_g;
ReiCallbacks reiCallbacks_g;

void reiInitialize(const ReiAllocator* allocator, const ReiCallbacks* callbacks)
{
    if (allocator != NULL)
        reiAllocator_g = *allocator;
    else
        reiAllocator_g = reiDefaultAllocator();
    if (callbacks)
        reiCallbacks_g = *callbacks;
    else
    {
        reiCallbacks_g.context  = NULL;
        reiCallbacks_g.debug    = NULL;
    }
}
