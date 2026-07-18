#include "rei/internal/type/rei_value.h"
#include "rei/internal/utils/rei_vector.h"
#include "rei/internal/rei_allocator.h"

REI_IMPL_VECTOR(, rei, Rei, ByteBuffer, ReiBytecode, &reiAllocator_g)
REI_IMPL_VECTOR(, rei, Rei, UIntBuffer, uint32_t, &reiAllocator_g)
REI_IMPL_VECTOR(, rei, Rei, ValueBuffer, ReiValue, &reiAllocator_g)
REI_IMPL_STRING(, rei, Rei, String, &reiAllocator_g)
