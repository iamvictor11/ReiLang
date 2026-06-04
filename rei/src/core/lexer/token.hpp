#pragma once
#include "core/base/common.hpp"
#include <string>
#include <vector>

namespace rei::Token
{
    enum Type : uint8_t
    {
        /* 注释 */
        TK_NOTE, // //
        /* 基础 */
        TK_ASSIGN, // =
        TK_WALRUS, // :=
        TK_CLONE,  // $=
        TK_WCLONE, // $:=
        /* 标识符 */
        TK_IDENT, // <ident>
        /* 字面量 */
        TK_LIT_INT,    // <int>
        TK_LIT_FLOAT,  // <float>
        TK_LIT_STRING, // <string>
        /* 数学运算 */
        TK_ADD,      // +
        TK_SUB,      // -
        TK_MUL,      // *
        TK_DIV,      // /
        TK_MOD,      // %
        TK_POW,      // **
        TK_SELF_ADD, // +=
        TK_SELF_SUB, // -=
        TK_SELF_MUL, // *=
        TK_SELF_DIV, // /=
        TK_SELF_MOD, // %=
        TK_SELF_POW, // **=
        /* 位运算 */
        TK_BIT_AND,       // &
        TK_BIT_OR,        // |
        TK_BIT_XOR,       // ^
        TK_BIT_XNOR,      // `
        TK_BIT_NOT,       // ~
        TK_BIT_SHL,       // <<
        TK_BIT_SHR,       // >>
        TK_SELF_BIT_AND,  // &=
        TK_SELF_BIT_OR,   // |=
        TK_SELF_BIT_XOR,  // ^=
        TK_SELF_BIT_XNOR, // `=
        TK_SELF_BIT_NOT,  // ~=
        TK_SELF_BIT_SHL,  // <<=
        TK_SELF_BIT_SHR,  // >>=
        /* 比较运算 */
        TK_EQ, // ==
        TK_NE, // !=
        TK_LT, // <
        TK_LE, // <=
        TK_GT, // >
        TK_GE, // >=
        /* 逻辑运算 */
        TK_AND, // &&
        TK_OR,  // ||
        TK_NOT, // !
        /* 保留词 */
        TK_NIL,         // nil
        TK_TRUE,        // true
        TK_FALSE,       // false
        TK_DEF,         // def
        TK_VAR,         // var
        TK_LET,         // let
        TK_REF,         // ref
        TK_TYPE_INT,    // int
        TK_TYPE_FLOAT,  // float
        TK_TYPE_STRING, // string
        TK_PACKAGE,     // package
        TK_IMPORT,      // import
        TK_IF,          // if
        TK_ELIF,        // elif
        TK_ELSE,        // else
        TK_LOOP,        // loop
        TK_UNTIL,       // until
        TK_CONTINUE,    // continue
        TK_BREAK,       // break
        TK_FUNC,        // func
        TK_ONCE,        // once
        TK_RETURN,      // return
        TK_PRINT,       // print
        TK_PRINTLN,     // println
        /* 区域 */
        TK_THEN,     // then
        TK_DO,       // do
        TK_BEG,      // beg
        TK_END,      // end
        TK_LPAREN,   // (
        TK_RPAREN,   // )
        TK_LBRACKET, // [
        TK_RBRACKET, // ]
        TK_LBRACE,   // {
        TK_RBRACE,   // }
        /* 其他 */
        TK_DOT,       // .
        TK_COMMA,     // ,
        TK_SEMICOLON, // ;
        TK_COLON,     // :
        TK_DCOLON,    // ::
        TK_RARROW,    // ->
        TK_LARROW,    // <-
        /* 结束 */
        TK_EOF // <eof>
    };
    Type toTypeFromKeyword(std::string_view s);
    Value::Data toLiteralFromKeyword(Type t);
    std::string toString(Type t);
    std::string toSymbol(Type t);
    struct Unit final
    {
    public:
        Type type;
        std::string_view lexeme;
        Value::Data literal;
        Position pos;
    public:
        Unit(Type t, std::string_view l, Position p) : type(t), lexeme(l), pos(p) {}
        Unit(Type t, std::string_view l, Value::Data v, Position p) : type(t), lexeme(l), literal(std::move(v)), pos(p) {}
    public:
        std::string toString() const;
        std::string toSymbol() const;
    };
    using List = std::vector<Unit>;
}