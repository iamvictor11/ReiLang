#ifndef C_TEMPLATE_CONTAINER_VECTOR_H
#define C_TEMPLATE_CONTAINER_VECTOR_H

#include "../allocator.h"
#include <stdbool.h>
#include <string.h>

#pragma region Konst
#define C_TEMPLATE_VECTOR_GROWTH_FACTOR 2
#pragma endregion
#pragma region Dec
#define C_TEMPLATE_DECL_VECTOR(ATTR, SPREFIX, LPREFIX, NAME, TYPE) \
    typedef struct LPREFIX##NAME##_T LPREFIX##NAME##_T; \
    typedef LPREFIX##NAME##_T* LPREFIX##NAME; \
    /* 内部类 */ \
    typedef bool(*LPREFIX##NAME##Equalor)(const TYPE* curr, const TYPE* goal); \
    typedef bool(*LPREFIX##NAME##Predicate)(const TYPE* curr); \
    typedef void(*LPREFIX##NAME##Visitor)(TYPE* curr); \
    typedef int(*LPREFIX##NAME##Comparator)(const TYPE* left, const TYPE* right); \
    /* 定义类 */ \
    typedef struct LPREFIX##NAME##Def LPREFIX##NAME##Def; \
    static inline LPREFIX##NAME##Def SPREFIX##Null##NAME##Def(void); \
    /* 创建销毁 */ \
    ATTR LPREFIX##NAME SPREFIX##NAME##Create(const LPREFIX##NAME##Def* def); \
    ATTR LPREFIX##NAME SPREFIX##NAME##Copy(const LPREFIX##NAME src); \
    ATTR void SPREFIX##NAME##CopyTo(LPREFIX##NAME me, const LPREFIX##NAME src); \
    ATTR LPREFIX##NAME SPREFIX##NAME##Move(const LPREFIX##NAME src); \
    ATTR void SPREFIX##NAME##MoveTo(LPREFIX##NAME me, const LPREFIX##NAME src); \
    ATTR void SPREFIX##NAME##Destroy(LPREFIX##NAME me); \
    /* 属性 */ \
    ATTR TYPE* SPREFIX##NAME##Data(LPREFIX##NAME me); \
    ATTR size_t SPREFIX##NAME##Size(LPREFIX##NAME me); \
    ATTR size_t SPREFIX##NAME##Capacity(LPREFIX##NAME me); \
    /* 状态 */ \
    ATTR bool SPREFIX##NAME##Empty(LPREFIX##NAME me); \
    /* 访问 */ \
    ATTR TYPE* SPREFIX##NAME##At(LPREFIX##NAME me, size_t index); \
    ATTR TYPE* SPREFIX##NAME##Front(LPREFIX##NAME me); \
    ATTR TYPE* SPREFIX##NAME##Back(LPREFIX##NAME me); \
    ATTR TYPE* SPREFIX##NAME##Find(LPREFIX##NAME me, const TYPE* goal, LPREFIX##NAME##Equalor equalor, size_t* out_index); \
    ATTR bool SPREFIX##NAME##Contains(LPREFIX##NAME me, const TYPE* goal, LPREFIX##NAME##Equalor equalor); \
    /* 批处理 */ \
    ATTR void SPREFIX##NAME##Assign(LPREFIX##NAME me, size_t count, const TYPE* elements); \
    ATTR void SPREFIX##NAME##Append(LPREFIX##NAME me, size_t count, const TYPE* elements); \
    ATTR void SPREFIX##NAME##InsertRange(LPREFIX##NAME me, size_t index, size_t count, const TYPE* elements); \
    ATTR void SPREFIX##NAME##EraseRange(LPREFIX##NAME me, size_t index, size_t count); \
    ATTR void SPREFIX##NAME##RemoveIf(LPREFIX##NAME me, LPREFIX##NAME##Predicate predicate); \
    ATTR void SPREFIX##NAME##Foreach(LPREFIX##NAME me, LPREFIX##NAME##Visitor visitor); \
    ATTR void SPREFIX##NAME##RForeach(LPREFIX##NAME me, LPREFIX##NAME##Visitor visitor); \
    /* 排序 */ \
    ATTR void SPREFIX##NAME##BubbleSort(LPREFIX##NAME me, LPREFIX##NAME##Comparator comparator); \
    /* 容量 */ \
    ATTR void SPREFIX##NAME##Reserve(LPREFIX##NAME me, size_t capacity); \
    ATTR void SPREFIX##NAME##Resize(LPREFIX##NAME me, size_t size); \
    ATTR void SPREFIX##NAME##ShriToFit(LPREFIX##NAME me); \
    ATTR void SPREFIX##NAME##Clear(LPREFIX##NAME me); \
    /* 增删改 */ \
    ATTR void SPREFIX##NAME##Insert(LPREFIX##NAME me, size_t index, const TYPE* element); \
    ATTR void SPREFIX##NAME##InsertLiteral(LPREFIX##NAME me, size_t index, TYPE literal); \
    ATTR void SPREFIX##NAME##Erase(LPREFIX##NAME me, size_t index); \
    ATTR size_t SPREFIX##NAME##Push(LPREFIX##NAME me, const TYPE* element); \
    ATTR size_t SPREFIX##NAME##PushLiteral(LPREFIX##NAME me, TYPE literal); \
    ATTR void SPREFIX##NAME##Pop(LPREFIX##NAME me); \
    ATTR void SPREFIX##NAME##Set(LPREFIX##NAME me, size_t index, const TYPE* element); \
    ATTR void SPREFIX##NAME##SetLiteral(LPREFIX##NAME me, size_t index, TYPE literal); \
    /* 交换 */ \
    ATTR void SPREFIX##NAME##Swap(LPREFIX##NAME me, LPREFIX##NAME other);
#pragma endregion
#pragma region Def
#define C_TEMPLATE_DEFN_VECTOR(ATTR, SPREFIX, LPREFIX, NAME, TYPE) \
    typedef struct LPREFIX##NAME##_T \
    { \
        TYPE* data; \
        size_t size; \
        size_t capacity; \
    } LPREFIX##NAME##_T; \
    typedef struct LPREFIX##NAME##Def \
    { \
        size_t size; \
        size_t capacity; \
        TYPE initial; \
    } LPREFIX##NAME##Def; \
    static inline LPREFIX##NAME##Def SPREFIX##Null##NAME##Def(void) \
    { \
        LPREFIX##NAME##Def def; \
        def.size = 0; \
        def.capacity = 0; \
        return def; \
    }
#pragma endregion
#pragma region Impl
#define C_TEMPLATE_IMPL_VECTOR(ATTR, SPREFIX, LPREFIX, NAME, TYPE, ALLOCATOR) \
    /* 创建销毁 */ \
    ATTR LPREFIX##NAME SPREFIX##NAME##Create(const LPREFIX##NAME##Def* def) \
    { \
        LPREFIX##NAME me = (LPREFIX##NAME)(ALLOCATOR)->malloc((ALLOCATOR)->context, sizeof(LPREFIX##NAME##_T)); \
        if (me == NULL) return NULL; \
        me->data = NULL; \
        me->size = 0; \
        me->capacity = 0; \
        if (def && def->capacity > 0) SPREFIX##NAME##Reserve(me, def->capacity); \
        if (def && def->size > 0) \
        { \
            if (me->capacity < def->size) SPREFIX##NAME##Reserve(me, def->size); \
            for (size_t i = 0; i < def->size; i++) \
                me->data[i] = def->initial; \
            me->size = def->size; \
        } \
        return me; \
    } \
    ATTR LPREFIX##NAME SPREFIX##NAME##Copy(const LPREFIX##NAME src) \
    { \
        if (src == NULL) return NULL; \
        LPREFIX##NAME me = (LPREFIX##NAME)(ALLOCATOR)->malloc((ALLOCATOR)->context, sizeof(LPREFIX##NAME##_T)); \
        if (me == NULL) return NULL; \
        me->data = NULL; \
        me->size = 0; \
        me->capacity = 0; \
        if (src->capacity > 0) SPREFIX##NAME##Reserve(me, src->capacity); \
        if (src->size > 0) \
        { \
            me->size = src->size; \
            for (size_t i = 0; i < src->size; i++) \
                me->data[i] = src->data[i]; \
        } \
        return me; \
    } \
    ATTR void SPREFIX##NAME##CopyTo(LPREFIX##NAME me, const LPREFIX##NAME src) \
    { \
        if (src == NULL || me == src) return; \
        SPREFIX##NAME##Clear(me); \
        if (src->capacity > 0) SPREFIX##NAME##Reserve(me, src->capacity); \
        if (src->size > 0) \
            for (size_t i = 0; i < src->size; i++) \
                me->data[i] = src->data[i]; \
        me->size = src->size; \
    } \
    ATTR LPREFIX##NAME SPREFIX##NAME##Move(const LPREFIX##NAME src) \
    { \
        if (src == NULL) return NULL; \
        LPREFIX##NAME me = (LPREFIX##NAME)(ALLOCATOR)->malloc((ALLOCATOR)->context, sizeof(LPREFIX##NAME##_T)); \
        if (me == NULL) return NULL; \
        me->data = src->data; \
        me->size = src->size; \
        me->capacity = src->capacity; \
        src->data = NULL; \
        src->size = 0; \
        src->capacity = 0; \
        return me; \
    } \
    ATTR void SPREFIX##NAME##MoveTo(LPREFIX##NAME me, const LPREFIX##NAME src) \
    { \
        if (src == NULL || me == src) return; \
        SPREFIX##NAME##Destroy(me); \
        me->data = src->data; \
        me->size = src->size; \
        me->capacity = src->capacity; \
        src->data = NULL; \
        src->size = 0; \
        src->capacity = 0; \
    } \
    ATTR void SPREFIX##NAME##Destroy(LPREFIX##NAME me) \
    { \
        if (me->data != NULL) \
        { \
            (ALLOCATOR)->free((ALLOCATOR)->context, me->data); \
            me->data = NULL; \
        } \
        me->size = 0; \
        me->capacity = 0; \
    } \
    /* 属性 */ \
    ATTR TYPE* SPREFIX##NAME##Data(LPREFIX##NAME me) \
    { \
        return me->data; \
    } \
    ATTR size_t SPREFIX##NAME##Size(LPREFIX##NAME me) \
    { \
        return me->size; \
    } \
    ATTR size_t SPREFIX##NAME##Capacity(LPREFIX##NAME me) \
    { \
        return me->capacity; \
    } \
    /* 状态 */ \
    ATTR bool SPREFIX##NAME##Empty(LPREFIX##NAME me) \
    { \
        return me->size == 0; \
    } \
    /* 访问 */ \
    ATTR TYPE* SPREFIX##NAME##At(LPREFIX##NAME me, size_t index) \
    { \
        return &me->data[index]; \
    } \
    ATTR TYPE* SPREFIX##NAME##Front(LPREFIX##NAME me) \
    { \
        return &me->data[0]; \
    } \
    ATTR TYPE* SPREFIX##NAME##Back(LPREFIX##NAME me) \
    { \
        return &me->data[me->size - 1]; \
    } \
    ATTR TYPE* SPREFIX##NAME##Find(LPREFIX##NAME me, const TYPE* goal, LPREFIX##NAME##Equalor equalor, size_t* out_index) \
    { \
        for (size_t i = 0; i < me->size; i++) \
            if (equalor(&me->data[i], goal)) \
            { \
                if (out_index) *out_index = i; \
                return &me->data[i]; \
            } \
        return NULL; \
    } \
    ATTR bool SPREFIX##NAME##Contains(LPREFIX##NAME me, const TYPE* goal, LPREFIX##NAME##Equalor equalor) \
    { \
        return SPREFIX##NAME##Find(me, goal, equalor, NULL) != NULL; \
    } \
    /* 批处理 */ \
    ATTR void SPREFIX##NAME##Assign(LPREFIX##NAME me, size_t count, const TYPE* elements) \
    { \
        SPREFIX##NAME##Clear(me); \
        if (count == 0) return; \
        SPREFIX##NAME##Reserve(me, count); \
        for (size_t i = 0; i < count; i++) \
            me->data[i] = elements[i]; \
        me->size = count; \
    } \
    ATTR void SPREFIX##NAME##Append(LPREFIX##NAME me, size_t count, const TYPE* elements) \
    { \
        if (count == 0) return; \
        size_t required = me->size + count; \
        if (required > me->capacity) \
        { \
            size_t newCapacity = me->capacity ? me->capacity : 1; \
            while (newCapacity < required) newCapacity *= C_TEMPLATE_VECTOR_GROWTH_FACTOR; \
            SPREFIX##NAME##Reserve(me, newCapacity); \
        } \
        for (size_t i = 0; i < count; i++) \
            me->data[me->size + i] = elements[i]; \
        me->size = required; \
    } \
    ATTR void SPREFIX##NAME##InsertRange(LPREFIX##NAME me, size_t index, size_t count, const TYPE* elements) \
    { \
        if (count == 0) return; \
        if (index > me->size) index = me->size; \
        size_t required = me->size + count; \
        if (required > me->capacity) \
        { \
            size_t newCapacity = me->capacity ? me->capacity : 1; \
            while (newCapacity < required) newCapacity *= C_TEMPLATE_VECTOR_GROWTH_FACTOR; \
            SPREFIX##NAME##Reserve(me, newCapacity); \
        } \
        if (index < me->size) \
            memmove(&me->data[index + count], &me->data[index], (me->size - index) * sizeof(TYPE)); \
        for (size_t i = 0; i < count; i++) \
            me->data[index + i] = elements[i]; \
        me->size = required; \
    } \
    ATTR void SPREFIX##NAME##EraseRange(LPREFIX##NAME me, size_t index, size_t count) \
    { \
        if (count == 0 || index >= me->size) return; \
        if (count > me->size - index) count = me->size - index; \
        size_t tail = me->size - (index + count); \
        if (tail > 0) \
            memmove(&me->data[index], &me->data[index + count], tail * sizeof(TYPE)); \
        me->size -= count; \
    } \
    ATTR void SPREFIX##NAME##RemoveIf(LPREFIX##NAME me, LPREFIX##NAME##Predicate predicate) \
    { \
        size_t write = 0; \
        for (size_t read = 0; read < me->size; read++) \
        { \
            if (predicate(&me->data[read])) continue; \
            if (write != read) me->data[write] = me->data[read]; \
            write++; \
        } \
        me->size = write; \
    } \
    ATTR void SPREFIX##NAME##Foreach(LPREFIX##NAME me, LPREFIX##NAME##Visitor visitor) \
    { \
        for (size_t i = 0; i < me->size; i++) \
            visitor(&me->data[i]); \
    } \
    ATTR void SPREFIX##NAME##RForeach(LPREFIX##NAME me, LPREFIX##NAME##Visitor visitor) \
    { \
        for (size_t i = me->size; i > 0; i--) \
            visitor(&me->data[i - 1]); \
    } \
    /* 排序 */ \
    ATTR void SPREFIX##NAME##BubbleSort(LPREFIX##NAME me, LPREFIX##NAME##Comparator comparator) \
    { \
        if (me->size < 2) return; \
        for (size_t i = 0; i < me->size - 1; i++) \
            for (size_t j = 0; j < me->size - 1 - i; j++) \
                if (comparator(&me->data[j], &me->data[j + 1]) > 0) \
                { \
                    TYPE temp = me->data[j]; \
                    me->data[j] = me->data[j + 1]; \
                    me->data[j + 1] = temp; \
                } \
    } \
    /* 容量 */ \
    ATTR void SPREFIX##NAME##Reserve(LPREFIX##NAME me, size_t capacity) \
    { \
        if (capacity <= me->capacity) return; \
        if (me->data == NULL) \
            me->data = (TYPE*)(ALLOCATOR)->malloc((ALLOCATOR)->context, capacity * sizeof(TYPE)); \
        else \
            me->data = (TYPE*)(ALLOCATOR)->realloc((ALLOCATOR)->context, me->data, capacity * sizeof(TYPE)); \
        if (me->data != NULL) me->capacity = capacity; \
    } \
    ATTR void SPREFIX##NAME##Resize(LPREFIX##NAME me, size_t size) \
    { \
        if (size > me->capacity) \
        { \
            size_t newCapacity = me->capacity ? me->capacity : 1; \
            while (newCapacity < size) newCapacity *= C_TEMPLATE_VECTOR_GROWTH_FACTOR; \
            SPREFIX##NAME##Reserve(me, newCapacity); \
        } \
        if (size > me->size) \
        { \
            for (size_t i = me->size; i < size; i++) \
                me->data[i] = (TYPE){}; \
        } \
        me->size = size; \
    } \
    ATTR void SPREFIX##NAME##ShriToFit(LPREFIX##NAME me) \
    { \
        if (me->size == me->capacity) return; \
        if (me->size == 0) \
        { \
            (ALLOCATOR)->free((ALLOCATOR)->context, me->data); \
            me->data = NULL; \
            me->capacity = 0; \
            return; \
        } \
        TYPE* newData = (TYPE*)(ALLOCATOR)->realloc((ALLOCATOR)->context, me->data, me->size * sizeof(TYPE)); \
        if (newData != NULL) \
        { \
            me->data = newData; \
            me->capacity = me->size; \
        } \
    } \
    ATTR void SPREFIX##NAME##Clear(LPREFIX##NAME me) \
    { \
        me->size = 0; \
    } \
    /* 增删改 */ \
    ATTR void SPREFIX##NAME##Insert(LPREFIX##NAME me, size_t index, const TYPE* element) \
    { \
        SPREFIX##NAME##InsertRange(me, index, 1, element); \
    } \
    ATTR void SPREFIX##NAME##InsertLiteral(LPREFIX##NAME me, size_t index, TYPE literal) { SPREFIX##NAME##Insert(me, index, &literal); } \
    ATTR void SPREFIX##NAME##Erase(LPREFIX##NAME me, size_t index) \
    { \
        SPREFIX##NAME##EraseRange(me, index, 1); \
    } \
    ATTR size_t SPREFIX##NAME##Push(LPREFIX##NAME me, const TYPE* element) \
    { \
        if (me->size + 1 > me->capacity) \
        { \
            size_t newCapacity = me->capacity ? me->capacity * C_TEMPLATE_VECTOR_GROWTH_FACTOR : 1; \
            SPREFIX##NAME##Reserve(me, newCapacity); \
        } \
        me->data[me->size] = *element; \
        me->size += 1; \
        return me->size - 1; \
    } \
    ATTR size_t SPREFIX##NAME##PushLiteral(LPREFIX##NAME me, TYPE literal) { return SPREFIX##NAME##Push(me, &literal); } \
    ATTR void SPREFIX##NAME##Pop(LPREFIX##NAME me) \
    { \
        if (me->size == 0) return; \
        me->size -= 1; \
    } \
    ATTR void SPREFIX##NAME##Set(LPREFIX##NAME me, size_t index, const TYPE* element) \
    { \
        me->data[index] = *element; \
    } \
    ATTR void SPREFIX##NAME##SetLiteral(LPREFIX##NAME me, size_t index, TYPE literal) { SPREFIX##NAME##Set(me, index, &literal); } \
    /* 交换 */ \
    ATTR void SPREFIX##NAME##Swap(LPREFIX##NAME me, LPREFIX##NAME other) \
    { \
        TYPE* tempData = me->data; \
        size_t tempSize = me->size; \
        size_t tempCapacity = me->capacity; \
        me->data = other->data; \
        me->size = other->size; \
        me->capacity = other->capacity; \
        other->data = tempData; \
        other->size = tempSize; \
        other->capacity = tempCapacity; \
    }
#pragma endregion
#pragma region Sugar
#define C_TEMPLATE_VECTOR_FOREACH(TYPE, ELEM_PTR_NAME, CONTAINER) \
    for ( \
        TYPE* ELEM_PTR_NAME = (CONTAINER)->data; \
        ELEM_PTR_NAME != NULL && (size_t)(ELEM_PTR_NAME - (CONTAINER)->data) < (CONTAINER)->size; \
        ELEM_PTR_NAME++ \
    )
#define C_TEMPLATE_VECTOR_RFOREACH(TYPE, ELEM_PTR_NAME, CONTAINER) \
    for ( \
        TYPE* ELEM_PTR_NAME = ((CONTAINER)->size > 0 ? &((CONTAINER)->data[(CONTAINER)->size - 1]) : NULL); \
        ELEM_PTR_NAME != NULL; \
        ELEM_PTR_NAME = (ELEM_PTR_NAME == (CONTAINER)->data ? NULL : ELEM_PTR_NAME - 1) \
    )
#pragma endregion

#endif
