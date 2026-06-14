#include "rei_parser.h"
#include "rei_parser.rule.i"

typedef struct Rei_ParserState
{
    const ReiToken* prev;
    const ReiToken* curr;
    ReiResult res;
} Rei_ParserState;
static Rei_ParserState parserState_ = {0};

#pragma region Private
#pragma endregion
#pragma region Public
void reiParserInit(ReiParser* me, ReiTokenBuffer tokens)
{
    me->tokens = tokens;
    parserState_.curr = me->tokens->data;
    parserState_.res = REI_RESULT_SUCCESS;
}
ReiResult reiParserStart(ReiParser* me)
{
    if (me == NULL || me->tokens == NULL) return REI_RESULT_PARSER_ERROR;
}
void reiParserFree(ReiParser* me)
{
}
#pragma endregion
#pragma region PIMPL
static inline ReiAstNode* group_(ReiParser* parser, ReiToken token, bool canAssign)
{
    return NULL;
}
static inline ReiAstNode* call_(ReiParser* parser, ReiToken token, ReiAstNode* left, bool canAssign)
{
    return NULL;
}
#pragma endregion
