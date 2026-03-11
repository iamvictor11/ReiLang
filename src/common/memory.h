#ifndef LUNA_COMMON_MEMORY_H
#define LUNA_COMMON_MEMORY_H
#include <stdint.h>
#include <stdlib.h>

#define LUNA_GROW_CAPACITY(capacity) \
    ((capacity) < 8 ? 8 : (capacity) * 2)
#define LUNA_GROW_ARRAY(type, pointer, newSize) \
    (type *)realloc(pointer, sizeof(type) * (newSize))
#define LUNA_FREE_ARRAY(type, pointer) \
    realloc(pointer, 0)

#endif