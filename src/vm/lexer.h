#ifndef LUNA_LEXER_LEXER_H
#define LUNA_LEXER_LEXER_H
#include "common/common.h"
#include "common/token.h"

typedef struct luna_Lexer
{
    struct
    {
        const char *start;
        const char *curr;
        luna_Position pos;
    } cursor;
    const char *source;
    size_t source_len;
    luna_TokenArray tokens;
} luna_Lexer;

void lunaLexer_start(luna_Lexer *self, const char *source);

#endif