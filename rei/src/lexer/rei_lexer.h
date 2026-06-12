#ifndef REI_LEXER_LEXER_H
#define REI_LEXER_LEXER_H

#include "rei_token.h"

typedef struct ReiLexer
{
    const char* source;
    ReiTokenBuffer tokens;
} ReiLexer;
void reiLexerInit(ReiLexer* me, const char* source);
void reiLexerStart(ReiLexer* me);
void reiLexerFree(ReiLexer* me);

#endif
