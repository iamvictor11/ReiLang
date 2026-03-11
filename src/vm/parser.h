#ifndef LUNA_PARSER_PARSER_H
#define LUNA_PARSER_PARSER_H
#include "common/token.h"
#include "common/chunk.h"

typedef struct luna_Parser
{
    struct
    {
        luna_Token *prev;
        luna_Token *curr;
        luna_Token *next;
    } cursor;
    luna_TokenArray tokens;
    luna_Chunk chunk;
} luna_Parser;

void lunaParser_start(luna_Parser *self, luna_TokenArray tokens);

#endif
