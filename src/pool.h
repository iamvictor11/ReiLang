#ifndef LUNA_POOL_H
#define LUNA_POOL_H
#include "base.h"
#include <stdlib.h>
#include <string.h>

typedef struct luna_PoolHeader
{
    luna_UInt free_head;
    luna_UInt *next_free;
    luna_UInt size;
    luna_UInt capacity;
} luna_PoolHeader;

typedef struct
{
    luna_PoolHeader header;
    luna_String *data;
} luna_StringPool;

typedef struct
{
    luna_PoolHeader header;
    luna_Table *data;
} luna_TablePool;

typedef struct
{
    luna_PoolHeader header;
    luna_Func *data;
} luna_FuncPool;

luna_Ref lunaPoolHeader_Alloc(luna_PoolHeader *header);
void lunaPoolHeader_Free(luna_PoolHeader *header, luna_Ref ref);
bool lunaPoolHeader_IsValid(luna_PoolHeader *header, luna_Ref ref);

#define LUNA_POOL_INIT(_pool, _type, _cap)                                          \
    do                                                                              \
    {                                                                               \
        (_pool).data = (_type *)calloc((_cap), sizeof(_type));                      \
        (_pool).header.next_free = (luna_UInt *)malloc((_cap) * sizeof(luna_UInt)); \
        (_pool).header.capacity = (_cap);                                           \
        (_pool).header.size = 0;                                                    \
        for (luna_UInt i = 0; i < (_cap)-1; i++)                                    \
            (_pool).header.next_free[i] = i + 1;                                    \
        (_pool).header.next_free[(_cap)-1] = LUNA_NULL_REF;                         \
        (_pool).header.free_head = 0;                                               \
    } while (0)
#define LUNA_POOL_ALLOC(_pool) lunaPoolHeader_Alloc(&((_pool).header))
#define LUNA_POOL_IS_VALID(_pool) lunaPoolHeader_IsValid(&((_pool).header))
#define LUNA_POOL_AT(_pool, _ref) ((_pool).data + (_ref))
#define LUNA_POOL_FREE(_pool, _type, _free_func, _ref)  \
    do                                                  \
    {                                                   \
        _type *raw = (_pool).data + (_ref);             \
        _free_func(raw);                                \
        lunaPoolHeader_Free(&((_pool).header), (_ref)); \
    } while (0)
#define LUNA_POOL_RESET(_pool, _cap)                               \
    do                                                             \
    {                                                              \
        (_pool).header.size = 0;                                   \
        (_pool).header.free_head = 0;                              \
        for (luna_UInt i = 0; i < (_cap)-1; i++)                   \
            (_pool).header.next_free[i] = i + 1;                   \
        (_pool).header.next_free[(_cap)-1] = LUNA_NULL_REF;        \
        memset((_pool).data, 0, (_cap) * sizeof(*((_pool).data))); \
    } while (0)
#endif
