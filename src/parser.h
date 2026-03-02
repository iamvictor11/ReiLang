#ifndef LUNA_PARSER_H
#define LUNA_PARSER_H

#include "lexer.h"
#include "ast.h"
#include "error.h"

typedef struct luna_Parser
{
    luna_VM *vm;
    luna_TokenInfo *token_infos;
    int token_info_count;
    int current_token_info;
    luna_Error error;
    bool has_error;
} luna_Parser;

void lunaParser_Init(luna_Parser *parser, luna_VM *vm);
void lunaParser_Free(luna_Parser *parser);
void lunaParser_SetTokens(luna_Parser *parser, luna_TokenInfo *tokens, int count);
luna_ASTNode *lunaParser_Load(luna_Parser *parser, luna_TokenInfo *token_infos);
bool lunaParser_HasError(luna_Parser *parser);
luna_Error lunaParser_GetError(luna_Parser *parser);

#endif