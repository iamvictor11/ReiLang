#include <inttypes.h>
#include "value.h"
#include "../allocator.h"

C_TEMPLATE_IMPL_VECTOR(, rei, Rei, ValueVector, ReiValue, &reiAllocator_g)

char* reiValueToCstr(ReiValue value)
{
    static char buffer[64];
    if (REI_IS_NIL(value))
        return "nil";
    else if (REI_IS_BOOL(value))
        return REI_AS_BOOL(value) ? "true" : "false";
    else if (REI_IS_INT(value))
    {
        sprintf(buffer, sizeof(buffer), "%" PRId64, REI_AS_INT(value));
        return buffer;
    }
    else if (REI_IS_FLOAT(value))
    {
        sprintf(buffer, sizeof(buffer), "%g", REI_AS_FLOAT(value));
        return buffer;
    }
    else if (REI_IS_OBJ(value))
    {
        return "obj";
    }
    return "undefined";
}
