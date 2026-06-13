#include "rei_token.h"
#include <string.h>
#include <inttypes.h>

#define REI_TOKEN_KIND_NAME_WIDTH 24

static const char* tokenKindNames_[REI_TOKEN_KIND_MAX_COUNT] =
{
    "LEFT_PAREN", "RIGHT_PAREN", "LEFT_BRACKET", "RIGHT_BRACKET", "LEFT_BRACE", "RIGHT_BRACE",
    "LEFT_BRACKET_BRACKET", "RIGHT_BRACKET_BRACKET",
    "COMMA", "QUESTION", "DOT", "DOT_DOT", "DOT_DOT_DOT", "COLON", "COLON_COLON", "SEMICOLON",
    "PLUS", "MINUS", "STAR", "SLASH", "MODULO", "STAR_STAR",
    "AND", "PIPE", "CTRL", "WAVE", "LESS_LESS", "GREATER_GREATER",
    "AND_AND", "PIPE_PIPE", "BANG",
    "LESS", "GREATER", "COMPARE",
    "EQUAL", "EQUAL_EQUAL", "BANG_EQUAL", "LESS_EQUAL", "GREATER_EQUAL",
    "COLON_EQUAL",
    "PLUS_EQUAL", "MINUS_EQUAL", "STAR_EQUAL", "SLASH_EQUAL", "MODULO_EQUAL", "STAR_STAR_EQUAL",
    "AND_EQUAL", "PIPE_EQUAL", "WAVE_EQUAL", "LESS_LESS_EQUAL", "GREATER_GREATER_EQUAL",
    "LEFT_ARROW", "RIGHT_ARROW",
    "POUND", "AT",
    "NIL", "TRUE", "FALSE",
    "INT", "FLOAT", "STRING",
    "IDENTIFIER",
    "CONST", "STATIC",
    "IF", "ELIF", "ELSE",
    "WHILE", "UNTIL", "CONTINUE", "BREAK", "DO",
    "SWITCH", "CASE", "PASS", "DEFAULT",
    "FUNC", "RETURN",
    "NAMESPACE", "EXTERNAL", "INTERNAL",
    "ENUM", "FLAG",
    "STRUCT", "CLASS", "INTERFACE", "TRAIT", "ATTRIBUTE",
    "PUBLIC", "PROTECTED", "PRIVATE",
    "SUPER", "THIS",
    "ABSTRACT", "SEALED", "FINAL",
    "VIRTUAL", "OVERRIDE",
    "_GET_", "_SET_",
    "NEW", "DEL",
    "IS", "HAS",
    "IMPORT", "AS",
    "RENAME",
    "__MAIN__",
    "__INIT__", "__FREE__", "__ATIX__", "__CALL__", "__OPER__",
    "EOF"
};

const char* reiTokenKindToCstr(ReiTokenKind kind)
{
    return tokenKindNames_[kind];
}
char* reiTokenToCstr(const ReiToken* me, char* buff, size_t len)
{
    const char* kindName = tokenKindNames_[me->kind];
    switch (me->kind)
    {
    case REI_TOKEN_KIND_INT:
        snprintf(
            buff, len,
            "%-*s%" PRId16,
            REI_TOKEN_KIND_NAME_WIDTH, kindName,
            REI_AS_INT(me->literal)
        );
        break;
    case REI_TOKEN_KIND_FLOAT:
        snprintf(
            buff, len,
            "%-*s%g",
            REI_TOKEN_KIND_NAME_WIDTH, kindName,
            REI_AS_FLOAT(me->literal)
        );
        break;
    default:
        snprintf(
            buff, len,
            "%-*s%.*s",
            REI_TOKEN_KIND_NAME_WIDTH, kindName,
            me->lexeme.length, me->lexeme.start
        );
        break;
    }
    return buff;
}
