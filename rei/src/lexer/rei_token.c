#include "rei_token.h"
#include "../rei_allocator.h"

C_TEMPLATE_IMPL_VECTOR(, rei, Rei, TokenBuffer, ReiToken, &reiAllocator_g)
