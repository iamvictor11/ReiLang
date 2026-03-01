#ifndef LUNA_LEXER_H
#define LUNA_LEXER_H

#include "base.h"
#include "token.h"
#include "error.h"
#include <stdio.h>

typedef struct luna_TokenInfo
{
    luna_Token type;
    luna_String text;
    int line;
    int column;
} luna_TokenInfo;
typedef struct luna_TokenInfoList
{
    luna_TokenInfo *tokens;
    int size;
    int capacity;
} luna_TokenInfoList;
typedef struct luna_Lexer
{
    const char *source;
    const char *current;
    int line;
    int column;
    luna_String current_token_text;
    luna_TokenInfoList token_info_list;
    luna_Error error;
    bool has_error;
} luna_Lexer;

void lunaLexer_Init(luna_Lexer *lexer);
void lunaLexer_Free(luna_Lexer *lexer);
void lunaLexer_Load(luna_Lexer *lexer, const char *source);
bool lunaLexer_HasError(luna_Lexer *lexer);
luna_Error lunaLexer_GetError(luna_Lexer *lexer);

#endif