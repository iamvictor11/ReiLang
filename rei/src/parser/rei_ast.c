#include "rei_ast.h"
#include "../rei_allocator.h"

C_TEMPLATE_IMPL_VECTOR(, rei, Rei, AstForest, ReiAstNodePtr, &reiAllocator_g)
#define REI_IMPL_AST(NAME) \
    C_TEMPLATE_IMPL_VECTOR(, rei, Rei, Ast##NAME##Forest, ReiAst##NAME##NodePtr, &reiAllocator_g)
REI_IMPL_AST(Root)
REI_IMPL_AST(DeclMut)
REI_IMPL_AST(DeclKon)
REI_IMPL_AST(DeclFunc)
REI_IMPL_AST(DeclClass)
REI_IMPL_AST(ExprGroup)
REI_IMPL_AST(ExprLiteral)
REI_IMPL_AST(ExprIdentifier)
REI_IMPL_AST(ExprUnary)
REI_IMPL_AST(ExprBinary)
REI_IMPL_AST(ExprLogic)
REI_IMPL_AST(ExprAtix)
REI_IMPL_AST(ExprCall)
REI_IMPL_AST(ExprNew)
REI_IMPL_AST(ExprDel)
REI_IMPL_AST(ExprIs)
REI_IMPL_AST(StmtBlock)
REI_IMPL_AST(StmtWhen)
REI_IMPL_AST(StmtWhile)
REI_IMPL_AST(StmtBreak)
REI_IMPL_AST(StmtContinue)
REI_IMPL_AST(StmtSwitch)
REI_IMPL_AST(StmtCase)
REI_IMPL_AST(StmtDefault)
REI_IMPL_AST(StmtPass)
REI_IMPL_AST(StmtReturn)
#undef REI_IMPL_AST

