#include "error.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

luna_Error luna_MakeError(int line, int column, const char *format, ...)
{
    luna_Error error;
    error.line = line;
    error.column = column;
    error.message = LUNA_NULL_STRING;
    lunaString_Malloc(&(error.message), 64);
    va_list args;
    va_start(args, format);
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    lunaString_Append(&(error.message), buffer);
    return error;
}
void lunaError_Free(luna_Error *error)
{
    if (error && !lunaString_IsEmpty(&(error->message)))
        lunaString_Free(&(error->message));
    error->line = 0;
    error->column = 0;
}
void lunaError_Copy(luna_Error *dest, const luna_Error *src)
{
    if (!dest || !src)
        return;
    lunaError_Free(dest);
    dest->line = src->line;
    dest->column = src->column;
    dest->message = LUNA_NULL_STRING;
    if (!lunaString_IsEmpty(&(src->message)))
    {
        lunaString_Malloc(&(dest->message), src->message.len + 1);
        lunaString_AppendObj(&(dest->message), &(src->message));
    }
}
bool lunaError_IsValid(const luna_Error *error)
{
    return error && !lunaString_IsEmpty(&(error->message));
}