#pragma once
#include <cstdint>
#include <string_view>
#include <string>

namespace rei
{
enum TokenKind
{
    TK_LEFT_PAREN,      // (
    TK_RIGHT_PAREN,     // )
    TK_LEFT_BRACKET,    // [
    TK_RIGHT_BRACKET,   // ]
    TK_LEFT_BRACE,      // {
    TK_RIGHT_BRACE,     // }

    TK_LEFT_BRACKET_BRACKET,    // [[ (暂无作用)
    TK_RIGHT_BRACKET_BRACKET,   // ]] (暂无作用)

    TK_COMMA,       // ,
    TK_QUESTION,    // ?
    TK_DOT,         // .
    TK_DOT_DOT,     // .. (暂无作用)
    TK_DOT_DOT_DOT, // ... (暂无作用)
    TK_COLON,       // :
    TK_COLON_COLON, // :: (暂无作用)
    TK_SEMICOLON,   // ;
    
    TK_PLUS,    // +
    TK_MINUS,   // -
    TK_STAR,    // *
    TK_SLASH,   // /
    TK_MODULO,  // %

    TK_PLUS_PLUS,   // ++
    TK_MINUS_MINUS, // --

    TK_AND,             // &
    TK_PIPE,            // |
    TK_CARET,           // ^
    TK_TILDE,           // ~
    TK_LESS_LESS,       // <<
    TK_GREATER_GREATER, // >>

    TK_AND_AND,     // &&
    TK_PIPE_PIPE,   // ||
    TK_BANG,        // !
    
    TK_LESS,            // <
    TK_GREATER,         // >
    TK_EQUAL_EQUAL,     // ==
    TK_BANG_EQUAL,      // !=
    TK_LESS_EQUAL,      // <=
    TK_GREATER_EQUAL,   // >=
    TK_COMPARE,         // <=>

    TK_WALRUS,  // := (暂无作用)
    TK_EQUAL,   // =

    TK_PLUS_EQUAL,      // +=
    TK_MINUS_EQUAL,     // -=
    TK_STAR_EQUAL,      // *=
    TK_SLASH_EQUAL,     // /=
    TK_MODULO_EQUAL,    // %=

    TK_AND_EQUAL,               // &=
    TK_PIPE_EQUAL,              // |=
    TK_CARET_EQUAL,             // ^=
    TK_TILDE_EQUAL,             // ~=
    TK_LESS_LESS_EQUAL,         // <<=
    TK_GREATER_GREATER_EQUAL,   // >>=

    TK_LEFT_ARROW,  // <- (暂无作用)
    TK_RIGHT_ARROW, // ->

    TK_POUND,   // \# (暂无作用)
    TK_AT,      // @ (暂无作用)

    TK_LIT_INT,
    TK_LIT_FLOAT,
    TK_LIT_STRING,

    TK_IDENTIFIER,

    TK_VOID,    // void
    TK_CHAR,    // char     8
    TK_SHORT,   // short    16
    TK_INT,     // int      32
    TK_LONG,    // long     64
    TK_FLOAT,   // float
    TK_DOUBLE,  // double

    TK_SIGNED,      // signed
    TK_UNSIGNED,    // unsigned

    TK_KON, // kon
    TK_MUT, // mut (暂无作用)

    TK_IF,      // if
    TK_ELIF,    // elif
    TK_ELSE,    // else

    TK_FOR,         // for
    TK_WHILE,       // while
    TK_DO,          // do
    TK_CONTINUE,    // continue
    TK_BREAK,       // break

    TK_SWITCH,  // switch
    TK_CASE,    // case
    TK_DEFAULT, // default

    TK_FN,      // fn
    TK_RETURN,  // return

    TK_ENUM,    // enum
    TK_STRUCT,  // struct
    TK_ALIGNAS, // alignas
    TK_TYPEDEF, // typedef

    TK_SIZEOF,      // sizeof
    TK_ALIGNOF,     // alignof
    TK_OFFSETOF,    // offsetof
    TK_TYPEOF,      // typeof
    
