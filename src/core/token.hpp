#pragma once
#include "common.hpp"
#include <string>
#include <vector>

namespace luna::Token
{
    #define LUNA_TOKEN_TYPE_LIST \
        /* 注释 */ \
        LUNA_TOKEN_X(NOTE, "//") \
        /* 基础 */ \
        LUNA_TOKEN_X(DEF, "#") \
        LUNA_TOKEN_X(ASSIGN, "=") \
        /* 标识符 */ \
        LUNA_TOKEN_X(IDENT, "<ident>") \
        /* 字面量 */ \
        LUNA_TOKEN_X(LIT_INT, "<int>") \
        LUNA_TOKEN_X(LIT_FLOAT, "<float>") \
        LUNA_TOKEN_X(LIT_STRING, "<string>") \
        /* 数学运算 */ \
        LUNA_TOKEN_X(ADD, "+") \
        LUNA_TOKEN_X(SUB, "-") \
        LUNA_TOKEN_X(MUL, "*") \
        LUNA_TOKEN_X(DIV, "/") \
        LUNA_TOKEN_X(MOD, "%") \
        LUNA_TOKEN_X(POW, "**") \
        LUNA_TOKEN_X(SELF_ADD, "+=") \
        LUNA_TOKEN_X(SELF_SUB, "-=") \
        LUNA_TOKEN_X(SELF_MUL, "*=") \
        LUNA_TOKEN_X(SELF_DIV, "/=") \
        LUNA_TOKEN_X(SELF_MOD, "%=") \
        LUNA_TOKEN_X(SELF_POW, "**=") \
        /* 位运算 */ \
        LUNA_TOKEN_X(BIT_AND, "&") \
        LUNA_TOKEN_X(BIT_OR, "|") \
        LUNA_TOKEN_X(BIT_XOR, "^") \
        LUNA_TOKEN_X(BIT_XNOR, "`") \
        LUNA_TOKEN_X(BIT_NOT, "~") \
        LUNA_TOKEN_X(BIT_SHL, "<<") \
        LUNA_TOKEN_X(BIT_SHR, ">>") \
        LUNA_TOKEN_X(SELF_BIT_AND, "&=") \
        LUNA_TOKEN_X(SELF_BIT_OR, "|=") \
        LUNA_TOKEN_X(SELF_BIT_XOR, "^=") \
        LUNA_TOKEN_X(SELF_BIT_XNOR, "`=") \
        LUNA_TOKEN_X(SELF_BIT_NOT, "~=") \
        LUNA_TOKEN_X(SELF_BIT_SHL, "<<=") \
        LUNA_TOKEN_X(SELF_BIT_SHR, ">>=") \
        /* 比较运算 */ \
        LUNA_TOKEN_X(EQ, "==") \
        LUNA_TOKEN_X(NE, "!=") \
        LUNA_TOKEN_X(LT, "<") \
        LUNA_TOKEN_X(LE, "<=") \
        LUNA_TOKEN_X(GT, ">") \
        LUNA_TOKEN_X(GE, ">=") \
        /* 逻辑运算 */ \
        LUNA_TOKEN_X(NOT, "!") \
        LUNA_TOKEN_X(AND, "&&") \
        LUNA_TOKEN_X(OR, "||") \
        /* 关键字 */ \
        LUNA_TOKEN_X(IMPORT, "import") \
        LUNA_TOKEN_X(NIL, "nil") \
        LUNA_TOKEN_X(TRUE, "true") \
        LUNA_TOKEN_X(FALSE, "false") \
        LUNA_TOKEN_X(IF, "if") \
        LUNA_TOKEN_X(ELIF, "elif") \
        LUNA_TOKEN_X(ELSE, "else") \
        LUNA_TOKEN_X(LOOP, "loop") \
        LUNA_TOKEN_X(CONTINUE, "continue") \
        LUNA_TOKEN_X(BREAK, "break") \
        LUNA_TOKEN_X(FUNC, "func") \
        LUNA_TOKEN_X(RETURN, "return") \
        LUNA_TOKEN_X(STRUCT, "struct") \
        LUNA_TOKEN_X(INTERFACE, "interface") \
        LUNA_TOKEN_X(CLASS, "class") \
        LUNA_TOKEN_X(PUBLIC, "public") \
        LUNA_TOKEN_X(PRIVATE, "private") \
        LUNA_TOKEN_X(ENTITY, "entity") \
        LUNA_TOKEN_X(COMPONENT, "component") \
        LUNA_TOKEN_X(NAMESPACE, "namespace") \
        /* 区域 */ \
        LUNA_TOKEN_X(LPAREN, "(") \
        LUNA_TOKEN_X(RPAREN, ")") \
        LUNA_TOKEN_X(LBRACKET, "[") \
        LUNA_TOKEN_X(RBRACKET, "]") \
        LUNA_TOKEN_X(LBRACE, "{") \
        LUNA_TOKEN_X(RBRACE, "}") \
        /* 其他 */ \
        LUNA_TOKEN_X(RARROW, "->") \
        LUNA_TOKEN_X(LARROW, "<-") \
        LUNA_TOKEN_X(RDARROW, "->>") \
        LUNA_TOKEN_X(LDARROW, "<<-") \
        LUNA_TOKEN_X(DOT, ".") \
        LUNA_TOKEN_X(COMMA, ",") \
        LUNA_TOKEN_X(SEMICOLON, ";") \
        LUNA_TOKEN_X(DCOLON, "::") \
        /* 结束 */ \
        LUNA_TOKEN_X(EOF, "<eof>")
    enum Type : uint8_t
    {
        #define LUNA_TOKEN_X(name, symbol) TK_##name,
        LUNA_TOKEN_TYPE_LIST
        #undef LUNA_TOKEN_X
        TK_COUNT
    };
    std::string to_symbol(Type type);
    std::string to_string(Type type);
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
        std::string symbol() const;
    };
    using List = std::vector<Unit>;
}