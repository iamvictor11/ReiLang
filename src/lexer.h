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
    luna_Var value;
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
    luna_VM *vm;
    const char *source;
    const char *current_char;
    int line;
    int column;
    luna_String current_token_text;
    luna_TokenInfoList token_info_list;
    luna_Error error;
    bool has_error;
} luna_Lexer;

void lunaLexer_Init(luna_Lexer *lexer, luna_VM *vm);
void lunaLexer_Free(luna_Lexer *lexer);
void lunaLexer_Load(luna_Lexer *lexer, const char *source);
bool lunaLexer_HasError(luna_Lexer *lexer);
luna_Error lunaLexer_GetError(luna_Lexer *lexer);

#ifdef LUNA_DEBUG_ENABLE
const char *lunaTokenInfo_ToString(luna_TokenInfo info);
const char *lunaTokenInfoList_ToString(luna_TokenInfoList *list);
#endif

#endif