    TK_EOF
};
struct Token final
{
    TokenKind kind;
    std::string_view lexeme;
    union
    {
        uint64_t i;
        double f;
    } literal;
    std::string str;
    uint32_t line;
};
auto keywordToTokenKind(std::string_view keyword) -> TokenKind;
}
static inline const char* string_ReiTokenKind(rei::TokenKind kind)
{
    using namespace rei;
    switch (kind)
    {
        case TK_LEFT_PAREN:     return "TK_LEFT_PAREN";
        case TK_RIGHT_PAREN:    return "TK_RIGHT_PAREN";
        case TK_LEFT_BRACKET:   return "TK_LEFT_BRACKET";
        case TK_RIGHT_BRACKET:  return "TK_RIGHT_BRACKET";
        case TK_LEFT_BRACE:     return "TK_LEFT_BRACE";
        case TK_RIGHT_BRACE:    return "TK_RIGHT_BRACE";
        case TK_LEFT_BRACKET_BRACKET:   return "TK_LEFT_BRACKET_BRACKET";
        case TK_RIGHT_BRACKET_BRACKET:  return "TK_RIGHT_BRACKET_BRACKET";
        case TK_COMMA:          return "TK_COMMA";
        case TK_QUESTION:       return "TK_QUESTION";
        case TK_DOT:            return "TK_DOT";
        case TK_DOT_DOT:        return "TK_DOT_DOT";
        case TK_DOT_DOT_DOT:    return "TK_DOT_DOT_DOT";
        case TK_COLON:          return "TK_COLON";
        case TK_COLON_COLON:    return "TK_COLON_COLON";
        case TK_SEMICOLON:      return "TK_SEMICOLON";
        case TK_PLUS:   return "TK_PLUS";
        case TK_MINUS:  return "TK_MINUS";
        case TK_STAR:   return "TK_STAR";
        case TK_SLASH:  return "TK_SLASH";
        case TK_MODULO: return "TK_MODULO";
        case TK_AND:    return "TK_AND";
        case TK_PIPE:   return "TK_PIPE";
        case TK_CARET:  return "TK_CARET";
        case TK_TILDE:  return "TK_TILDE";
        case TK_LESS_LESS:          return "TK_LESS_LESS";
        case TK_GREATER_GREATER:    return "TK_GREATER_GREATER";
        case TK_AND_AND:    return "TK_AND_AND";
        case TK_PIPE_PIPE:  return "TK_PIPE_PIPE";
        case TK_BANG:       return "TK_BANG";
        case TK_LESS:           return "TK_LESS";
        case TK_GREATER:        return "TK_GREATER";
        case TK_EQUAL_EQUAL:    return "TK_EQUAL_EQUAL";
        case TK_BANG_EQUAL:     return "TK_BANG_EQUAL";
        case TK_LESS_EQUAL:     return "TK_LESS_EQUAL";
        case TK_GREATER_EQUAL:  return "TK_GREATER_EQUAL";
        case TK_COMPARE:        return "TK_COMPARE";
        case TK_WALRUS: return "TK_WALRUS";
        case TK_EQUAL:  return "TK_EQUAL";
        case TK_PLUS_EQUAL:     return "TK_PLUS_EQUAL";
        case TK_MINUS_EQUAL:    return "TK_MINUS_EQUAL";
        case TK_STAR_EQUAL:     return "TK_STAR_EQUAL";
        case TK_SLASH_EQUAL:    return "TK_SLASH_EQUAL";
        case TK_MODULO_EQUAL:   return "TK_MODULO_EQUAL";
        case TK_AND_EQUAL:              return "TK_AND_EQUAL";
        case TK_PIPE_EQUAL:             return "TK_PIPE_EQUAL";
        case TK_CARET_EQUAL:            return "TK_CARET_EQUAL";
        case TK_TILDE_EQUAL:            return "TK_TILDE_EQUAL";
        case TK_LESS_LESS_EQUAL:        return "TK_LESS_LESS_EQUAL";
        case TK_GREATER_GREATER_EQUAL:  return "TK_GREATER_GREATER_EQUAL";
        case TK_LEFT_ARROW:     return "TK_LEFT_ARROW";
        case TK_RIGHT_ARROW:    return "TK_RIGHT_ARROW";
        case TK_POUND:  return "TK_POUND";
        case TK_AT:     return "TK_AT";
        case TK_LIT_INT:    return "TK_LIT_INT";
        case TK_LIT_FLOAT:  return "TK_LIT_FLOAT";
        case TK_LIT_STRING: return "TK_LIT_STRING";
        case TK_IDENTIFIER: return "TK_IDENTIFIER";
        case TK_VOID:   return "TK_VOID";
        case TK_CHAR:   return "TK_CHAR";
        case TK_SHORT:  return "TK_SHORT";
        case TK_INT:    return "TK_INT";
        case TK_LONG:   return "TK_LONG";
        case TK_FLOAT:  return "TK_FLOAT";
        case TK_DOUBLE: return "TK_DOUBLE";
        case TK_SIGNED:     return "TK_SIGNED";
        case TK_UNSIGNED:   return "TK_UNSIGNED";
        case TK_KON: return "TK_KON";
        case TK_MUT: return "TK_MUT";
        case TK_IF:     return "TK_IF";
        case TK_ELIF:   return "TK_ELIF";
        case TK_ELSE:   return "TK_ELSE";
        case TK_FOR:        return "TK_FOR";
        case TK_WHILE:      return "TK_WHILE";
        case TK_DO:         return "TK_DO";
        case TK_CONTINUE:   return "TK_CONTINUE";
        case TK_BREAK:      return "TK_BREAK";
        case TK_SWITCH:     return "TK_SWITCH";
        case TK_CASE:       return "TK_CASE";
        case TK_DEFAULT:    return "TK_DEFAULT";
        case TK_FN:     return "TK_FN";
        case TK_RETURN: return "TK_RETURN";
        case TK_ENUM:       return "TK_ENUM";
        case TK_STRUCT:     return "TK_STRUCT";
        case TK_ALIGNAS:    return "TK_ALIGNAS";
        case TK_TYPEDEF:    return "TK_TYPEDEF";
        case TK_SIZEOF:     return "TK_SIZEOF";
        case TK_ALIGNOF:    return "TK_ALIGNOF";
        case TK_OFFSETOF:   return "TK_OFFSETOF";
        case TK_TYPEOF:     return "TK_TYPEOF";
        case TK_EOF: return "TK_EOF";
        default: return "TK_UNKNOWN";
    }
}
