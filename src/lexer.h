#ifndef LUNA_LEXER_H
#define LUNA_LEXER_H

#include "base.h"
#include "token.h"
#include "error.h"
#include <stdio.h>

typedef struct luna_Lexer
{
    luna_VM *vm;
    luna_Error error;
    const char *source;
    const char *current;
    int line;
    int column;
    luna_String *current_token_text;
    bool has_error;
} luna_Lexer;

typedef struct luna_TokenInfo
{
    luna_Token type;
    luna_String *text;
    int line;
    int column;
} luna_TokenInfo;

void lunaLexer_Init(luna_Lexer *lexer, luna_VM *vm, const char *source);
void lunaLexer_Free(luna_Lexer *lexer);
luna_TokenInfo lunaLexer_NextToken(luna_Lexer *lexer);
bool lunaLexer_IsAtEnd(luna_Lexer *lexer);
bool lunaLexer_HasError(luna_Lexer *lexer);
luna_Error lunaLexer_GetError(luna_Lexer *lexer);
void lunaLexer_ClearError(luna_Lexer *lexer);

#endif