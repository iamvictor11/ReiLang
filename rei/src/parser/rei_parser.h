#ifndef REI_PARSER_PARSER_H
#define REI_PARSER_PARSER_H

#include "rei_ast.h"
#include "../lexer/rei_token.h"

typedef struct ReiParser
{
    ReiTokenBuffer tokens;
    ReiAst ast;
} ReiParser;
void reiParserInit(ReiParser* me, ReiTokenBuffer tokens);
ReiResult reiParserStart(ReiParser* me);
void reiParserFree(ReiParser* me);

#endif
