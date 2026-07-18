#include "rei/internal/lexer/rei_token.h"
#include "rei/internal/rei_allocator.h"

REI_IMPL_VECTOR(, rei, Rei, TokenBuffer, ReiToken, &reiAllocator_g)
