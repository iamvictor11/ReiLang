#ifndef REI_ALLOCATOR_H
#define REI_ALLOCATOR_H

#include "rei_internal.h"

extern ReiAllocator reiAllocator_g;

#define reiMalloc(TYPE, COUNT) (TYPE*)reiAllocator_g.malloc(reiAllocator_g.context, sizeof(TYPE) * COUNT)
#define reiRealloc(TYPE, PTR, COUNT) (TYPE*)reiAllocator_g.realloc(reiAllocator_g.context, PTR, sizeof(TYPE) * COUNT)
#define reiFree(PTR) reiAllocator_g.free(reiAllocator_g.context, PTR)

#endif
