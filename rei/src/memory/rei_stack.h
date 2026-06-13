#ifndef REI_MEMORY_STACK_H
#define REI_MEMORY_STACK_H

#include "../type/rei_value.h"

typedef struct ReiStack_T
{
    ReiValue* bp;
    ReiValue* tp;
    ReiValue data[REI_MAX_STACK_SIZE]
} ReiStack_T;
typedef ReiStack_T* ReiStack;
typedef ReiValue* ReiRegId;

static inline void reiStackInit(ReiStack me);
static inline ReiValue* reiStackBp(ReiStack me);
static inline ReiValue* reiStackTp(ReiStack me);
static inline ReiValue* reiStackData(ReiStack me);
static inline size_t reiStackSize(ReiStack me);
static inline size_t reiStackCapacity(ReiStack me);
static inline bool reiStackEmpty(ReiStack me);
static inline bool reiStackFull(ReiStack me);
static inline void reiStackSetTp(ReiStack me, ReiValue* newTp);
static inline void reiStackSetBp(ReiStack me, ReiValue* newBp);
static inline void reiStackPush(ReiStack me, ReiValue value);
static inline ReiValue reiStackPop(ReiStack me);
static inline bool reiStackCheck(ReiStack me, uint32_t count);

#pragma region Impl
static inline void reiStackInit(ReiStack me)
{
    me->bp = data;
    me->tp = data;
}
static inline ReiValue* reiStackBp(ReiStack me)
{
    return me->bp;
}
static inline ReiValue* reiStackTp(ReiStack me)
{
    return me->tp;
}
static inline ReiValue* reiStackData(ReiStack me)
{
    return me->data;
}
static inline size_t reiStackSize(ReiStack me)
{
    return (size_t)(st->tp - st->data);
}
static inline size_t reiStackCapacity(ReiStack me)
{
    return REI_MAX_STACK_SIZE;
}
static inline bool reiStackEmpty(ReiStack me)
{
    return st->tp == st->data;
}
static inline bool reiStackFull(ReiStack me)
{
    return st->tp >= st->data + REI_MAX_STACK_SIZE;
}
static inline void reiStackSetTp(ReiStack me, ReiValue* newTp)
{
    me->tp = newTp;
}
static inline void reiStackSetBp(ReiStack me, ReiValue* newBp)
{
    me->bp = newBp;
}
static inline void reiStackPush(ReiStack me, ReiValue value)
{
    me->tp = value;
    me->tp++;
}
static inline ReiValue reiStackPop(ReiStack me)
{
    me->tp--;
    return *me->tp;
}
static inline bool reiStackCheck(ReiStack me, uint32_t count)
{
    return ((st->tp + n) <= (st->data + REI_MAX_STACK_SIZE));
}
#pragma endregion

#endif
