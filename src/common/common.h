#ifndef LUNA_COMMON_COMMON_H
#define LUNA_COMMON_COMMON_H
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t luna_Byte;
typedef uint32_t luna_Hash;

typedef struct luna_Position
{
    size_t line;
    size_t column;
} luna_Position;

typedef struct luna_StringView
{
    const char *start;
    size_t len;
} luna_StringView;


#endif
