#ifndef REI_PARSER_AST_H
#define REI_PARSER_AST_H

#include "../lexer/rei_token.h"
#include REI_C_TEMPLATE_LIB_CONTAINER_VECTOR_H

typedef enum ReiAstNodeType
{
    REI_AST_NODE_TYPE_ROOT,
    REI_AST_NODE_TYPE_DECL_MUT,
    REI_AST_NODE_TYPE_DECL_KON,
    REI_AST_NODE_TYPE_DECL_FUNC,
    REI_AST_NODE_TYPE_DECL_CLASS,
    REI_AST_NODE_TYPE_EXPR_GROUP,
    REI_AST_NODE_TYPE_EXPR_LITERAL,
    REI_AST_NODE_TYPE_EXPR_IDENTIFIER,
    REI_AST_NODE_TYPE_EXPR_UNARY,
    REI_AST_NODE_TYPE_EXPR_BINARY,
    REI_AST_NODE_TYPE_EXPR_LOGIC,
    REI_AST_NODE_TYPE_EXPR_ATIX,
    REI_AST_NODE_TYPE_EXPR_CALL,
    REI_AST_NODE_TYPE_EXPR_NEW,
    REI_AST_NODE_TYPE_EXPR_DEL,
    REI_AST_NODE_TYPE_EXPR_IS,
    REI_AST_NODE_TYPE_STMT_BLOCK,
    REI_AST_NODE_TYPE_STMT_WHEN,
    REI_AST_NODE_TYPE_STMT_WHILE,
    REI_AST_NODE_TYPE_STMT_BREAK,
    REI_AST_NODE_TYPE_STMT_CONTINUE,
    REI_AST_NODE_TYPE_STMT_SWITCH,
    REI_AST_NODE_TYPE_STMT_CASE,
    REI_AST_NODE_TYPE_STMT_DEFAULT,
    REI_AST_NODE_TYPE_STMT_PASS,
    REI_AST_NODE_TYPE_STMT_RETURN,
    REI_AST_NODE_TYPE_MAX_COUNT
} ReiAstNodeType;
typedef enum ReiAstNodeCategory
{
    REI_AST_NODE_CATEGORY_ROOT,
    REI_AST_NODE_CATEGORY_DECL,
    REI_AST_NODE_CATEGORY_EXPR,
    REI_AST_NODE_CATEGORY_STMT,
    REI_AST_NODE_CATEGORY_MAX_COUNT
} ReiAstNodeCategory;
typedef struct ReiAst ReiAst, ReiAstNode;
typedef ReiAstNode* ReiAstNodePtr, ReiAstPtr;
C_TEMPLATE_DECL_VECTOR(, rei, Rei, AstForest, ReiAstNodePtr)
C_TEMPLATE_DEFN_VECTOR(, rei, Rei, AstForest, ReiAstNodePtr)
struct ReiAst
{
    ReiAstNodeType type;
    ReiAstNodeCategory category;
    ReiAstNodePtr parent;
    uint32_t line;
};
#define REI_DECL_AST(NAME) typedef struct ReiAst##NAME ReiAst##NAME, ReiAst##NAME##Node;
REI_DECL_AST(Root)
REI_DECL_AST(DeclMut)
REI_DECL_AST(DeclKon)
REI_DECL_AST(DeclFunc)
REI_DECL_AST(DeclClass)
REI_DECL_AST(ExprGroup)
REI_DECL_AST(ExprLiteral)
REI_DECL_AST(ExprIdentifier)
REI_DECL_AST(ExprUnary)
REI_DECL_AST(ExprBinary)
REI_DECL_AST(ExprLogic)
REI_DECL_AST(ExprAtix)
REI_DECL_AST(ExprCall)
REI_DECL_AST(ExprNew)
REI_DECL_AST(ExprDel)
REI_DECL_AST(ExprIs)
REI_DECL_AST(StmtBlock)
REI_DECL_AST(StmtWhen)
REI_DECL_AST(StmtWhile)
REI_DECL_AST(StmtBreak)
REI_DECL_AST(StmtContinue)
REI_DECL_AST(StmtSwitch)
REI_DECL_AST(StmtCase)
REI_DECL_AST(StmtDefault)
REI_DECL_AST(StmtPass)
REI_DECL_AST(StmtReturn)
#undef REI_DECL_AST
struct ReiAstRoot
{
    ReiAstNode base;
    ReiAstForest children;
};
struct ReiAstDeclMut
{
    ReiAstNode base;
    ReiValue name;
    ReiAstNode* expr;
};
struct ReiAstDeclKon
{
    ReiAstNode base;
    ReiValue name;
    ReiAstNode* expr;
};
struct ReiAstDeclFunc
{
    ReiAstNode base;
    ReiValue name;
    ReiAstForest args;
    ReiAstStmtBlock* block;
    ReiAstForest returns;
};
struct ReiAstDeclClass
{
    ReiAstNode base;
    ReiValue name;
    ReiAstForest propertys;
    ReiAstForest methods;
};
struct ReiAstExprGroup
{
    ReiAstNode base;
    ReiAstForest exprs;
};
struct ReiAstExprLiteral
{
    ReiAstNode base;
    ReiValue value;
};
struct ReiAstExprIdentifier
{
    ReiAstNode base;
    ReiValue string;
};
struct ReiAstExprUnary
{
    ReiAstNode base;
    ReiTokenKind oper;
    ReiAstNode* expr;
};
struct ReiAstExprBinary
{
    ReiAstNode base;
    ReiAstNode* leftExpr;
    ReiTokenKind oper;
    ReiAstNode* rightExpr;
};
struct ReiAstExprLogic
{
    ReiAstNode base;
    ReiAstNode* leftExpr;
    ReiTokenKind oper;
    ReiAstNode* rightExpr;
};
struct ReiAstExprAtix
{
    ReiAstNode base;
    ReiAstNode* leftExpr;
    ReiAstNode* centerExpr;
};
struct ReiAstExprCall
{
    ReiAstNode base;
    ReiAstNode* expr;
    ReiAstForest args;
};
struct ReiAstExprNew
{
    ReiAstNode base;
    ReiAstExprIdentifier* name;
    ReiAstForest args;
};
struct ReiAstExprDel
{
    ReiAstNode base;
    ReiAstExprIdentifier* object;
};
struct ReiAstExprIs
{
    ReiAstNode base;
    ReiAstNode* expr;
    ReiAstExprIdentifier* name;
};
struct ReiAstStmtBlock
{
    ReiAstNode base;
    ReiAstForest nodes;
};
struct ReiAstStmtWhen
{
    ReiAstNode base;
    ReiAstNode* condition;
    ReiAstStmtBlock* block;
    union {ReiAstStmtWhen* elifn, elsen;};
};
struct ReiAstStmtWhile
{
    ReiAstNode base;
    ReiAstNode* doBlock;
    bool isUnitl;
    ReiAstNode* condition;
    ReiAstStmtBlock* block;
    ReiAstForest breaks;
    ReiAstForest continues;
};
struct ReiAstStmtBreak
{
    ReiAstNode base;
    uint32_t depth;
};
struct ReiAstStmtContinue
{
    ReiAstNode base;
    uint32_t depth;
};
struct ReiAstStmtSwitch
{
    ReiAstNode base;
    ReiAstForest cases;
    ReiAstStmtDefault* defaultBlock;
};
struct ReiAstStmtCase
{
    ReiAstNode base;
    int64_t index;
    ReiAstStmtBlock* block;
};
struct ReiAstStmtDefault
{
    ReiAstNode base;
    ReiAstStmtBlock* block;
};
struct ReiAstStmtPass
{
    ReiAstNode base;
    uint32_t depth;
    int64_t nextIndex;
};
struct ReiAstStmtReturn
{
    ReiAstNode base;
    uint32_t depth;
};

#endif
