#ifndef REI_PARSER_PARSER_H
#define REI_PARSER_PARSER_H

#include "rei_ast.h"
#include "../lexer/rei_token.h"

typedef struct ReiParser
{
    const ReiTokenBuffer_T* tokens;
    ReiAst* ast;
} ReiParser;
bool reiParserInit(ReiParser* me, const ReiTokenBuffer_T* tokens);
ReiResult reiParserStart(ReiParser* me);
void reiParserFree(ReiParser* me);

#endif
