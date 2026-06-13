#include "rei_token.h"
#include <string.h>
#include <inttypes.h>

#define REI_TOKEN_TYPE_NAME_WIDTH 24

static const char* tokenTypeNames_[REI_TOKEN_TYPE_MAX_COUNT] =
{
    "NIL", "TRUE", "FALSE",
    "INT", "FLOAT", "STRING",
    "IDENTIFIER",
    "LEFT_PAREN", "RIGHT_PAREN", "LEFT_BRACKET", "RIGHT_BRACKET", "LEFT_BRACE", "RIGHT_BRACE",
    "LEFT_BRACKET_BRACKET", "RIGHT_BRACKET_BRACKET",
    "COMMA", "QUESTION", "DOT", "DOT_DOT_DOT", "COLON", "COLON_COLON", "SEMICOLON",
    "PLUS", "MINUS", "STAR", "SLASH", "MODULO", "STAR_STAR",
    "AND", "PIPE", "WAVE", "LESS_LESS", "GREATER_GREATER",
    "AND_AND", "PIPE_PIPE", "BANG",
    "LESS", "GREATER", "COMPARE",
    "EQUAL", "EQUAL_EQUAL", "BANG_EQUAL", "LESS_EQUAL", "GREATER_EQUAL",
    "COLON_EQUAL",
    "PLUS_EQUAL", "MINUS_EQUAL", "STAR_EQUAL", "SLASH_EQUAL", "MODULO_EQUAL", "STAR_STAR_EQUAL",
    "AND_EQUAL", "PIPE_EQUAL", "WAVE_EQUAL", "LESS_LESS_EQUAL", "GREATER_GREATER_EQUAL",
    "LEFT_ARROW", "RIGHT_ARROW",
    "POUND", "AT",
    "CONST", "STATIC",
    "IF", "ELIF", "ELSE",
    "WHILE", "UNTIL", "CONTINUE", "BREAK",
    "DO",
    "SWITCH", "CASE", "PASS", "DEFAULT",
    "FUNC", "RETURN",
    "NAMESPACE", "EXTERNAL", "INTERNAL",
    "ENUM", "FLAG",
    "STRUCT", "CLASS", "INTERFACE", "TRAIT", "ATTRIBUTE",
    "PUBLIC", "PROTECTED", "PRIVATE",
    "SUPER", "THIS",
    "ABSTRACT", "SEALED", "FINAL",
    "VIRTUAL", "OVERRIDE",
    "NEW", "DEL",
    "IS", "HAS",
    "IMPORT", "AS",
    "RENAME",
    "__MAIN__",
    "__INIT__", "__FREE__", "__CALL__", "__OPER__",
    "EOF"
};

