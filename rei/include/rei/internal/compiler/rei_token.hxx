#pragma once
#include <string_view>
#include <cstdint>

namespace rei
{
namespace TokenKind
{
enum E
{
    LEFT_PAREN,     // (
    RIGHT_PAREN,    // )
    LEFT_BRACKET,   // [
    RIGHT_BRACKET,  // ]
    LEFT_BRACE,     // {
    RIGHT_BRACE,    // }

    LEFT_BRACKET_BRACKET,   // [[
    RIGHT_BRACKET_BRACKET,  // ]]

    COMMA,          // ,
    QUESTION,       // ?
    DOT,            // .
    DOT_DOT,        // ..
    DOT_DOT_DOT,    // ...
    COLON,          // :
    COLON_COLON,    // ::
    SEMICOLON,      // ;
    
    PLUS,   // +
    MINUS,  // -
    STAR,   // *
    SLASH,  // /
    MODULO, // %

    PLUS_PLUS,      // ++
    MINUS_MINUS,    // --

    AND,                // &
    PIPE,               // |
    CARET,              // ^
    TILDE,              // ~
    LESS_LESS,          // <<
    GREATER_GREATER,    // >>

    AND_AND,    // &&
    PIPE_PIPE,  // ||
    BANG,       // !
    
    LESS,           // <
    GREATER,        // >
    EQUAL_EQUAL,    // ==
    BANG_EQUAL,     // !=
    LESS_EQUAL,     // <=
    GREATER_EQUAL,  // >=
    COMPARE,        // <=>

    WALRUS,  // :=
    EQUAL,   // =

    PLUS_EQUAL,     // +=
    MINUS_EQUAL,    // -=
    STAR_EQUAL,     // *=
    SLASH_EQUAL,    // /=
    MODULO_EQUAL,   // %=

    AND_EQUAL,              // &=
    PIPE_EQUAL,             // |=
    CARET_EQUAL,            // ^=
    TILDE_EQUAL,            // ~=
    LESS_LESS_EQUAL,        // <<=
    GREATER_GREATER_EQUAL,  // >>=

    LEFT_ARROW,     // <-
    RIGHT_ARROW,    // ->

    POUND,  // \#
    AT,     // @

    VOID,      // void
    CHAR,      // char     8
    SHORT,     // short    16
    INT,       // int      32
    LONG,      // long     64
    FLOAT,     // float
    DOUBLE,    // double

    SIGNED,    // signed
    UNSIGNED,  // unsigned

    ALIGNAS, // alignas()

    LIT_INT,
    LIT_FLOAT,
    LIT_STRING,

    IDENTIFIER,

    KON, // kon
    MUT, // mut

    IF,     // if
    ELIF,   // elif
    ELSE,   // else

    FOR,        // for
    WHILE,      // while
    DO,         // do
    CONTINUE,   // continue
    BREAK,      // break

    SWITCH,     // switch
    CASE,       // case
    DEFAULT,    // default

    FN,     // fn
    RETURN, // return

    ENUM,   // enum
    STRUCT, // struct

    TYPEDEF, // typedef

    SIZEOF,     // sizeof()
    ALIGNOF,    // alignof()
    OFFSETOF,   // offsetof()
    TYPEOF,     // typeof()
    
