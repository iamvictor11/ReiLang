#include "pool.h"
#include <stdio.h>
luna_Ref lunaPoolHeader_Alloc(luna_PoolHeader *header)
{
    if (!header)
    {
        printf("Header is NULL");
        return LUNA_NULL_REF;
    }
    if (header->free_head == LUNA_NULL_REF)
    {
        // TODO: 池空间不足
        return LUNA_NULL_REF;
    }
    luna_Ref ref = header->free_head;
    header->free_head = header->next_free[ref];
    header->size++;
    return ref;
}
void lunaPoolHeader_Free(luna_PoolHeader *header, luna_Ref ref)
{
    if (ref == LUNA_NULL_REF)
        return;
    header->next_free[ref] = header->free_head;
    header->free_head = ref;
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
    // luna_Ref current = header->free_head;
    // while (current != LUNA_NULL_REF)
    // {
    //     if (current == ref)
    //         return false;
    //     current = header->next_free[current];
    // }
    // return true;
}
