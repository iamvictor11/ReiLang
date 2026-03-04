#include "visitor.hpp"
#include <iostream>

namespace luna::ast
{
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
