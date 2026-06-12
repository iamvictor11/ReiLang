#include "rei_value.h"

C_TEMPLATE_IMPL_VECTOR(, rei, Rei, ValueBuffer, ReiValue, &reiAllocator_g)

C_TEMPLATE_IMPL_STRING(, rei, Rei, String, &reiAllocator_g)
