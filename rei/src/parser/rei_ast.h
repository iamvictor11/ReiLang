#ifndef REI_PARSER_AST_H
#define REI_PARSER_AST_H

#define REI_DECL_AST_NODE(NAME) typedef struct Rei##NAME##Node Rei##NAME##Node;
REI_DECL_AST_NODE(Root)
REI_DECL_AST_NODE(Literal)
REI_DECL_AST_NODE(Identifier)
REI_DECL_AST_NODE(VariableDeclaration)
REI_DECL_AST_NODE(EnumDeclaration)
REI_DECL_AST_NODE(FlagDeclaration)
REI_DECL_AST_NODE(FunctionDeclaration)
REI_DECL_AST_NODE(ClassDeclaration)
REI_DECL_AST_NODE(NamespaceDeclaration)
REI_DECL_AST_NODE(UnaryExpression)
REI_DECL_AST_NODE(BinaryExpression)
REI_DECL_AST_NODE(AtixExpression)
REI_DECL_AST_NODE(CallExpression)
REI_DECL_AST_NODE(MemberExpression)
REI_DECL_AST_NODE(BlockStatement)
REI_DECL_AST_NODE(IfStatement)
REI_DECL_AST_NODE(WhileStatement)
REI_DECL_AST_NODE(BreakStatement)
REI_DECL_AST_NODE(ContinueStatement)
REI_DECL_AST_NODE(SwitchStatement)
REI_DECL_AST_NODE(PassStatement)
REI_DECL_AST_NODE(ReturnStatement)
REI_DECL_AST_NODE(NewStatement)
REI_DECL_AST_NODE(DelStatement)
REI_DECL_AST_NODE(IsStatement)
#undef REI_DECL_AST_NODE

struct ReiRootNode
{
};


#endif
