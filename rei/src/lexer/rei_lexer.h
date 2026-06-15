#ifndef REI_LEXER_LEXER_H
#define REI_LEXER_LEXER_H

#include "rei_token.h"

typedef struct ReiLexer
{
    const char* source;
    ReiTokenBuffer tokens;
} ReiLexer;
bool reiLexerInit(ReiLexer* me, const char* source);
ReiResult reiLexerStart(ReiLexer* me);
void reiLexerFree(ReiLexer* me);

#endif