const char* reiTokenTypeToCstr(ReiTokenType type)
{
    return tokenTypeNames_[type];
}
char* reiTokenToCstr(const ReiToken* me, char* buff, size_t len)
{
    const char* typeName = tokenTypeNames_[me->type];
    switch (me->type)
    {
    case REI_TOKEN_TYPE_NIL:
    case REI_TOKEN_TYPE_TRUE:
    case REI_TOKEN_TYPE_FALSE:
        snprintf(
            buff, len,
            "%-*s%.*s",
            REI_TOKEN_TYPE_NAME_WIDTH, typeName,
            me->lexeme.length, me->lexeme.start
        );
        break;
    case REI_TOKEN_TYPE_INT:
        snprintf(
            buff, len,
            "%-*s%" PRId16,
            REI_TOKEN_TYPE_NAME_WIDTH, typeName,
            REI_AS_INT(me->literal)
        );
        break;
    case REI_TOKEN_TYPE_FLOAT:
        snprintf(
            buff, len,
            "%-*s%g",
            REI_TOKEN_TYPE_NAME_WIDTH, typeName,
            REI_AS_FLOAT(me->literal)
        );
        break;
    case REI_TOKEN_TYPE_STRING:
    case REI_TOKEN_TYPE_IDENTIFIER:
    case REI_TOKEN_TYPE_LEFT_PAREN:
    case REI_TOKEN_TYPE_RIGHT_PAREN:
    case REI_TOKEN_TYPE_LEFT_BRACKET:
    case REI_TOKEN_TYPE_RIGHT_BRACKET:
    case REI_TOKEN_TYPE_LEFT_BRACE:
    case REI_TOKEN_TYPE_RIGHT_BRACE:
    case REI_TOKEN_TYPE_LEFT_BRACKET_BRACKET:
    case REI_TOKEN_TYPE_RIGHT_BRACKET_BRACKET:
    case REI_TOKEN_TYPE_COMMA:
    case REI_TOKEN_TYPE_QUESTION:
    case REI_TOKEN_TYPE_DOT:
    case REI_TOKEN_TYPE_DOT_DOT_DOT:
    case REI_TOKEN_TYPE_COLON:
    case REI_TOKEN_TYPE_COLON_COLON:
    case REI_TOKEN_TYPE_SEMICOLON:
    case REI_TOKEN_TYPE_PLUS:
    case REI_TOKEN_TYPE_MINUS:
    case REI_TOKEN_TYPE_STAR:
    case REI_TOKEN_TYPE_SLASH:
    case REI_TOKEN_TYPE_MODULO:
    case REI_TOKEN_TYPE_STAR_STAR:
    case REI_TOKEN_TYPE_AND:
    case REI_TOKEN_TYPE_PIPE:
    case REI_TOKEN_TYPE_WAVE:
    case REI_TOKEN_TYPE_LESS_LESS:
    case REI_TOKEN_TYPE_GREATER_GREATER:
    case REI_TOKEN_TYPE_AND_AND:
    case REI_TOKEN_TYPE_PIPE_PIPE:
    case REI_TOKEN_TYPE_BANG:
    case REI_TOKEN_TYPE_LESS:
    case REI_TOKEN_TYPE_GREATER:
    case REI_TOKEN_TYPE_COMPARE:
    case REI_TOKEN_TYPE_EQUAL:
    case REI_TOKEN_TYPE_EQUAL_EQUAL:
    case REI_TOKEN_TYPE_BANG_EQUAL:
    case REI_TOKEN_TYPE_LESS_EQUAL:
    case REI_TOKEN_TYPE_GREATER_EQUAL:
    case REI_TOKEN_TYPE_COLON_EQUAL:
    case REI_TOKEN_TYPE_PLUS_EQUAL:
    case REI_TOKEN_TYPE_MINUS_EQUAL:
    case REI_TOKEN_TYPE_STAR_EQUAL:
    case REI_TOKEN_TYPE_SLASH_EQUAL:
    case REI_TOKEN_TYPE_MODULO_EQUAL:
    case REI_TOKEN_TYPE_STAR_STAR_EQUAL:
    case REI_TOKEN_TYPE_AND_EQUAL:
    case REI_TOKEN_TYPE_PIPE_EQUAL:
    case REI_TOKEN_TYPE_WAVE_EQUAL:
    case REI_TOKEN_TYPE_LESS_LESS_EQUAL:
    case REI_TOKEN_TYPE_GREATER_GREATER_EQUAL:
    case REI_TOKEN_TYPE_LEFT_ARROW:
    case REI_TOKEN_TYPE_RIGHT_ARROW:
    case REI_TOKEN_TYPE_POUND:
    case REI_TOKEN_TYPE_AT:
    case REI_TOKEN_TYPE_CONST:
    case REI_TOKEN_TYPE_STATIC:
    case REI_TOKEN_TYPE_IF:
    case REI_TOKEN_TYPE_ELIF:
    case REI_TOKEN_TYPE_ELSE:
    case REI_TOKEN_TYPE_WHILE:
    case REI_TOKEN_TYPE_UNTIL:
    case REI_TOKEN_TYPE_CONTINUE:
    case REI_TOKEN_TYPE_BREAK:
    case REI_TOKEN_TYPE_DO:
    case REI_TOKEN_TYPE_SWITCH:
    case REI_TOKEN_TYPE_CASE:
    case REI_TOKEN_TYPE_PASS:
    case REI_TOKEN_TYPE_DEFAULT:
    case REI_TOKEN_TYPE_FUNC:
    case REI_TOKEN_TYPE_RETURN:
    case REI_TOKEN_TYPE_NAMESPACE:
    case REI_TOKEN_TYPE_EXTERNAL:
    case REI_TOKEN_TYPE_INTERNAL:
    case REI_TOKEN_TYPE_ENUM:
    case REI_TOKEN_TYPE_FLAG:
    case REI_TOKEN_TYPE_STRUCT:
    case REI_TOKEN_TYPE_CLASS:
    case REI_TOKEN_TYPE_INTERFACE:
    case REI_TOKEN_TYPE_TRAIT:
    case REI_TOKEN_TYPE_ATTRIBUTE:
    case REI_TOKEN_TYPE_PUBLIC:
    case REI_TOKEN_TYPE_PROTECTED:
    case REI_TOKEN_TYPE_PRIVATE:
    case REI_TOKEN_TYPE_SUPER:
    case REI_TOKEN_TYPE_THIS:
    case REI_TOKEN_TYPE_ABSTRACT:
    case REI_TOKEN_TYPE_SEALED:
    case REI_TOKEN_TYPE_FINAL:
    case REI_TOKEN_TYPE_VIRTUAL:
    case REI_TOKEN_TYPE_OVERRIDE:
    case REI_TOKEN_TYPE_NEW:
    case REI_TOKEN_TYPE_DEL:
    case REI_TOKEN_TYPE_IS:
    case REI_TOKEN_TYPE_HAS:
    case REI_TOKEN_TYPE_IMPORT:
    case REI_TOKEN_TYPE_AS:
    case REI_TOKEN_TYPE_RENAME:
    case REI_TOKEN_TYPE_DUNDER_MAIN:
    case REI_TOKEN_TYPE_DUNDER_INIT:
    case REI_TOKEN_TYPE_DUNDER_FREE:
    case REI_TOKEN_TYPE_DUNDER_CALL:
    case REI_TOKEN_TYPE_DUNDER_OPER:
    case REI_TOKEN_TYPE_EOF:
        snprintf(
            buff, len,
            "%-*s%.*s",
            REI_TOKEN_TYPE_NAME_WIDTH, typeName,
            me->lexeme.length, me->lexeme.start
        );
        break;
    }
    return buff;
}