    LIT_EOF
};
}
struct Token final
{
    TokenKind::E kind;
    std::string_view lexeme;
    union
    {
        uint64_t i;
        double f;
        bool b;
    } literal;
    uint32_t line;
};
}
static inline const char* string_ReiTokenKind(rei::TokenKind::E kind)
{
    using namespace rei::TokenKind;
    switch (kind)
    {
        case LEFT_PAREN:    return "LEFT_PAREN";
        case RIGHT_PAREN:   return "RIGHT_PAREN";
        case LEFT_BRACKET:  return "LEFT_BRACKET";
        case RIGHT_BRACKET: return "RIGHT_BRACKET";
        case LEFT_BRACE:    return "LEFT_BRACE";
        case RIGHT_BRACE:   return "RIGHT_BRACE";
        case LEFT_BRACKET_BRACKET:  return "LEFT_BRACKET_BRACKET";
        case RIGHT_BRACKET_BRACKET: return "RIGHT_BRACKET_BRACKET";
        case COMMA:         return "COMMA";
        case QUESTION:      return "QUESTION";
        case DOT:           return "DOT";
        case DOT_DOT:       return "DOT_DOT";
        case DOT_DOT_DOT:   return "DOT_DOT_DOT";
        case COLON:         return "COLON";
        case COLON_COLON:   return "COLON_COLON";
        case SEMICOLON:     return "SEMICOLON";
        case PLUS:      return "PLUS";
        case MINUS:     return "MINUS";
        case STAR:      return "STAR";
        case SLASH:     return "SLASH";
        case MODULO:    return "MODULO";
        case AND:   return "AND";
        case PIPE:  return "PIPE";
        case CARET: return "CARET";
        case TILDE: return "TILDE";
        case LESS_LESS:         return "LESS_LESS";
        case GREATER_GREATER:   return "GREATER_GREATER";
        case AND_AND:   return "AND_AND";
        case PIPE_PIPE: return "PIPE_PIPE";
        case BANG:      return "BANG";
        case LESS:          return "LESS";
        case GREATER:       return "GREATER";
        case EQUAL_EQUAL:   return "EQUAL_EQUAL";
        case BANG_EQUAL:    return "BANG_EQUAL";
        case LESS_EQUAL:    return "LESS_EQUAL";
        case GREATER_EQUAL: return "GREATER_EQUAL";
        case COMPARE:       return "COMPARE";
        case WALRUS:    return "WALRUS";
        case EQUAL:     return "EQUAL";
        case PLUS_EQUAL:    return "PLUS_EQUAL";
        case MINUS_EQUAL:   return "MINUS_EQUAL";
        case STAR_EQUAL:    return "STAR_EQUAL";
        case SLASH_EQUAL:   return "SLASH_EQUAL";
        case MODULO_EQUAL:  return "MODULO_EQUAL";
        case AND_EQUAL:             return "AND_EQUAL";
        case PIPE_EQUAL:            return "PIPE_EQUAL";
        case CARET_EQUAL:           return "CARET_EQUAL";
        case TILDE_EQUAL:           return "TILDE_EQUAL";
        case LESS_LESS_EQUAL:       return "LESS_LESS_EQUAL";
        case GREATER_GREATER_EQUAL: return "GREATER_GREATER_EQUAL";
        case LEFT_ARROW:    return "LEFT_ARROW";
        case RIGHT_ARROW:   return "RIGHT_ARROW";
        case POUND: return "POUND";
        case AT:    return "AT";
        case VOID:      return "VOID";
        case CHAR:      return "CHAR";
        case SHORT:     return "SHORT";
        case INT:       return "INT";
        case LONG:      return "LONG";
        case FLOAT:     return "FLOAT";
        case DOUBLE:    return "DOUBLE";
        case SIGNED:    return "SIGNED";
        case UNSIGNED:  return "UNSIGNED";
        case ALIGNAS: return "ALIGNAS";
        case LIT_INT:       return "LIT_INT";
        case LIT_FLOAT:     return "LIT_FLOAT";
        case LIT_STRING:    return "LIT_STRING";
        case IDENTIFIER: return "IDENTIFIER";
        case KON: return "KON";
        case MUT: return "MUT";
        case IF:    return "IF";
        case ELIF:  return "ELIF";
        case ELSE:  return "ELSE";
        case FOR:       return "FOR";
        case WHILE:     return "WHILE";
        case DO:        return "DO";
        case CONTINUE:  return "CONTINUE";
        case BREAK:     return "BREAK";
        case SWITCH:    return "SWITCH";
        case CASE:      return "CASE";
        case DEFAULT:   return "DEFAULT";
        case FN:        return "FN";
        case RETURN:    return "RETURN";
        case ENUM:      return "ENUM";
        case STRUCT:    return "STRUCT";
        case TYPEDEF: return "TYPEDEF";
        case SIZEOF:    return "SIZEOF";
        case ALIGNOF:   return "ALIGNOF";
        case OFFSETOF:  return "OFFSETOF";
        case TYPEOF:    return "TYPEOF";
        case LIT_EOF: return "LIT_EOF";
        default: return "UNKNOWN_REI_TOKEN_KIND";
    }
}
