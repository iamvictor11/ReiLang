#ifndef C_TEMPLATE_UTILS_RANDOM_H
#define C_TEMPLATE_UTILS_RANDOM_H

#include "../allocator.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#pragma region Dec
#define C_TEMPLATE_DECL_RANDOM_GEN(ATTR, SPREFIX, LPREFIX, NAME, NUM_TYPE) \
    typedef struct LPREFIX##NAME##_T LPREFIX##NAME##_T; \
    typedef LPREFIX##NAME##_T* LPREFIX##NAME; \
    /* 内部类 */ \
    typedef NUM_TYPE(*LPREFIX##NAME##Generator)(NUM_TYPE state); \
    typedef void(*LPREFIX##NAME##Clampor)(NUM_TYPE* value, NUM_TYPE min, NUM_TYPE max); \
    /* 定义类 */ \
    typedef struct LPREFIX##NAME##Def LPREFIX##NAME##Def; \
    static inline LPREFIX##NAME##Def SPREFIX##Null##NAME##Def(void); \
    /* 创建销毁 */ \
    ATTR bool SPREFIX##NAME##Init(LPREFIX##NAME me, const LPREFIX##NAME##Def* def); \
    ATTR LPREFIX##NAME SPREFIX##NAME##Create(const LPREFIX##NAME##Def* def); \
    ATTR void SPREFIX##NAME##Free(LPREFIX##NAME me); \
    ATTR void SPREFIX##NAME##Destroy(LPREFIX##NAME me); \
    /* 属性 */ \
    ATTR NUM_TYPE SPREFIX##NAME##Seed(LPREFIX##NAME me); \
    ATTR NUM_TYPE SPREFIX##NAME##State(LPREFIX##NAME me); \
    /* 生成 */ \
    ATTR NUM_TYPE SPREFIX##NAME##Next(LPREFIX##NAME me); \
    ATTR NUM_TYPE SPREFIX##NAME##NextRange(LPREFIX##NAME me, NUM_TYPE min, NUM_TYPE max); \
    ATTR void SPREFIX##NAME##NextBulk(LPREFIX##NAME me, size_t count, NUM_TYPE* out); \
    ATTR void SPREFIX##NAME##NextRangetBulk(LPREFIX##NAME me, NUM_TYPE min, NUM_TYPE max, size_t count, NUM_TYPE* out); \
    /* 状态 */ \
    ATTR void SPREFIX##NAME##Reset(LPREFIX##NAME me);
#pragma endregion
#pragma region Def
#define C_TEMPLATE_DEFN_RANDOM_GEN(ATTR, SPREFIX, LPREFIX, NAME, NUM_TYPE) \
    typedef struct LPREFIX##NAME##_T \
    { \
        NUM_TYPE seed; \
        NUM_TYPE state; \
        LPREFIX##NAME##Generator generator; \
        LPREFIX##NAME##Clampor clampor; \
    } LPREFIX##NAME##_T; \
    /* 定义类 */ \
    typedef struct LPREFIX##NAME##Def \
    { \
        NUM_TYPE seed; \
        LPREFIX##NAME##Generator generator; \
        LPREFIX##NAME##Clampor clampor; \
    } LPREFIX##NAME##Def; \
    static inline LPREFIX##NAME##Def SPREFIX##Null##NAME##Def(void) \
    { \
        LPREFIX##NAME##Def def = {0}; \
        return def; \
    }
#pragma endregion
#pragma region Impl
#define C_TEMPLATE_IMPL_RANDOM_GEN(ATTR, SPREFIX, LPREFIX, NAME, NUM_TYPE, ALLOCATOR) \
    /* 私有 */ \
    ATTR NUM_TYPE SPREFIX##NAME##_Next(LPREFIX##NAME me) \
    { \
        me->state = me->generator(me->state); \
        return me->state; \
    } \
    /* 创建销毁 */ \
    ATTR bool SPREFIX##NAME##Init(LPREFIX##NAME me, const LPREFIX##NAME##Def* def) \
    { \
        if (me == NULL) return false; \
        if (def) \
        { \
            me->seed = def->seed; \
            me->state = def->seed; \
            me->generator = def->generator; \
            me->clampor = def->clampor; \
        } \
        else \
        { \
            SPREFIX##NAME##Reset(me); \
            me->generator = NULL; \
            me->clampor = NULL; \
        } \
        return true; \
    } \
    ATTR LPREFIX##NAME SPREFIX##NAME##Create(const LPREFIX##NAME##Def* def) \
    { \
        LPREFIX##NAME me = (LPREFIX##NAME)(ALLOCATOR)->malloc((ALLOCATOR)->context, sizeof(LPREFIX##NAME##_T)); \
        if (!SPREFIX##NAME##Init(me, def)) return NULL; \
        return me; \
    } \
    ATTR void SPREFIX##NAME##Free(LPREFIX##NAME me) \
    { \
    } \
    ATTR void SPREFIX##NAME##Destroy(LPREFIX##NAME me) \
    { \
		if (me == NULL) return; \
        (ALLOCATOR)->free((ALLOCATOR)->context, me); \
    } \
    /* 属性 */ \
    ATTR NUM_TYPE SPREFIX##NAME##Seed(LPREFIX##NAME me) \
    { \
        return me->seed; \
    } \
    ATTR NUM_TYPE SPREFIX##NAME##State(LPREFIX##NAME me) \
    { \
        return me->state; \
    } \
    /* 生成 */ \
    ATTR NUM_TYPE SPREFIX##NAME##Next(LPREFIX##NAME me) \
    { \
        if (me->generator) \
            return SPREFIX##NAME##_Next(me); \
        return me->seed; \
    } \
    ATTR NUM_TYPE SPREFIX##NAME##NextRange(LPREFIX##NAME me, NUM_TYPE min, NUM_TYPE max) \
    { \
        NUM_TYPE random; \
        if (me->generator) \
            random = SPREFIX##NAME##_Next(me); \
        else \
            random = me->seed; \
        if (me->clampor) \
            me->clampor(&random, min, max); \
        return random; \
    } \
    ATTR void SPREFIX##NAME##NextBulk(LPREFIX##NAME me, size_t count, NUM_TYPE* out) \
    { \
        if (me->generator) \
            for (size_t i = 0; i < count; i++) \
                out[i] = SPREFIX##NAME##_Next(me); \
    } \
    ATTR void SPREFIX##NAME##NextRangetBulk(LPREFIX##NAME me, NUM_TYPE min, NUM_TYPE max, size_t count, NUM_TYPE* out) \
    { \
        SPREFIX##NAME##NextBulk(me, count, out); \
        if (me->generator) \
            for (size_t i = 0; i < count; i++) \
                out[i] = SPREFIX##NAME##_Next(me); \
        if (me->clampor) \
            for (size_t i = 0; i < count; i++) \
                me->clampor(&out[i], min, max); \
    } \
    /* 状态 */ \
    ATTR void SPREFIX##NAME##Reset(LPREFIX##NAME me) \
    { \
        memset(&me->seed, 0, sizeof(me->seed)); \
        memset(&me->state, 0, sizeof(me->state)); \
    }
#pragma endregion

#endif
