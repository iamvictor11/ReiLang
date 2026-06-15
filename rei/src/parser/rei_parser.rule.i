#ifndef REI_PARSER_PARSER_RULE_I
#define REI_PARSER_PARSER_RULE_I

#include "rei_parser.h"

typedef enum Rei_Precedence
{
    REI_PREC_NONE,
    REI_PREC_ASSIGN,    // = x= :=
    REI_PREC_LOR,       // ||
    REI_PREC_LAND,      // &&
    REI_PREC_BOR,       // |
    REI_PREC_BXOR,      // ^
    REI_PREC_BAND,      // &
    REI_PREC_EQUAL,     // == !=
    REI_PREC_COMPARE,   // < > <= >= <=>
    REI_PREC_SHIFT,     // << >>
    REI_PREC_TERM,      // + -
    REI_PREC_FACTOR,    // * / %
    REI_PREC_POW,       // **
    REI_PREC_UNARY,     // ~ !
    REI_PREC_CALL,      // ()
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
    bool startPoint;
} Rei_ParseRule;
static inline ReiAstNode* group_(ReiParser* parser, ReiToken token, bool canAssign);
static inline ReiAstNode* call_(ReiParser* parser, ReiToken token, ReiAstNode* left, bool canAssign);
static inline ReiAstNode* question_(ReiParser* parser, ReiToken token, ReiAstNode* left, bool canAssign);
static inline ReiAstNode* dot_(ReiParser* parser, ReiToken token, ReiAstNode* left, bool canAssign);
static inline ReiAstNode* dotdot_(ReiParser* parser, ReiToken token, ReiAstNode* left, bool canAssign);
static inline ReiAstNode* binary_(ReiParser* parser, ReiToken token, ReiAstNode* left, bool canAssign);
static inline ReiAstNode* unary_(ReiParser* parser, ReiToken token, bool canAssign);
static inline ReiAstNode* literal_(ReiParser* parser, ReiToken token, bool canAssign);
static inline ReiAstNode* identifier_(ReiParser* parser, ReiToken token, bool canAssign);
static Rei_ParseRule parseRules[REI_TOKEN_KIND_MAX_COUNT] =
{
    /* 区域 */
    [REI_TOKEN_KIND_LEFT_PAREN]     = {group_,  call_,  REI_PREC_CALL,  true},  // (
    [REI_TOKEN_KIND_RIGHT_PAREN]    = {NULL,    NULL,   REI_PREC_NONE,  false}, // )
    [REI_TOKEN_KIND_LEFT_BRACKET]   = {NULL,    NULL,   REI_PREC_UNDEFINE,  true},  // [
    [REI_TOKEN_KIND_RIGHT_BRACKET]  = {NULL,    NULL,   REI_PREC_UNDEFINE,  false}, // ]
    [REI_TOKEN_KIND_LEFT_BRACE]     = {NULL,    NULL,   REI_PREC_NONE,  true},  // {
    [REI_TOKEN_KIND_RIGHT_BRACE]    = {NULL,    NULL,   REI_PREC_NONE,  false}, // }

    [REI_TOKEN_KIND_LEFT_BRACKET_BRACKET]   = {NULL,    NULL,   REI_PREC_UNDEFINE,  true},  // [[
    [REI_TOKEN_KIND_RIGHT_BRACKET_BRACKET]  = {NULL,    NULL,   REI_PREC_UNDEFINE,  false}, // ]]
    /* 符号 */
    [REI_TOKEN_KIND_COMMA]          = {NULL,    NULL,       REI_PREC_NONE,  false}, // ,
    [REI_TOKEN_KIND_QUESTION]       = {NULL,    question_,  REI_PREC_CALL,  false}, // ?
    [REI_TOKEN_KIND_DOT]            = {NULL,    dot_,       REI_PREC_CALL,  false}, // .
    [REI_TOKEN_KIND_DOT_DOT]        = {NULL,    dotdot_,    REI_PREC_CALL,  false}, // ..
    [REI_TOKEN_KIND_DOT_DOT_DOT]    = {NULL,    NULL,       REI_PREC_UNDEFINE,  false}, // ...
    [REI_TOKEN_KIND_COLON]          = {NULL,    NULL,       REI_PREC_NONE,  false}, // :
    [REI_TOKEN_KIND_COLON_COLON]    = {NULL,    NULL,       REI_PREC_UNDEFINE,  false}, // ::
    [REI_TOKEN_KIND_SEMICOLON]      = {NULL,    NULL,       REI_PREC_NONE,  true}, // ;
    /* 数学运算 */
    [REI_TOKEN_KIND_PLUS]       = {unary_,  binary_, REI_PREC_TERM,     false}, // +
    [REI_TOKEN_KIND_MINUS]      = {NULL,    binary_, REI_PREC_TERM,     true},  // -
    [REI_TOKEN_KIND_STAR]       = {NULL,    binary_, REI_PREC_FACTOR,   false}, // *
    [REI_TOKEN_KIND_SLASH]      = {NULL,    binary_, REI_PREC_FACTOR,   false}, // /
    [REI_TOKEN_KIND_MODULO]     = {NULL,    binary_, REI_PREC_FACTOR,   false}, // %
    [REI_TOKEN_KIND_STAR_STAR]  = {NULL,    binary_, REI_PREC_POW,      false}, // **
    /* 位运算 */
    [REI_TOKEN_KIND_AND]                = {NULL,    binary_,    REI_PREC_BAND,  false}, // &
    [REI_TOKEN_KIND_PIPE]               = {NULL,    binary_,    REI_PREC_BOR,   false}, // |
    [REI_TOKEN_KIND_CTRL]               = {NULL,    binary_,    REI_PREC_BXOR,  false}, // ^
    [REI_TOKEN_KIND_WAVE]               = {unary_,  NULL,       REI_PREC_UNARY, true},  // ~
    [REI_TOKEN_KIND_LESS_LESS]          = {NULL,    binary_,    REI_PREC_SHIFT, false}, // <<
    [REI_TOKEN_KIND_GREATER_GREATER]    = {NULL,    binary_,    REI_PREC_SHIFT, false}, // >>
    /* 逻辑运算 */
    [REI_TOKEN_KIND_AND_AND]    = {NULL,    binary_,    REI_PREC_LAND,  false}, // &&
    [REI_TOKEN_KIND_PIPE_PIPE]  = {NULL,    binary_,    REI_PREC_LOR,   false}, // ||
    [REI_TOKEN_KIND_BANG]       = {unary_,  NULL,       REI_PREC_UNARY, true},  // !
    /* 比较运算 */
    [REI_TOKEN_KIND_LESS]           = {NULL, binary_, REI_PREC_COMPARE, false}, // <
    [REI_TOKEN_KIND_GREATER]        = {NULL, binary_, REI_PREC_COMPARE, false}, // >
    [REI_TOKEN_KIND_EQUAL_EQUAL]    = {NULL, binary_, REI_PREC_EQUAL, false},   // ==
    [REI_TOKEN_KIND_BANG_EQUAL]     = {NULL, binary_, REI_PREC_EQUAL, false},   // !=
    [REI_TOKEN_KIND_LESS_EQUAL]     = {NULL, binary_, REI_PREC_COMPARE, false}, // <=
    [REI_TOKEN_KIND_GREATER_EQUAL]  = {NULL, binary_, REI_PREC_COMPARE, false}, // >=
    [REI_TOKEN_KIND_COMPARE]        = {NULL, binary_, REI_PREC_COMPARE, false}, // <=>
    /* 赋值运算 */
    [REI_TOKEN_KIND_WALRUS] = {NULL, binary_, REI_PREC_ASSIGN, false},  // :=
    [REI_TOKEN_KIND_EQUAL]  = {NULL, binary_, REI_PREC_NONE, false},    // =
    /* 自数学运算 */
    [REI_TOKEN_KIND_PLUS_EQUAL]         = {NULL, binary_, REI_PREC_EQUAL, false}, // +=
    [REI_TOKEN_KIND_MINUS_EQUAL]        = {NULL, binary_, REI_PREC_EQUAL, false}, // -=
    [REI_TOKEN_KIND_STAR_EQUAL]         = {NULL, binary_, REI_PREC_EQUAL, false}, // *=
    [REI_TOKEN_KIND_SLASH_EQUAL]        = {NULL, binary_, REI_PREC_EQUAL, false}, // /=
    [REI_TOKEN_KIND_MODULO_EQUAL]       = {NULL, binary_, REI_PREC_EQUAL, false}, // %=
    [REI_TOKEN_KIND_STAR_STAR_EQUAL]    = {NULL, binary_, REI_PREC_EQUAL, false}, // **=
    /* 自位运算 */
    [REI_TOKEN_KIND_AND_EQUAL]              = {NULL, binary_, REI_PREC_EQUAL, false}, // &=
    [REI_TOKEN_KIND_PIPE_EQUAL]             = {NULL, binary_, REI_PREC_EQUAL, false}, // |=
    [REI_TOKEN_KIND_CTRL_EQUAL]             = {NULL, binary_, REI_PREC_EQUAL, false}, // ^=
    [REI_TOKEN_KIND_WAVE_EQUAL]             = {NULL, binary_, REI_PREC_EQUAL, false}, // ~=
    [REI_TOKEN_KIND_LESS_LESS_EQUAL]        = {NULL, binary_, REI_PREC_EQUAL, false}, // <<=
    [REI_TOKEN_KIND_GREATER_GREATER_EQUAL]  = {NULL, binary_, REI_PREC_EQUAL, false}, // >>=
    
    [REI_TOKEN_KIND_LEFT_ARROW]     = {NULL, NULL, REI_PREC_UNDEFINE, false}, // <-
    [REI_TOKEN_KIND_RIGHT_ARROW]    = {NULL, NULL, REI_PREC_UNDEFINE, false}, // ->

    [REI_TOKEN_KIND_POUND]  = {NULL, NULL, REI_PREC_UNDEFINE, false}, // \#
    [REI_TOKEN_KIND_AT]     = {NULL, NULL, REI_PREC_UNDEFINE, false}, // @
    /* 字面量 */
    [REI_TOKEN_KIND_NIL]    = {literal_, NULL, REI_PREC_NONE, true},
    [REI_TOKEN_KIND_TRUE]   = {literal_, NULL, REI_PREC_NONE, true},
    [REI_TOKEN_KIND_FALSE]  = {literal_, NULL, REI_PREC_NONE, true},
    [REI_TOKEN_KIND_INT]    = {literal_, NULL, REI_PREC_NONE, true},
    [REI_TOKEN_KIND_FLOAT]  = {literal_, NULL, REI_PREC_NONE, true},
    [REI_TOKEN_KIND_STRING] = {literal_, NULL, REI_PREC_NONE, true},
    /* 标识符 */
    [REI_TOKEN_KIND_IDENTIFIER] = {identifier_, NULL, REI_PREC_NONE, true},
    /* 声明量 */
    [REI_TOKEN_KIND_MUT] = {NULL, NULL, REI_PREC_NONE, false},
    [REI_TOKEN_KIND_KON] = {NULL, NULL, REI_PREC_NONE, false},
    /* 静态区 */
    [REI_TOKEN_KIND_STATIC] = {NULL, NULL, REI_PREC_UNDEFINE, false},
    /* 条件语句 */
    [REI_TOKEN_KIND_IF]     = {NULL, NULL, REI_PREC_NONE, false},
    [REI_TOKEN_KIND_ELIF]   = {NULL, NULL, REI_PREC_NONE, false},
    [REI_TOKEN_KIND_ELSE]   = {NULL, NULL, REI_PREC_NONE, false},
    /* 循环语句 */
    [REI_TOKEN_KIND_WHILE]      = {NULL, NULL, REI_PREC_NONE, false},
    [REI_TOKEN_KIND_UNTIL]      = {NULL, NULL, REI_PREC_NONE, false},
    [REI_TOKEN_KIND_CONTINUE]   = {NULL, NULL, REI_PREC_NONE, false},
    [REI_TOKEN_KIND_BREAK]      = {NULL, NULL, REI_PREC_NONE, false},
    [REI_TOKEN_KIND_DO]         = {NULL, NULL, REI_PREC_NONE, false},
    /* 分支语句 */
    [REI_TOKEN_KIND_SWITCH]     = {NULL, NULL, REI_PREC_NONE, false},
    [REI_TOKEN_KIND_CASE]       = {NULL, NULL, REI_PREC_NONE, false},
    [REI_TOKEN_KIND_PASS]       = {NULL, NULL, REI_PREC_UNDEFINE, false},
    [REI_TOKEN_KIND_DEFAULT]    = {NULL, NULL, REI_PREC_NONE, false},
    /* 函数 */
    [REI_TOKEN_KIND_FUNC]   = {NULL, NULL, REI_PREC_NONE, false},
    [REI_TOKEN_KIND_RETURN] = {NULL, NULL, REI_PREC_NONE, false},
    /* 命名空间 */
    [REI_TOKEN_KIND_NAMESPACE]  = {NULL, NULL, REI_PREC_UNDEFINE, false},
    [REI_TOKEN_KIND_EXTERNAL]   = {NULL, NULL, REI_PREC_UNDEFINE, false},
    [REI_TOKEN_KIND_INTERNAL]   = {NULL, NULL, REI_PREC_UNDEFINE, false},
    /* 声明语句 */
    [REI_TOKEN_KIND_ENUM]       = {NULL, NULL, REI_PREC_UNDEFINE, false},
    [REI_TOKEN_KIND_FLAG]       = {NULL, NULL, REI_PREC_UNDEFINE, false},
    [REI_TOKEN_KIND_STRUCT]     = {NULL, NULL, REI_PREC_UNDEFINE, false},
    [REI_TOKEN_KIND_CLASS]      = {NULL, NULL, REI_PREC_NONE, false},
    [REI_TOKEN_KIND_INTERFACE]  = {NULL, NULL, REI_PREC_UNDEFINE, false},
    [REI_TOKEN_KIND_TRAIT]      = {NULL, NULL, REI_PREC_UNDEFINE, false},
    [REI_TOKEN_KIND_ATTRIBUTE]  = {NULL, NULL, REI_PREC_UNDEFINE, false},
    /* 类关键字 */
    [REI_TOKEN_KIND_PUBLIC]     = {NULL, NULL, REI_PREC_UNDEFINE, false},
    [REI_TOKEN_KIND_PROTECTED]  = {NULL, NULL, REI_PREC_UNDEFINE, false},
    [REI_TOKEN_KIND_PRIVATE]    = {NULL, NULL, REI_PREC_UNDEFINE, false},
    [REI_TOKEN_KIND_THIS]       = {NULL, NULL, REI_PREC_NONE, false},
    [REI_TOKEN_KIND_SUPER]      = {NULL, NULL, REI_PREC_NONE, false},
    [REI_TOKEN_KIND_ABSTRACT]   = {NULL, NULL, REI_PREC_UNDEFINE, false},
    [REI_TOKEN_KIND_SEALED]     = {NULL, NULL, REI_PREC_UNDEFINE, false},
    [REI_TOKEN_KIND_FINAL]      = {NULL, NULL, REI_PREC_UNDEFINE, false},
    [REI_TOKEN_KIND_VIRTUAL]    = {NULL, NULL, REI_PREC_UNDEFINE, false},
    [REI_TOKEN_KIND_OVERRIDE]   = {NULL, NULL, REI_PREC_UNDEFINE, false},
    [REI_TOKEN_KIND_UNDER_GET]  = {NULL, NULL, REI_PREC_UNDEFINE, false},
    [REI_TOKEN_KIND_UNDER_SET]  = {NULL, NULL, REI_PREC_UNDEFINE, false},
    [REI_TOKEN_KIND_NEW]        = {NULL, NULL, REI_PREC_NONE, false},
    [REI_TOKEN_KIND_DEL]        = {NULL, NULL, REI_PREC_NONE, false},
    [REI_TOKEN_KIND_IS]         = {NULL, NULL, REI_PREC_NONE, false},
    [REI_TOKEN_KIND_HAS]        = {NULL, NULL, REI_PREC_UNDEFINE, false},
    /* 模块关键字 */
    [REI_TOKEN_KIND_IMPORT] = {NULL, NULL, REI_PREC_UNDEFINE, false},
    [REI_TOKEN_KIND_AS]     = {NULL, NULL, REI_PREC_UNDEFINE, false},
    /* 重命名 */
    [REI_TOKEN_KIND_RENAME] = {NULL, NULL, REI_PREC_UNDEFINE, false},
    /* 魔法关键字 */
    [REI_TOKEN_KIND_DUNDER_MAIN] = {NULL, NULL, REI_PREC_UNDEFINE, false},
    [REI_TOKEN_KIND_DUNDER_INIT] = {NULL, NULL, REI_PREC_UNDEFINE, false},
    [REI_TOKEN_KIND_DUNDER_FREE] = {NULL, NULL, REI_PREC_UNDEFINE, false},
    [REI_TOKEN_KIND_DUNDER_OPER] = {NULL, NULL, REI_PREC_UNDEFINE, false},
    /* EOF */
    [REI_TOKEN_KIND_EOF] = {NULL, NULL, REI_PREC_NONE, true}
};

#endif
