#ifndef REI_PARSER_PARSER_RULE_I
#define REI_PARSER_PARSER_RULE_I

#include "rei_parser.h"

typedef enum Rei_Precedence
{
    REI_PREC_NONE,
    REI_PREC_ASSIGN,
    REI_PREC_OR,
    REI_PREC_AND,
    REI_PREC_EQUAL,
    REI_PREC_COMPARE,
    REI_PREC_TERM,
    REI_PREC_FACTOR,
    REI_PREC_UNARY,
    REI_PREC_CALL,
    REI_PREC_PRIMARY,
    REI_PREC_UNDEFINE,
    REI_PREC_MAX_COUNT
} Rei_Precedence;
typedef ReiAstNode*(*ReiParsePrefixFn)(ReiParser* parser, ReiToken token, bool canAssign);
typedef ReiAstNode*(*ReiParseInfixFn)(ReiParser* parser, ReiToken token, ReiAstNode* left, bool canAssign);
typedef struct Rei_ParseRule
{
    ReiParsePrefixFn prefix;
    ReiParseInfixFn infix;
    Rei_Precedence precedence;
    bool outset;
} Rei_ParseRule;
static inline ReiAstNode* group_(ReiParser* parser, ReiToken token, bool canAssign);
static inline ReiAstNode* call_(ReiParser* parser, ReiToken token, ReiAstNode* left, bool canAssign);
static inline ReiAstNode* question_(ReiParser* parser, ReiToken token, ReiAstNode* left, bool canAssign);
static inline ReiAstNode* dot_(ReiParser* parser, ReiToken token, ReiAstNode* left, bool canAssign);
static inline ReiAstNode* dotdot_(ReiParser* parser, ReiToken token, ReiAstNode* left, bool canAssign);
static inline ReiAstNode* binary_(ReiParser* parser, ReiToken token, ReiAstNode* left, bool canAssign);
static inline ReiAstNode* unary_(ReiParser* parser, ReiToken token, bool canAssign);
static Rei_ParseRule parseRules[REI_TOKEN_KIND_MAX_COUNT] =
{
    [REI_TOKEN_KIND_LEFT_PAREN]     = {group_,  call_,  REI_PREC_CALL,  true},  // (
    [REI_TOKEN_KIND_RIGHT_PAREN]    = {NULL,    NULL,   REI_PREC_NONE,  false}, // )
    [REI_TOKEN_KIND_LEFT_BRACKET]   = {NULL,    NULL,   REI_PREC_UNDEFINE,  true},  // [
    [REI_TOKEN_KIND_RIGHT_BRACKET]  = {NULL,    NULL,   REI_PREC_UNDEFINE,  false}, // ]
    [REI_TOKEN_KIND_LEFT_BRACE]     = {NULL,    NULL,   REI_PREC_NONE,  true},  // {
    [REI_TOKEN_KIND_RIGHT_BRACE]    = {NULL,    NULL,   REI_PREC_NONE,  false}, // }

    [REI_TOKEN_KIND_LEFT_BRACKET_BRACKET]   = {NULL,    NULL,   REI_PREC_UNDEFINE,  true},  // [[
    [REI_TOKEN_KIND_RIGHT_BRACKET_BRACKET]  = {NULL,    NULL,   REI_PREC_UNDEFINE,  false}, // ]]

    [REI_TOKEN_KIND_COMMA]          = {NULL,    NULL,       REI_PREC_NONE,  false}, // ,
    [REI_TOKEN_KIND_QUESTION]       = {NULL,    question_,  REI_PREC_CALL,  false}, // ?
    [REI_TOKEN_KIND_DOT]            = {NULL,    dot_,       REI_PREC_CALL,  false}, // .
    [REI_TOKEN_KIND_DOT_DOT]        = {NULL,    dotdot_,    REI_PREC_CALL,  false}, // ..
    [REI_TOKEN_KIND_DOT_DOT_DOT]    = {NULL,    NULL,       REI_PREC_UNDEFINE,  false}, // ...
    [REI_TOKEN_KIND_COLON]          = {NULL,    NULL,       REI_PREC_NONE,  false}, // :
    [REI_TOKEN_KIND_COLON_COLON]    = {NULL,    NULL,       REI_PREC_UNDEFINE,  false}, // ::
    [REI_TOKEN_KIND_SEMICOLON]      = {NULL,    NULL,       REI_PREC_NONE,  true}, // ;

    [REI_TOKEN_KIND_PLUS]       = {unary_,  binary_, REI_PREC_TERM,     false}, // +
    [REI_TOKEN_KIND_MINUS]      = {NULL,    binary_, REI_PREC_TERM,     true},  // -
    [REI_TOKEN_KIND_STAR]       = {NULL,    binary_, REI_PREC_FACTOR,   false}, // *
    [REI_TOKEN_KIND_SLASH]      = {NULL,    binary_, REI_PREC_FACTOR,   false}, // /
    [REI_TOKEN_KIND_MODULO]     = {NULL,    binary_, REI_PREC_FACTOR,   false}, // %
    [REI_TOKEN_KIND_STAR_STAR]  = {NULL,    binary_, REI_PREC_FACTOR,   false}, // **
    
    [REI_TOKEN_KIND_EOF] = {NULL, NULL, REI_PREC_NONE, true}
};

#endif
