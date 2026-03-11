#ifndef LUNA_COMMON_TOKEN_H
#define LUNA_COMMON_TOKEN_H
#include "value.h"

typedef enum luna_TokenType
{
    /* 注释 */
    LUNA_TK_NOTE, // //
    /* 基础 */
    LUNA_TK_ASSIGN, // =
    LUNA_TK_WALRUS, // :=
    /* 标识符 */
    LUNA_TK_IDENT, // <ident>
    /* 字面量 */
    LUNA_TK_LIT_INT,    // <int>
    LUNA_TK_LIT_FLOAT,  // <float>
    LUNA_TK_LIT_STRING, // <string>
    /* 数学运算 */
    LUNA_TK_ADD,      // +
    LUNA_TK_SUB,      // -
    LUNA_TK_MUL,      // *
    LUNA_TK_DIV,      // /
    LUNA_TK_MOD,      // %
    LUNA_TK_POW,      // **
    LUNA_TK_SELF_ADD, // +=
    LUNA_TK_SELF_SUB, // -=
    LUNA_TK_SELF_MUL, // *=
    LUNA_TK_SELF_DIV, // /=
    LUNA_TK_SELF_MOD, // %=
    LUNA_TK_SELF_POW, // **=
    /* 位运算 */
    LUNA_TK_BIT_AND,       // &
    LUNA_TK_BIT_OR,        // |
    LUNA_TK_BIT_XOR,       // ^
    LUNA_TK_BIT_XNOR,      // `
    LUNA_TK_BIT_NOT,       // ~
    LUNA_TK_BIT_SHL,       // <<
    LUNA_TK_BIT_SHR,       // >>
    LUNA_TK_SELF_BIT_AND,  // &=
    LUNA_TK_SELF_BIT_OR,   // |=
    LUNA_TK_SELF_BIT_XOR,  // ^=
    LUNA_TK_SELF_BIT_XNOR, // `=
    LUNA_TK_SELF_BIT_NOT,  // ~=
    LUNA_TK_SELF_BIT_SHL,  // <<=
    LUNA_TK_SELF_BIT_SHR,  // >>=
    /* 比较运算 */
    LUNA_TK_EQ, // ==
    LUNA_TK_NE, // !=
    LUNA_TK_LT, // <
    LUNA_TK_LE, // <=
    LUNA_TK_GT, // >
    LUNA_TK_GE, // >=
    /* 逻辑运算 */
    LUNA_TK_AND, // &&
    LUNA_TK_OR,  // ||
    LUNA_TK_NOT, // !
    /* 保留词 */
    LUNA_TK_NIL,         // nil
    LUNA_TK_TRUE,        // true
    LUNA_TK_FALSE,       // false
    LUNA_TK_DEF,         // def
    LUNA_TK_VAR,         // var
    LUNA_TK_LET,         // let
    LUNA_TK_REF,         // ref
    LUNA_TK_TYPE_INT,    // int
    LUNA_TK_TYPE_FLOAT,  // float
    LUNA_TK_TYPE_STRING, // string
    LUNA_TK_PACKAGE,     // package
    LUNA_TK_IMPORT,      // import
    LUNA_TK_IF,          // if
    LUNA_TK_ELIF,        // elif
    LUNA_TK_ELSE,        // else
    LUNA_TK_LOOP,        // loop
    LUNA_TK_CONTINUE,    // continue
    LUNA_TK_BREAK,       // break
    LUNA_TK_FUNC,        // func
    LUNA_TK_RETURN,      // return
    LUNA_TK_STRUCT,      // struct
    LUNA_TK_NAMESPACE,   // namespace
    LUNA_TK_GLOBAL,      // global
    LUNA_TK_LOCAL,       // local
    LUNA_TK_INTERFACE,   // interface
    LUNA_TK_CLASS,       // class
    LUNA_TK_THIS,        // this
    LUNA_TK_SUPER,       // super
    LUNA_TK_STATIC,      // static
    LUNA_TK_PUBLIC,      // public
    LUNA_TK_PROTECTED,   // protected
    LUNA_TK_PRIVATE,     // private
    LUNA_TK_VIRTUAL,     // virtual
    LUNA_TK_OVERRIDE,    // override
    LUNA_TK_IS,          // is
    LUNA_TK_ENTITY,      // entity
    LUNA_TK_COMPONENT,   // component
    LUNA_TK_HAS,         // has
    LUNA_TK_NEW,         // new
    LUNA_TK_DEL,         // del
    LUNA_TK_PRINT,       // print
    LUNA_TK_PRINTLN,     // println
    /* 区域 */
    LUNA_TK_LPAREN,   // (
    LUNA_TK_RPAREN,   // )
    LUNA_TK_LBRACKET, // [
    LUNA_TK_RBRACKET, // ]
    LUNA_TK_LBRACE,   // {
    LUNA_TK_RBRACE,   // }
    /* 其他 */
    LUNA_TK_DOT,       // .
    LUNA_TK_COMMA,     // ,
    LUNA_TK_SEMICOLON, // ;
    LUNA_TK_COLON,     // :
    LUNA_TK_DCOLON,    // ::
    LUNA_TK_RARROW,    // ->
    LUNA_TK_LARROW,    // <-
    /* 结束 */
    LUNA_TK_EOF // <eof>
} luna_TokenType;

typedef struct luna_Token
{
    luna_TokenType type;
    luna_Position pos;
    luna_StringView lexeme;
    luna_Value literal;
} luna_Token;

typedef struct luna_TokenArray
{
  luna_Token* data;
  size_t size;
  size_t capacity;
} luna_TokenArray;
void lunaTokenArray_init(luna_TokenArray *self);
void lunaTokenArray_free(luna_TokenArray *self);
void lunaTokenArray_write(luna_TokenArray *self, luna_Token element);

luna_TokenType lunaTokenType_fromKeyword(const char *lexeme, size_t len);
luna_Value lunaToken_toLiteral(luna_TokenType type);

#endif