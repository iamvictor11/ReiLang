#include "rei_value.h"
#include "../rei_allocator.h"

C_TEMPLATE_IMPL_VECTOR(, rei, Rei, ValueBuffer, ReiValue, &reiAllocator_g)

C_TEMPLATE_IMPL_STRING(, rei, Rei, String, &reiAllocator_g)
