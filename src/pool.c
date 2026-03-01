#include "pool.h"

luna_Ref lunaPoolHeader_Alloc(luna_PoolHeader *header)
{
    if (header->free_list == LUNA_NULL_REF)
    {
        // TODO: 池空间不足
        return LUNA_NULL_REF;
    }
    luna_Ref ref = header->free_list;
    header->free_list = header->next_free[ref];
    header->size++;
    return ref;
}
void lunaPoolHeader_Free(luna_PoolHeader *header, luna_Ref ref)
{
    if (ref == LUNA_NULL_REF)
        return;
    header->next_free[ref] = header->free_list;
    header->free_list = ref;
    header->size--;
}
bool lunaPoolHeader_IsValid(luna_PoolHeader *header, luna_Ref ref)
{
    if (header)
    {
    }
    return ref != LUNA_NULL_REF;
    // if (ref == LUNA_NULL_REF)
    //     return false;
    // luna_Ref current = header->free_list;
    // while (current != LUNA_NULL_REF)
    // {
    //     if (current == ref)
    //         return false;
    //     current = header->next_free[current];
    // }
    // return true;
}
