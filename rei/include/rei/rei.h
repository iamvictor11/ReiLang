#ifndef REI_REI_H
#define REI_REI_H

#define REI_NOT_USE_UPPERCASE

typedef enum ReiStatus
{
    REI_RESULT_SUCCESS,
    REI_RESULT_COMPILE_ERROR,
    REI_RESULT_RUNTIME_ERROR
} ReiStatus;
typedef struct ReiVM_T ReiVM_T;
typedef struct ReiVM_T* ReiVM;

#endif
