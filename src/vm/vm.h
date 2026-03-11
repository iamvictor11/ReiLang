#ifndef LUNA_VM_VM_H
#define LUNA_VM_VM_H
#include "common/chunk.h"
#include "config.h"

typedef enum luna_Result
{
  LUNA_OK,
  LUNA_ERROR
} luna_Result;

typedef struct luna_VM
{
    luna_Chunk chunk;
    luna_Byte *ip;
    luna_Value stack[LUNA_STACK_MAX];
    luna_Value *stack_top;
} luna_VM;

void lunaVM_init(luna_VM *self);
void lunaVM_free(luna_VM *self);
luna_Result lunaVM_loadFile(luna_VM *self, const char *path);
luna_Result lunaVM_loadSrc(luna_VM *self, const char *source);
luna_Result lunaVM_run(luna_VM *self);
void lunaVM_push(luna_VM *self, luna_Value value);
luna_Value lunaVM_pop(luna_VM *self);

#endif