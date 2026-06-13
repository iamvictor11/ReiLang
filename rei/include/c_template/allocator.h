#ifndef C_TEMPLATE_CONTAINER_ALLOCATOR_H
#define C_TEMPLATE_CONTAINER_ALLOCATOR_H

#include <stddef.h>
#include <stdlib.h>

#define C_TEMPLATE_DECL_ALLOCATOR(ATTR, SPREFIX, LPREFIX) \
    typedef struct LPREFIX##Allocator LPREFIX##Allocator; \
    ATTR void* SPREFIX##AllocatorDefaultMalloc(void* ctx, size_t size); \
    ATTR void* SPREFIX##AllocatorDefaultRealloc(void* ctx, void* ptr, size_t size); \
    ATTR void SPREFIX##AllocatorDefaultFree(void* ctx, void* ptr); \
    ATTR LPREFIX##Allocator SPREFIX##DefaultAllocator(void);
#define C_TEMPLATE_DEFN_ALLOCATOR(ATTR, SPREFIX, LPREFIX, CALL) \
    typedef struct LPREFIX##Allocator \
    { \
        void* context; \
        void* (CALL *malloc)(void* ctx, size_t size); \
        void* (CALL *realloc)(void* ctx, void* ptr, size_t size); \
        void  (CALL *free)(void* ctx, void* ptr); \
    } LPREFIX##Allocator;
#define C_TEMPLATE_IMPL_ALLOCATOR(ATTR, SPREFIX, LPREFIX) \
    ATTR void* SPREFIX##AllocatorDefaultMalloc(void* ctx, size_t size) \
    { \
        (void)ctx; \
        return malloc(size); \
    } \
    ATTR void* SPREFIX##AllocatorDefaultRealloc(void* ctx, void* ptr, size_t size) \
    { \
        (void)ctx; \
        return realloc(ptr, size); \
    } \
    ATTR void SPREFIX##AllocatorDefaultFree(void* ctx, void* ptr) \
    { \
        (void)ctx; \
        free(ptr); \
    } \
    ATTR LPREFIX##Allocator SPREFIX##DefaultAllocator(void) \
    { \
        LPREFIX##Allocator allocator = \
        { \
            .context = NULL, \
            .malloc = SPREFIX##AllocatorDefaultMalloc, \
            .realloc = SPREFIX##AllocatorDefaultRealloc, \
            .free = SPREFIX##AllocatorDefaultFree \
        }; \
        return allocator; \
    }

#endif
