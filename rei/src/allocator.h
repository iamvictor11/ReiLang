#ifndef REI_ALLOCATOR_H
#define REI_ALLOCATOR_H

#include "template/allocator.h"

C_TEMPLATE_DECL_ALLOCATOR(static inline, rei, Rei)
C_TEMPLATE_DEFN_ALLOCATOR(static inline, rei, Rei)
C_TEMPLATE_IMPL_ALLOCATOR(static inline, rei, Rei)

ReiAllocator reiAllocator_g;

void reiInitGlobalAllocator(void);

#endif
