#include "vm.h"
#include <stdio.h>
#include <stdlib.h>

static char *_readFile(const char *path);

#pragma region Helper
static char *_readFile(const char *path)
{
    FILE* file = fopen(path, "rb");
    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);
    char* buffer = (char*)malloc(fileSize + 1);
    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    buffer[bytesRead] = '\0';
    fclose(file);
    return buffer;
}
#pragma endregion

void lunaVM_init(luna_VM *self)
{
    lunaChunk_init(&self->chunk);
    self->ip = NULL;
    self->stack_top = self->stack;
}
void lunaVM_free(luna_VM *self)
{
    lunaChunk_free(&self->chunk);
    self->ip = NULL;
    self->stack_top = self->stack;
}
luna_Result lunaVM_loadFile(luna_VM *self, const char *path)
{
    char *source = _readFile(path);
    free(source);
    return LUNA_OK;
}
luna_Result lunaVM_loadSrc(luna_VM *self, const char *source)
{
    return LUNA_OK;
}
luna_Result lunaVM_run(luna_VM *self)
{
#define LUNA_READ_BYTE() (*self->ip++)
#define LUNA_READ_CONSTANT() (self->chunk.constants.data[LUNA_READ_BYTE()])
#define LUNA_BINARY_OP(op)           \
    do                               \
    {                                \
        double b = lunaVM_pop(self); \
        double a = lunaVM_pop(self); \
        lunaVM_push(self, a op b);   \
    } while (false)
    for (;;)
    {
        luna_Byte instruction;
        switch (instruction = LUNA_READ_BYTE())
        {
        case LUNA_OP_CONSTANT:
        {
            luna_Value constant = LUNA_READ_CONSTANT();
            lunaVM_push(self, constant);
            break;
        }
        case LUNA_OP_RETURN:
            lunaVM_pop(self);
            break;
        case LUNA_OP_NEG:
            lunaVM_push(self, -lunaVM_pop(self));
            break;
        case LUNA_OP_ADD:
            LUNA_BINARY_OP(+);
            break;
        case LUNA_OP_SUB:
            LUNA_BINARY_OP(-);
            break;
        case LUNA_OP_MUL:
            LUNA_BINARY_OP(*);
            break;
        case LUNA_OP_DIV:
            LUNA_BINARY_OP(/);
            break;
        // case LUNA_OP_MOD:
        //     LUNA_BINARY_OP(%);
        //     break;
        default:
            break;
        }
    }
#undef LUNA_READ_BYTE
#undef LUNA_READ_CONSTANT
#undef LUNA_BINARY_OP
    return LUNA_OK;
}
void lunaVM_push(luna_VM *self, luna_Value value)
{
    *self->stack_top = value;
    self->stack_top++;
}
luna_Value lunaVM_pop(luna_VM *self)
{
    self->stack_top--;
    return *self->stack_top;
}
