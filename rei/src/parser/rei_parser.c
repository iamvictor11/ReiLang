#include "rei_parser.h"
#include "rei_parser.rule.i"
#include "../rei_debug.h"

typedef struct Rei_ParserState
{
    const ReiToken* prev;
    const ReiToken* curr;
    ReiResult res;
} Rei_ParserState;
static Rei_ParserState parserState_ = {0};

#pragma region Private
static inline const ReiToken* peekPrev_(void);
static inline const ReiToken* peek_(void);
static inline const ReiToken* peekNext_(void);
static inline const ReiToken* peekNextNext_(void);
static inline const ReiToken* advance_(void);
static inline const ReiToken* match_(ReiTokenKind tkk);
static inline void consume_(ReiTokenKind tkk, const char* message);
static inline bool isAtEnd_(void);
static inline void error_(const char* message);
static inline ReiAstNode* declaration_(void);
static inline ReiAstNode* mutDecl_(void);
static inline ReiAstNode* konDecl_(void);
static inline ReiAstNode* funcDecl_(void);
static inline ReiAstNode* classDecl_(void);
static inline ReiAstNode* expression_(void);
static inline ReiAstNode* statement_(void);
static inline ReiAstNode* blockStmt_(void);
static inline ReiAstNode* whenStmt_(void);
static inline ReiAstNode* loopStmt_(void);
static inline ReiAstNode* breakStmt_(void);
static inline ReiAstNode* continueStmt_(void);
static inline ReiAstNode* switchStmt_(void);
static inline ReiAstNode* passStmt_(void);
static inline ReiAstNode* returnStmt_(void);
static inline ReiAstNode* thisStmt_(void);
static inline ReiAstNode* superStmt_(void);
#pragma endregion
#pragma region Public
bool reiParserInit(ReiParser* me, const ReiTokenBuffer_T* tokens)
{
    me->tokens = tokens;
    me->ast = reiAstNodeCreateEmpty(REI_AST_NODE_TYPE_ROOT, (ReiToken){});
    if (me->ast == NULL)
        return false;
    parserState_.curr = me->tokens->data;
    parserState_.res = REI_RESULT_SUCCESS;
    return true;
}
void reiParserFree(ReiParser* me)
{
    reiAstNodeDestroy(me->ast, true);
}
ReiResult reiParserStart(ReiParser* me)
{
    if (me == NULL || me->tokens == NULL) return REI_RESULT_PARSER_ERROR;
    while (!isAtEnd_())
    {
        ReiAstNode* child = declaration_();
        reiAstNodeAddChild(me->ast, child);
        if (parserState_.res != REI_RESULT_SUCCESS) break;
        if (REI_HAS_ERROR)
        {
            parserState_.res = REI_RESULT_PARSER_ERROR;
            break;
        }
    }
#if REI_ENABLE_DEBUG >= REI_DEBUG_LEVEL_TRACE
#endif
    return parserState_.res;
}
#pragma endregion
#pragma region PIMPL
static inline const ReiToken* peekPrev_(void)
{
    if (parserState_.prev == NULL) return NULL;
    return parserState_.prev;
}
static inline const ReiToken* peek_(void)
{
    return parserState_.curr;
}
static inline const ReiToken* peekNext_(void)
{
    if (peek_() == NULL) return NULL;
    if (peek_()->kind == REI_TOKEN_KIND_EOF) return NULL;
    return &(parserState_.curr[1]);
}
static inline const ReiToken* peekNextNext_(void)
{
    if (peekNext_() == NULL) return NULL;
    if (peekNext_()->kind == REI_TOKEN_KIND_EOF) return NULL;
    return &(parserState_.curr[2]);
}
static inline const ReiToken* advance_(void)
{
    const ReiToken* tk = peek_();
    parserState_.curr++;
    return tk;
}
static inline const ReiToken* match_(ReiTokenKind tkk)
{
    if (parserState_.curr != NULL && parserState_.curr->kind == tkk)
        return advance_();
    return NULL;
}
static inline void consume_(ReiTokenKind tkk, const char* message)
{
    if (parserState_.curr != NULL && parserState_.curr->kind == tkk)
    {
        advance_();
        return;
    }
    error_(message);
}
static inline bool isAtEnd_(void)
{
    return peek_() == NULL || peek_()->kind == REI_TOKEN_KIND_EOF;
}
static inline void error_(const char* message)
{
    REI_DEBUG_LOG_ERROR(message);
    parserState_.res = REI_RESULT_PARSER_ERROR;
}
#pragma region Decl
static inline ReiAstNode* declaration_(void)
{
    if (match_(REI_TOKEN_KIND_MUT))
    {
    }
    else if (match_(REI_TOKEN_KIND_KON))
    {
    }
    else if (match_(REI_TOKEN_KIND_FUNC))
    {
    }
    else if (match_(REI_TOKEN_KIND_CLASS))
    {
    }
    return statement_();
}
static inline ReiAstNode* mutDecl_(void)
{
    return NULL;
}
static inline ReiAstNode* konDecl_(void)
{
    return NULL;
}
static inline ReiAstNode* funcDecl_(void)
{
    return NULL;
}
static inline ReiAstNode* classDecl_(void)
{
    return NULL;
}
#pragma endregion
#pragma region Expr
static inline ReiAstNode* expression_(void)
{
    return NULL;
}
static inline ReiAstNode* groupExpr_(ReiToken token, bool canAssign)
{
    return NULL;
}
static inline ReiAstNode* callExpr_(ReiToken token, ReiAstNode* left, bool canAssign)
{
    return NULL;
}
static inline ReiAstNode* questionExpr_(ReiToken token, ReiAstNode* left, bool canAssign)
{
    return NULL;
}
static inline ReiAstNode* dotExpr_(ReiToken token, ReiAstNode* left, bool canAssign)
{
    return NULL;
}
static inline ReiAstNode* dotdotExpr_(ReiToken token, ReiAstNode* left, bool canAssign)
{
    return NULL;
}
static inline ReiAstNode* binaryExpr_(ReiToken token, ReiAstNode* left, bool canAssign)
{
    return NULL;
}
static inline ReiAstNode* unaryExpr_(ReiToken token, bool canAssign)
{
    return NULL;
}
static inline ReiAstNode* literalExpr_(ReiToken token, bool canAssign)
{
    return NULL;
}
static inline ReiAstNode* identifierExpr_(ReiToken token, bool canAssign)
{
    return NULL;
}
#pragma endregion
#pragma region Stmt
static inline ReiAstNode* statement_(void)
{
    return NULL;
}
static inline ReiAstNode* blockStmt_(void)
{
    return NULL;
}
static inline ReiAstNode* whenStmt_(void)
{
    return NULL;
}
static inline ReiAstNode* loopStmt_(void)
{
    return NULL;
}
static inline ReiAstNode* breakStmt_(void)
{
    return NULL;
}
static inline ReiAstNode* continueStmt_(void)
{
    return NULL;
}
static inline ReiAstNode* switchStmt_(void)
{
    return NULL;
}
static inline ReiAstNode* passStmt_(void)
{
    return NULL;
}
static inline ReiAstNode* returnStmt_(void)
{
    return NULL;
}
static inline ReiAstNode* thisStmt_(void)
{
    return NULL;
}
static inline ReiAstNode* superStmt_(void)
{
    return NULL;
}
#pragma endregion
#pragma endregion
