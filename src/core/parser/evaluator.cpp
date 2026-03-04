#include "visitor.hpp"
#include <iostream>

namespace luna::ast
{
    Value::Data Evaluator::operator()(const Node& node)
    {
        using namespace Token;
        return std::visit(overloaded
        {
            [](const Expr::Literal& e) -> Value::Data { return e.value; },
            [&](const Expr::Unary& e) -> Value::Data
            {
                Value::Data right = this->operator()(*e.right);
                switch (e.op)
                {
                case TK_SUB: break;
                case TK_NOT: break;
                case TK_BIT_NOT: break;
                default:
                    break;
                }
                if (e.op == TK_SUB) return -std::get<double>(right);
                return right;
            },
            [&](const Expr::Binary& e) -> Value::Data
            {
                Value::Data left = this->operator()(*e.left);
                Value::Data right = this->operator()(*e.right);
                switch (e.op)
                {
                case TK_ASSIGN: break;
                case TK_WALRUS: break;
                case TK_SELF_ADD: break;
                case TK_SELF_SUB: break;
                case TK_SELF_MUL: break;
                case TK_SELF_DIV: break;
                case TK_SELF_MOD: break;
                case TK_SELF_BIT_AND: break;
                case TK_SELF_BIT_OR: break;
                case TK_SELF_BIT_XOR: break;
                case TK_SELF_BIT_XNOR: break;
                case TK_SELF_BIT_NOT: break;
                case TK_SELF_BIT_SHL: break;
                case TK_SELF_BIT_SHR: break;
                case TK_ADD: break;
                case TK_SUB: break;
                case TK_MUL: break;
                case TK_DIV: break;
                case TK_MOD: break;
                case TK_POW: break;
                case TK_BIT_AND: break;
                case TK_BIT_OR: break;
                case TK_BIT_XOR: break;
                case TK_BIT_XNOR: break;
                case TK_BIT_SHL: break;
                case TK_BIT_SHR: break;
                case TK_EQ: break;
                case TK_NE: break;
                case TK_LT: break;
                case TK_LE: break;
                case TK_GT: break;
                case TK_GE: break;
                case TK_AND: break;
                case TK_OR: break;
                case TK_NOT: break;
                    default: return Nil{};
                }
            },
            [&](const Expr::Grouping& e) -> Value::Data
            {
                return this->operator()(*e.expression);
            }
        }, node.data);
    }
}
