#ifndef LUNA_ERROR_H
#define LUNA_ERROR_H

#include "base.h"

typedef struct luna_Error
{
    int line;
    int column;
    luna_String *message;
} luna_Error;

luna_Error luna_MakeError(int line, int column, const char *format, ...);
void lunaError_Free(luna_Error *error);
void lunaError_Copy(luna_Error *dest, const luna_Error *src);
bool lunaError_IsValid(const luna_Error *error);
void lunaError_Clear(luna_Error *error);

#endif