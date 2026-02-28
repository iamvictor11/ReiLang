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
    error.message = (luna_String *)malloc(sizeof(luna_String));
    if (error.message)
    {
        *error.message = LUNA_NULL_STRING;
        lunaString_Malloc(error.message, 64);
        va_list args;
        va_start(args, format);
        char buffer[256];
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);
        lunaString_Append(error.message, buffer);
    }
    return error;
}
void lunaError_Free(luna_Error *error)
{
    if (error && error->message)
    {
        lunaString_Free(error->message);
        free(error->message);
        error->message = NULL;
    }
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
    if (src->message && src->message->data)
    {
        dest->message = (luna_String *)malloc(sizeof(luna_String));
        if (dest->message)
        {
            *dest->message = LUNA_NULL_STRING;
            lunaString_Malloc(dest->message, src->message->len + 1);
            lunaString_AppendObj(dest->message, src->message);
        }
    }
    else
    {
        dest->message = NULL;
    }
}
bool lunaError_IsValid(const luna_Error *error)
{
    return error && error->message && error->message->data && error->message->len > 0;
}
void lunaError_Clear(luna_Error *error)
{
    if (error)
    {
        lunaError_Free(error);
        error->line = 0;
        error->column = 0;
        error->message = NULL;
    }
}