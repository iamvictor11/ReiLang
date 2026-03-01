#ifndef LUNA_AST_H
#define LUNA_AST_H

#include "base.h"
#include "operator.h"

typedef enum luna_ASTNodeType
{
    LUNA_AST_PROGRAM, // 程序根节点
    /*表达式*/
    LUNA_AST_EXPR_LIT,    // 字面量 nil int float strings
    LUNA_AST_EXPR_VAR,    // 变量名
    LUNA_AST_EXPR_BINARY, // 二元运算
    LUNA_AST_EXPR_UNARY,  // 一元运算
    LUNA_AST_EXPR_TABLE,  // 表单构造 { expr, expr, ... }
    LUNA_AST_EXPR_FUNC,   // 函数构造 (params) { body }
    LUNA_AST_EXPR_INDEX,  // 表单访问
    LUNA_AST_EXPR_CALL,   // 函数调用
    /*语句*/
    LUNA_AST_STMT_DEF,      // 定义语句 #name = expr
    LUNA_AST_STMT_ASSIGN,   // 赋值语句 name = expr
    LUNA_AST_STMT_IF,       // 条件语句 ? cond { body } :? cond { body } : { body }
    LUNA_AST_STMT_LOOP,     // 循环语句 <> expr { body }
    LUNA_AST_STMT_BLOCK,    // 代码块 { ... }
    LUNA_AST_STMT_RETURN,   // 返回语句 +> expr
    LUNA_AST_STMT_CONTINUE, // 继续语句 :>
    LUNA_AST_STMT_BREAT     // 中断语句 :>>
    /*其他*/
} luna_ASTNodeType;
typedef struct luna_ASTNode luna_ASTNode;
typedef luna_Var luna_ASTExprLit;
typedef char * luna_ASTExprVar;
typedef struct luna_ASTExprBinary luna_ASTExprBinary;
typedef struct luna_ASTExprUnary luna_ASTExprUnary;
typedef struct luna_ASTExprTable luna_ASTExprTable;
typedef struct luna_ASTExprFunc luna_ASTExprFunc;
typedef struct luna_ASTExprIndex luna_ASTExprIndex;
typedef struct luna_ASTExprCall luna_ASTExprCall;
typedef struct luna_ASTStmtDef luna_ASTStmtDef;
typedef struct luna_ASTStmtAssign luna_ASTStmtAssign;
typedef struct luna_ASTStmtIf luna_ASTStmtIf;
typedef struct luna_ASTStmtLoop luna_ASTStmtLoop;
typedef struct luna_ASTStmtBlock luna_ASTStmtBlock;
typedef struct luna_ASTStmtReturn luna_ASTStmtReturn;
typedef struct luna_ASTStmtContinue luna_ASTStmtContinue;
typedef struct luna_ASTStmtBreat luna_ASTStmtBreat;
typedef struct luna_ASTNodeList luna_ASTNodeList;
typedef struct luna_ASTNodeList
{
    luna_ASTNode **nodes;
    int size;
    int capacity;
} luna_ASTNodeList;
// 表达式
// 二元运算
struct
{
    luna_Operator op;
    luna_ASTNode *left;
    luna_ASTNode *right;
} binary;
// 一元运算
struct
{
    luna_Operator op;
    luna_ASTNode *self;
} unary;
// 表单构造
struct
{
    luna_ASTNodeList *values;
} table;
// 函数构造
struct
{
    luna_ASTNodeList *params;
    luna_ASTNode *body;
} func;
// 表单访问
struct
{
    /* data */
} index;
// 函数调用
struct
{
    luna_ASTNode *callee;
    luna_ASTNodeList **args;
    int arg_count;
} call;
// 语句
// 定义语句
// 赋值语句
struct
{
    luna_ASTNode *from;
    luna_ASTNode *to;
} assign;
// 条件语句
struct
{

} ifthel;
// 循环语句
// 语句块
struct
{
    luna_ASTNodeList stmts;
} block;
// 返回语句
// 继续语句
// 中断语句
typedef struct luna_ASTNode
{
    luna_ASTNodeType type;
    int line;
    int column;
    union
    {
    } data;
} luna_ASTNode;

luna_ASTNode *lunaAST_CreateNode(luna_ASTNodeType type, int line, int column);
void lunaAST_FreeNode(luna_ASTNode *node);
void lunaAST_FreeNodeList(luna_ASTNodeList *list);
void lunaAST_NodeListInit(luna_ASTNodeList *list);
void lunaAST_NodeListAdd(luna_ASTNodeList *list, luna_ASTNode *node);

#endif