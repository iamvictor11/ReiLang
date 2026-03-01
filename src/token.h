#ifndef LUNA_TOKEN_H
#define LUNA_TOKEN_H

typedef enum luna_Token
{
    /*注释*/
    LUNA_TOKEN_NOTE, // //
    /*定义/赋值/变量名/字面量*/
    LUNA_TOKEN_DEF,    // #
    LUNA_TOKEN_ASSIGN, // =
    LUNA_TOKEN_VAR,
    LUNA_TOKEN_LIT_NIL,
    LUNA_TOKEN_LIT_INT,
    LUNA_TOKEN_LIT_FLOAT,
    LUNA_TOKEN_LIT_STRING,
    // 复杂字面量在 Lexer 阶段不解析
    // LUNA_TOKEN_LIT_TABLE,
    // LUNA_TOKEN_LIT_FUNC,
    /*数学运算*/
    LUNA_TOKEN_ADD,      // +
    LUNA_TOKEN_SUB,      // -
    LUNA_TOKEN_MUL,      // *
    LUNA_TOKEN_DIV,      // /
    LUNA_TOKEN_MOD,      // %
    LUNA_TOKEN_POW,      // **
    LUNA_TOKEN_SELF_ADD, // +=
    LUNA_TOKEN_SELF_SUB, // -=
    LUNA_TOKEN_SELF_MUL, // *=
    LUNA_TOKEN_SELF_DIV, // /=
    LUNA_TOKEN_SELF_MOD, // %=
    LUNA_TOKEN_SELF_POW, // **=
    /*位运算*/
    LUNA_TOKEN_BIT_AND,       // &
    LUNA_TOKEN_BIT_OR,        // |
    LUNA_TOKEN_BIT_XOR,       // ^
    LUNA_TOKEN_BIT_XNOR,      // `
    LUNA_TOKEN_BIT_NOT,       // ~
    LUNA_TOKEN_BIT_SHL,       // <<
    LUNA_TOKEN_BIT_SHR,       // >>
    LUNA_TOKEN_SELF_BIT_AND,  // &=
    LUNA_TOKEN_SELF_BIT_OR,   // |=
    LUNA_TOKEN_SELF_BIT_XOR,  // ^=
    LUNA_TOKEN_SELF_BIT_XNOR, // `=
    LUNA_TOKEN_SELF_BIT_NOT,  // ~=
    LUNA_TOKEN_SELF_BIT_SHL,  // <<=
    LUNA_TOKEN_SELF_BIT_SHR,  // >>=
    /*比较运算*/
    LUNA_TOKEN_EQ, // ==
    LUNA_TOKEN_NE, // !=
    LUNA_TOKEN_LT, // <
    LUNA_TOKEN_LE, // <=
    LUNA_TOKEN_GT, // >
    LUNA_TOKEN_GE, // >=
    /*逻辑运算*/
    LUNA_TOKEN_NOT, // !
    LUNA_TOKEN_AND, // &&
    LUNA_TOKEN_OR,  // ||
    /*关键字*/
    LUNA_TOKEN_IF,       // ?
    LUNA_TOKEN_ELIF,     // :?
    LUNA_TOKEN_ELSE,     // :
    LUNA_TOKEN_LOOP,     // <>
    LUNA_TOKEN_RETURN,   // +>
    LUNA_TOKEN_CONTINUE, // :>
    LUNA_TOKEN_BREAT,    // :>>
    /*区域*/
    LUNA_TOKEN_LPAREN,   // (
    LUNA_TOKEN_RPAREN,   // )
    LUNA_TOKEN_LBRACKET, // [
    LUNA_TOKEN_RBRACKET, // ]
    LUNA_TOKEN_LBRACE,   // {
    LUNA_TOKEN_RBRACE,   // }
    /*功能*/
    LUNA_TOKEN_RARROW,  // ->
    LUNA_TOKEN_LARROW,  // <-
    LUNA_TOKEN_RDARROW, // ->>
    LUNA_TOKEN_LDARROW, // <<-
    LUNA_TOKEN_DOT,     // .
    LUNA_TOKEN_COMMA,   // ,
    /*终端*/
    LUNA_TOKEN_TERMINAL, // >_
    /*其他*/
    LUNA_TOKEN_EOF,
    LUNA_TOKEN_ERROR
} luna_Token;

#endif