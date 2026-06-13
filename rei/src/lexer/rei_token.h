#ifndef REI_LEXER_TOKEN_H
#define REI_LEXER_TOKEN_H

#include "../type/rei_value.h"

typedef enum ReiTokenType
{
    REI_TOKEN_TYPE_NIL,     // NIL
    REI_TOKEN_TYPE_TRUE,    // TRUE
    REI_TOKEN_TYPE_FALSE,   // FALSE
    REI_TOKEN_TYPE_INT,     // INT
    REI_TOKEN_TYPE_FLOAT,   // FLOAT
    REI_TOKEN_TYPE_STRING,  // STRING

    REI_TOKEN_TYPE_IDENTIFIER,

    REI_TOKEN_TYPE_LEFT_PAREN,              // (
    REI_TOKEN_TYPE_RIGHT_PAREN,             // )
    REI_TOKEN_TYPE_LEFT_BRACKET,            // [
    REI_TOKEN_TYPE_RIGHT_BRACKET,           // ]
    REI_TOKEN_TYPE_LEFT_BRACE,              // {
    REI_TOKEN_TYPE_RIGHT_BRACE,             // }

    REI_TOKEN_TYPE_LEFT_BRACKET_BRACKET,    // [[
    REI_TOKEN_TYPE_RIGHT_BRACKET_BRACKET,   // ]]

    REI_TOKEN_TYPE_COMMA,       // ,
    REI_TOKEN_TYPE_QUESTION,    // ?
    REI_TOKEN_TYPE_DOT,         // .
    REI_TOKEN_TYPE_DOT_DOT_DOT, // ...
    REI_TOKEN_TYPE_COLON,       // :
    REI_TOKEN_TYPE_COLON_COLON,  // ::
    REI_TOKEN_TYPE_SEMICOLON,   // ;
    
    REI_TOKEN_TYPE_PLUS,        // +
    REI_TOKEN_TYPE_MINUS,       // -
    REI_TOKEN_TYPE_STAR,        // *
    REI_TOKEN_TYPE_SLASH,       // /
    REI_TOKEN_TYPE_MODULO,      // %
    REI_TOKEN_TYPE_STAR_STAR,   // **

    REI_TOKEN_TYPE_AND,             // &
    REI_TOKEN_TYPE_PIPE,            // |
    REI_TOKEN_TYPE_WAVE,            // ~
    REI_TOKEN_TYPE_LESS_LESS,       // <<
    REI_TOKEN_TYPE_GREATER_GREATER, // >>

    REI_TOKEN_TYPE_AND_AND,         // &&
    REI_TOKEN_TYPE_PIPE_PIPE,       // ||
    REI_TOKEN_TYPE_BANG,            // !
    
    REI_TOKEN_TYPE_LESS,                    // <
    REI_TOKEN_TYPE_GREATER,                 // >
    REI_TOKEN_TYPE_COMPARE,                 // <=>
    REI_TOKEN_TYPE_EQUAL,                   // =
    REI_TOKEN_TYPE_EQUAL_EQUAL,             // ==
    REI_TOKEN_TYPE_BANG_EQUAL,              // !=
    REI_TOKEN_TYPE_LESS_EQUAL,              // <=
    REI_TOKEN_TYPE_GREATER_EQUAL,           // >=

    REI_TOKEN_TYPE_COLON_EQUAL,             // :=

    REI_TOKEN_TYPE_PLUS_EQUAL,              // +=
    REI_TOKEN_TYPE_MINUS_EQUAL,             // -=
    REI_TOKEN_TYPE_STAR_EQUAL,              // *=
    REI_TOKEN_TYPE_SLASH_EQUAL,             // /=
    REI_TOKEN_TYPE_MODULO_EQUAL,            // %=
    REI_TOKEN_TYPE_STAR_STAR_EQUAL,         // **=
    REI_TOKEN_TYPE_AND_EQUAL,               // &=
    REI_TOKEN_TYPE_PIPE_EQUAL,              // |=
    REI_TOKEN_TYPE_WAVE_EQUAL,              // ~=
    REI_TOKEN_TYPE_LESS_LESS_EQUAL,         // <<=
    REI_TOKEN_TYPE_GREATER_GREATER_EQUAL,   // >>=

    REI_TOKEN_TYPE_LEFT_ARROW,  // <-
    REI_TOKEN_TYPE_RIGHT_ARROW, // ->

    REI_TOKEN_TYPE_POUND,   // \#
    REI_TOKEN_TYPE_AT,      // @

    REI_TOKEN_TYPE_CONST,
    REI_TOKEN_TYPE_STATIC,

    REI_TOKEN_TYPE_IF,
    REI_TOKEN_TYPE_ELIF,
    REI_TOKEN_TYPE_ELSE,

    REI_TOKEN_TYPE_WHILE,
    REI_TOKEN_TYPE_UNTIL,
    REI_TOKEN_TYPE_CONTINUE,
    REI_TOKEN_TYPE_BREAK,

    REI_TOKEN_TYPE_DO,

    REI_TOKEN_TYPE_SWITCH,
    REI_TOKEN_TYPE_CASE,
    REI_TOKEN_TYPE_PASS,
    REI_TOKEN_TYPE_DEFAULT,

    REI_TOKEN_TYPE_FUNC,
    REI_TOKEN_TYPE_RETURN,
    
    REI_TOKEN_TYPE_NAMESPACE,
    REI_TOKEN_TYPE_EXTERNAL,
    REI_TOKEN_TYPE_INTERNAL,

    REI_TOKEN_TYPE_ENUM,
    REI_TOKEN_TYPE_FLAG,
    REI_TOKEN_TYPE_STRUCT,
    REI_TOKEN_TYPE_CLASS,
    REI_TOKEN_TYPE_INTERFACE,
    REI_TOKEN_TYPE_TRAIT,
    REI_TOKEN_TYPE_ATTRIBUTE,

    REI_TOKEN_TYPE_PUBLIC,
    REI_TOKEN_TYPE_PROTECTED,
    REI_TOKEN_TYPE_PRIVATE,
    REI_TOKEN_TYPE_SUPER,
    REI_TOKEN_TYPE_THIS,
    REI_TOKEN_TYPE_ABSTRACT,
    REI_TOKEN_TYPE_SEALED,
    REI_TOKEN_TYPE_FINAL,
    REI_TOKEN_TYPE_VIRTUAL,
    REI_TOKEN_TYPE_OVERRIDE,
    REI_TOKEN_TYPE_NEW,
    REI_TOKEN_TYPE_DEL,
    REI_TOKEN_TYPE_IS,
    REI_TOKEN_TYPE_HAS,

    REI_TOKEN_TYPE_IMPORT,
    REI_TOKEN_TYPE_AS,

    REI_TOKEN_TYPE_RENAME,

    REI_TOKEN_TYPE_DUNDER_MAIN,

    REI_TOKEN_TYPE_DUNDER_INIT,
    REI_TOKEN_TYPE_DUNDER_FREE,
    REI_TOKEN_TYPE_DUNDER_CALL,
    REI_TOKEN_TYPE_DUNDER_OPER,

    REI_TOKEN_TYPE_EOF,

    REI_TOKEN_TYPE_MAX_COUNT
} ReiTokenType;
typedef struct ReiToken
{
    ReiTokenType type;
    struct
    {
    const char* start;
    int length;
    } lexeme;
    ReiValue literal;
    uint32_t line;
} ReiToken;

C_TEMPLATE_DECL_VECTOR(, rei, Rei, TokenBuffer, ReiToken)
C_TEMPLATE_DEFN_VECTOR(, rei, Rei, TokenBuffer, ReiToken)

#include "rei_token.debug.i"

#endif
