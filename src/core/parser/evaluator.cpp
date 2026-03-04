#include "visitor.hpp"
#include <iostream>

namespace luna::ast
{
    Value::Data Evaluator::operator()(const Node& node)
    {
        return std::visit(overloaded
        {
            [](const Expr::Literal& e) -> Value::Data
            {
                return e.value; 
            },
            [&](const Expr::Unary& e) -> Value::Data
            {
                Value::Data right = this->operator()(*e.right);
                if (e.op == Token::TK_SUB) return -std::get<double>(right);
                return right;
            },
            [&](const Expr::Binary& e) -> Value::Data
            {
                Value::Data left = this->operator()(*e.left);
                Value::Data right = this->operator()(*e.right);
                switch (e.op)
                {
                    case Token::TK_ADD: return std::get<double>(left) + std::get<double>(right);
                    case Token::TK_MUL: return std::get<double>(left) * std::get<double>(right);
                    default: return 0.0;
                }
            },
            [&](const Expr::Grouping& e) -> Value::Data
            {
                return this->operator()(*e.expression);
            }
        }, node.data);
    }
}
