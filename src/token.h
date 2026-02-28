#ifndef LUNA_TOKEN_H
#define LUNA_TOKEN_H

/*
* <VAL> 是值的字面量
* <VAR> 是变量，可以承接 nil, int, float, string, table, func
* <EXP> 是表达式，比如 (a * (b + 2))
*/
typedef enum luna_Token
{
    /*注释*/
    LUNA_TOKEN_NOTE, // //
    /*定义/赋值/变量名/字面量*/
    LUNA_TOKEN_DEF,         // #qux/#qux=<VAL>/#qux=<VAR>/#qux=<EXP>
    LUNA_TOKEN_ASSIGN,      // qux=<VAL>
    LUNA_TOKEN_VAR,         // qux
    LUNA_TOKEN_TYPE_NIL,    // nil
    LUNA_TOKEN_TYPE_INT,    // 1
    LUNA_TOKEN_TYPE_FLOAT,  // 42./.168/3.14
    LUNA_TOKEN_TYPE_STRING, // "bar"/'baz'
    LUNA_TOKEN_TYPE_TABLE,   // {} / {<VAL>, <VAR>, <EXP>, nil}
    LUNA_TOKEN_TYPE_FUNC,   // (){} / (a,b){}
    /*数学运算*/
    LUNA_TOKEN_ADD, // x+y
    LUNA_TOKEN_SUB, // x-y
    LUNA_TOKEN_MUL, // x*y
    LUNA_TOKEN_DIV, // x/y
    LUNA_TOKEN_MOD, // x%y
    // LUNA_TOKEN_NEG, // -x
    LUNA_TOKEN_POW, // x**y
    /*位运算*/
    LUNA_TOKEN_BIT_AND,  // x&y
    LUNA_TOKEN_BIT_OR,   // x|y
    LUNA_TOKEN_BIT_XOR,  // x^y
    LUNA_TOKEN_BIT_XNOR, // x`y
    LUNA_TOKEN_BIT_NOT,  // x~y
    LUNA_TOKEN_BIT_SHL,  // x<<y
    LUNA_TOKEN_BIT_SHR,  // x>>y
    /*比较运算*/
    LUNA_TOKEN_EQ, // x==y
    LUNA_TOKEN_NE, // x!=y
    LUNA_TOKEN_LT, // x<y
    LUNA_TOKEN_LE, // x<=y
    LUNA_TOKEN_GT, // x>y
    LUNA_TOKEN_GE, // x>=y
    /*逻辑运算*/
    LUNA_TOKEN_NOT, // !x
    LUNA_TOKEN_AND, // x&&y
    LUNA_TOKEN_OR,  // x||y
    /*条件操作*/
    LUNA_TOKEN_IF,   // ? <EXP> {}/? <EXP> is single <EXP>
    LUNA_TOKEN_ELIF, // :? <EXP> {}/? <EXP> is single <EXP>
    LUNA_TOKEN_ELSE, // : <EXP> {}/? <EXP> is single <EXP>
    /*循环操作*/
    LUNA_TOKEN_LOOP,     // <> <EXP> {}/<> <EXP> is single <EXP>
    LUNA_TOKEN_CONTINUE, // =>
    /*跳跃操作*/
    LUNA_TOKEN_BREAK, // =>>
    /*函数操作*/
    // LUNA_TOKEN_CALL,   // foo()/foo(a,b)
    LUNA_TOKEN_RETURN, // +>/+> x/+> x,y,z
    /*区域*/
    LUNA_TOKEN_LPAREN,   // (
    LUNA_TOKEN_RPAREN,   // )
    LUNA_TOKEN_LBRACKET, // [
    LUNA_TOKEN_RBRACKET, // ]
    LUNA_TOKEN_LBRACE,   // {
    LUNA_TOKEN_RBRACE,   // }
    LUNA_TOKEN_DOT,      // .
    LUNA_TOKEN_COMMA,    // ,
    /*终端*/
    LUNA_TOKEN_TERMINAL, // >_
    LUNA_TOKEN_PRINT,    // >_ <- <VAR>/<EXP>/<CODE>
    LUNA_TOKEN_SCAN,     // >_ -> <STRING>
    /*其他*/
    LUNA_TOKEN_EOF,
    LUNA_TOKEN_ERROR
} luna_Token;

#endif