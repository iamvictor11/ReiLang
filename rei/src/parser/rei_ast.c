#include "rei_ast.h"
#include "../rei_allocator.h"

C_TEMPLATE_IMPL_VECTOR(, rei, Rei, AstForest, ReiAstNodePtr, &reiAllocator_g)
