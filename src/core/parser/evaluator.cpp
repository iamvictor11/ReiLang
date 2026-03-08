#include "visitor.hpp"

namespace luna::ast
{
    Evaluator::ResType Evaluator::operator()(const Node& node)
    {
        using namespace Token;
        return std::visit(LambdaOverloaded
        {
            [&](const Program& n) { return _execute(n); },
            [](const Expr::Literal& n) { return n.value; },
            [&](const Expr::VarName& n) { return _execute(n); },
            [&](const Expr::Assign& n) { return _execute(n); },
            [&](const Expr::Unary& n) { return _execute(n); },
            [&](const Expr::Binary& n) { return _execute(n); },
            [&](const Expr::Grouping& n) { return _execute(n); },
            [&](const Stmt::Expression& n) { return _execute(n); },
            [&](const Stmt::Print& n) { return _execute(n); },
            [&](const Stmt::VarDecl& n) { return _execute(n); },
            [&](const Stmt::Block& n) { return _execute(n); }
        }, node.data);
    }
    Evaluator::ResType Evaluator::_execute(const Program& n)
    {
        for (size_t i = 0; i < n.nodes.size(); i++)
            this->operator()(*n.nodes[i]);
        return Nil{};
    }
}
