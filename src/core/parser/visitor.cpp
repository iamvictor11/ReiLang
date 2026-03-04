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

    Value::Data Printer::operator()(const Node& node)
    {
        std::visit(overloaded
        {
            [&](const Expr::Literal& e)
            {
                _printIndent();
                std::cout << "Literal: " << Value::toString(e.value) << std::endl;
            },
            [&](const Expr::Unary& e)
            {
                _printIndent();
                std::cout << "Unary: " << Token::toSymbol(e.op) << std::endl;
                _indent_level++;
                this->operator()(*e.right);
                _indent_level--;
            },
            [&](const Expr::Binary& e)
            {
                _printIndent();
                std::cout << "Binary: " << Token::toSymbol(e.op) << std::endl;
                _indent_level++;
                this->operator()(*e.left);
                this->operator()(*e.right);
                _indent_level--;
            },
            [&](const Expr::Grouping& e)
            {
                _printIndent();
                std::cout << "Grouping:" << std::endl;
                _indent_level++;
                this->operator()(*e.expression);
                _indent_level--;
            }
        }, node.data);
        return Nil{};
    }
    void Printer::_printIndent() const
    {
        std::string result;
        for (int i = 0; i < _indent_level; ++i)
            result += _indent_char;
        std::cout << result;
    }
}
