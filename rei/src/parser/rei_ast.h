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
    REI_AST_NODE_TYPE_EXPR_OR,
    REI_AST_NODE_TYPE_EXPR_AND,
    REI_AST_NODE_TYPE_EXPR_CALL,
    REI_AST_NODE_TYPE_EXPR_ARGS,
    REI_AST_NODE_TYPE_EXPR_THIS,
    REI_AST_NODE_TYPE_EXPR_SUPER,
    REI_AST_NODE_TYPE_EXPR_NEW,
    REI_AST_NODE_TYPE_EXPR_DEL,
    REI_AST_NODE_TYPE_EXPR_IS,

    REI_AST_NODE_TYPE_STMT_BLOCK,
    REI_AST_NODE_TYPE_STMT_WHEN,
    REI_AST_NODE_TYPE_STMT_LOOP,
    REI_AST_NODE_TYPE_STMT_CONTINUE,
    REI_AST_NODE_TYPE_STMT_BREAK,
    REI_AST_NODE_TYPE_STMT_DO,
    REI_AST_NODE_TYPE_STMT_SWITCH,
    REI_AST_NODE_TYPE_STMT_CASE,
    REI_AST_NODE_TYPE_STMT_PASS,
    REI_AST_NODE_TYPE_STMT_DEFAULT,
    REI_AST_NODE_TYPE_STMT_RETURN,
    REI_AST_NODE_TYPE_STMT_THIS,
    REI_AST_NODE_TYPE_STMT_SUPER,

    REI_AST_NODE_TYPE_DECL_LOWER_BOUND = REI_AST_NODE_TYPE_DECL_MUT,
    REI_AST_NODE_TYPE_DECL_UPPER_BOUND = REI_AST_NODE_TYPE_DECL_CLASS,
    REI_AST_NODE_TYPE_EXPR_LOWER_BOUND = REI_AST_NODE_TYPE_EXPR_GROUP,
    REI_AST_NODE_TYPE_EXPR_UPPER_BOUND = REI_AST_NODE_TYPE_EXPR_SUPER,
    REI_AST_NODE_TYPE_STMT_LOWER_BOUND = REI_AST_NODE_TYPE_STMT_BLOCK,
    REI_AST_NODE_TYPE_STMT_UPPER_BOUND = REI_AST_NODE_TYPE_STMT_SUPER,

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
typedef ReiAstNode* ReiAstNodePtr;
C_TEMPLATE_DECL_VECTOR(, rei, Rei, AstForest, ReiAstNodePtr)
C_TEMPLATE_DEFN_VECTOR(, rei, Rei, AstForest, ReiAstNodePtr)
struct ReiAst
{
    ReiAstNodeType type;
    ReiAstNodeCategory category;
    ReiToken token;
    ReiAstNode* parent;
    ReiAstForest_T children;
    ReiAstNode* sibling;
};
ReiAstNode* reiAstNodeCreateEmpty(ReiAstNodeType type, ReiToken token);
ReiAstNode* reiAstNodeCreate(ReiAstNodeType type, ReiToken token, int childCount, ...);
void reiAstNodeDestroy(ReiAstNode* me, bool destroyChildren);
void reiAstNodeAddChild(ReiAstNode* me, ReiAstNode* child);
ReiAstNode* reiAstNodeFrontChild(ReiAstNode* me);
ReiAstNode* reiAstNodeBackChild(ReiAstNode* me);
bool reiAstNodeHasChildren(ReiAstNode* me);
uint32_t reiAstNodeChildCount(ReiAstNode* me);
ReiAstNode* reiAstNodeGetChild(ReiAstNode* me, uint32_t index);
static inline ReiAstNodeCategory reiAstNodeTypeToCategory(ReiAstNodeType type)
{
    if (type == REI_AST_NODE_TYPE_ROOT)
        return REI_AST_NODE_CATEGORY_ROOT;
    else if (type >= REI_AST_NODE_TYPE_DECL_LOWER_BOUND && type <= REI_AST_NODE_TYPE_DECL_UPPER_BOUND)
        return REI_AST_NODE_CATEGORY_DECL;
    else if (type >= REI_AST_NODE_TYPE_EXPR_LOWER_BOUND && type <= REI_AST_NODE_TYPE_EXPR_UPPER_BOUND)
        return REI_AST_NODE_CATEGORY_EXPR;
    else if (type >= REI_AST_NODE_TYPE_STMT_LOWER_BOUND && type <= REI_AST_NODE_TYPE_STMT_UPPER_BOUND)
        return REI_AST_NODE_CATEGORY_STMT;
    return REI_AST_NODE_CATEGORY_MAX_COUNT;
}
#define REI_IS_ROOT_AST(NODE) ((NODE).category == REI_AST_NODE_CATEGORY_ROOT)
#define REI_IS_DECL_AST(NODE) ((NODE).category == REI_AST_NODE_CATEGORY_DECL)
#define REI_IS_EXPR_AST(NODE) ((NODE).category == REI_AST_NODE_CATEGORY_EXPR)
#define REI_IS_STMT_AST(NODE) ((NODE).category == REI_AST_NODE_CATEGORY_STMT)

#endif
