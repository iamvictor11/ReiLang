#ifndef REI_COMMON_COMMON_H
#define REI_COMMON_COMMON_H

typedef enum ReiStatus
{
    REI_RESULT_SUCCESS,
    REI_RESULT_COMPILE_ERROR,
    REI_RESULT_RUNTIME_ERROR
} ReiStatus;
typedef enum ReiLifeCycle
{
    REI_LIFE_CYCLE_EDEN,
    REI_LIFE_CYCLE_YOUNG,
    REI_LIFE_CYCLE_OLD,
    REI_LIFE_CYCLE_PERMANENT
} ReiLifeCycle;
typedef struct ReiVM_T ReiVM_T;
typedef struct ReiVM_T* ReiVM;

#endif
