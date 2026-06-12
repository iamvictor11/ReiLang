#ifndef REI_LEXER_LEXER_H
#define REI_LEXER_LEXER_H

#include "rei_token.h"

typedef struct ReiLexer
{
    ReiString_T script;
    ReiTokenBuffer tokens;
} ReiLexer;


#endif
