#include "rei_ast.h"
#include "../rei_allocator.h"
#include "../rei_debug.h"

C_TEMPLATE_IMPL_VECTOR(, rei, Rei, AstForest, ReiAstNodePtr, &reiAllocator_g)

ReiAstNode* reiAstNodeCreateEmpty(ReiAstNodeType type, ReiToken token)
{
    ReiAstNode* node = reiMalloc(ReiAstNode, 1);
    if (node != NULL)
    {
        REI_DEBUG_LOG_FATAL("语法树节点 创建时 内存不足！");
        node->type = type;
        node->category = reiAstNodeTypeToCategory(type);
        node->token = token;
        node->parent = NULL;
        reiAstForestInit(&(node->children), NULL);
    }
    return node;
}
ReiAstNode* reiAstNodeCreate(ReiAstNodeType type, ReiToken token, int childCount, ...)
{
    ReiAstNode* node = reiAstNodeCreateEmpty(type, token);
    va_list children;
    va_start(children, childCount);
    for (int i = 0; i < childCount; i++)
        reiAstNodeAddChild(node, va_arg(children, ReiAstNode*));
    va_end(children);
    return node;
}
void reiAstNodeDestroy(ReiAstNode* me, bool destroyChildren)
{
    if (destroyChildren)
        C_TEMPLATE_VECTOR_FOREACH(ReiAstNodePtr, child, &(me->children))
            reiAstNodeDestroy(child, true);
    reiFree(me);
}
void reiAstNodeAddChild(ReiAstNode* me, ReiAstNode* child)
{
    child->parent = me;
    ReiAstNode* sibling = reiAstNodeBackChild(me);
    if (sibling) sibling->sibling = child;
    reiAstForestPushLiteral(&(me->children), child);
}
ReiAstNode* reiAstNodeFrontChild(ReiAstNode* me)
{
    if (!reiAstNodeHasChildren(me)) return NULL;
    return *reiAstForestFront(&(me->children));
}
ReiAstNode* reiAstNodeBackChild(ReiAstNode* me)
{
    if (!reiAstNodeHasChildren(me)) return NULL;
    return *reiAstForestBack(&(me->children));
}
bool reiAstNodeHasChildren(ReiAstNode* me)
{
    return !reiAstForestEmpty(&(me->children));
}
uint32_t reiAstNodeChildCount(ReiAstNode* me)
{
    return reiAstForestSize(&(me->children));
}
ReiAstNode* reiAstNodeGetChild(ReiAstNode* me, uint32_t index)
{
    return *reiAstForestAt(&(me->children), index);
}
